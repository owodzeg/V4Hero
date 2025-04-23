#include "Entity.h"

#include <spdlog/spdlog.h>
#include <libzippp.h>
#include <filesystem>
#include <fstream>

#include <CoreManager.h>

using namespace libzippp;

using json = nlohmann::json;
namespace fs = std::filesystem;

Entity::Entity()
{

}

void Entity::LoadEntity(const std::string& path)
{
    SPDLOG_INFO("Loading PNGAnimation model from {}", path);
    std::uniform_real_distribution<double> roll(0.0, 1.0);

    auto f = fs::path(path);
    std::string model_name = f.stem().string();

    bool zip = false;

    if (f.extension() == ".zip")
    {
        SPDLOG_INFO(".zip was provided, we need to extract the file");
        zip = true;
    }

    json entity;

    if(zip)
    {
        ZipArchive zf(f.string());
        zf.open(ZipArchive::ReadOnly);
        ZipEntry entry = zf.getEntry("entity.json");
        if(!entry.isNull())
        {
            entity = json::parse(entry.readAsText());

            SPDLOG_INFO("Reading entity.json.");
        }
    }
    else
    {
        std::ifstream ent(model_name+"/entity.json");
        if(ent.good())
        {
            entity << ent;
        }
        else
        {
            SPDLOG_ERROR("No entity.json file found. Entity will use default behaviors and statistics.");
        }
    }

    if(entity.contains("defaultStats"))
    {
        if(entity["defaultStats"].contains("hp"))
        {
            maxHP = entity["defaultStats"]["hp"].get<int>();
            curHP = entity["defaultStats"]["hp"].get<int>();
        }

        if(entity["defaultStats"].contains("mindmg"))
        {
            minDmg = entity["defaultStats"]["mindmg"].get<int>();
        }

        if(entity["defaultStats"].contains("maxdmg"))
        {
            maxDmg = entity["defaultStats"]["maxdmg"].get<int>();
        }
    }

    if(entity.contains("specifications"))
    {
        if(entity["specifications"].contains("type"))
            entityType = convStringToTypeEnum(entity["specifications"]["type"]);
        if(entity["specifications"].contains("category"))
            entityCategory = convStringToCategoryEnum(entity["specifications"]["category"]);
    }

    if(entity.contains("behavior"))
    {
        if(entity["behavior"].contains("hit") && !entity["behavior"]["hit"].is_null())
            bh_hit = behavior.convStringToHitEnum(entity["behavior"]["hit"]);
        if(entity["behavior"].contains("death") && !entity["behavior"]["death"].is_null())
            bh_death = behavior.convStringToDeathEnum(entity["behavior"]["death"]);
        if(entity["behavior"].contains("spawn") && !entity["behavior"]["spawn"].is_null())
            bh_spawn = behavior.convStringToSpawnEnum(entity["behavior"]["spawn"]);
        if(entity["behavior"].contains("loot") && !entity["behavior"]["loot"].is_null())
            bh_loot = behavior.convStringToLootEnum(entity["behavior"]["loot"]);
        if(entity["behavior"].contains("decision") && !entity["behavior"]["decision"].is_null())
            bh_decision = behavior.convStringToDecisionEnum(entity["behavior"]["decision"]);
        if(entity["behavior"].contains("attack") && !entity["behavior"]["attack"].is_null())
            bh_attack = behavior.convStringToAttackEnum(entity["behavior"]["attack"]);
        if(entity["behavior"].contains("approach") && !entity["behavior"]["approach"].is_null())
            bh_approach = behavior.convStringToApproachEnum(entity["behavior"]["approach"]);
    }

    global_y = 1735;
    if(entity.contains("position"))
    {
        if(entity["position"].contains("y"))
            global_y = entity["position"]["y"].get<float>();
    }

    if(entity.contains("scale"))
    {
        if(entity["scale"].contains("x"))
            setScale(sf::Vector2f(entity["scale"]["x"].get<float>(), getScale().y));
        if(entity["scale"].contains("y"))
            setScale(sf::Vector2f(getScale().x, entity["scale"]["y"].get<float>()));
    }

    if(entity.contains("loot"))
    {
        SPDLOG_INFO("Loot table: {}", entity["loot"].dump());

        std::vector<Entity::Loot> new_loot;
        Func::parseEntityLoot(CoreManager::getInstance().getCore()->gen, roll, entity["loot"], new_loot);

        loot_table = new_loot;

        if(loot_table.size() > 0)
        {
            for(auto l : loot_table)
            {
                SPDLOG_INFO("This item will drop: {} {} {}", l.order_id[0], l.order_id[1], l.order_id[2]);
            }
        }
    }

    AnimatedObject::LoadConfig(path);

    handleSpawn();
}

void Entity::setEntityID(int new_entityID)
{
    entityID = new_entityID;
}

int Entity::getEntityID()
{
    return entityID;
}

std::vector<sf::FloatRect> Entity::getHitbox()
{
    return animation.animations[animation.currentAnimation].hitboxes;
}

// handlers - environment
void Entity::handleHit(float damage) // when inflicted damage from player
{
    if(!dead)
    {
        switch(bh_hit)
        {
            case Behavior::Hit::BH_HIT_DEFAULT: {
                curHP -= damage;
                break;
            }

            case Behavior::Hit::BH_HIT_STAGGER: {
                curHP -= damage;
                setAnimation("stagger");
                restartAnimation();
                break;
            }

            case Behavior::Hit::BH_HIT_STATIONARY_TWO_STATE: {
                curHP -= damage;

                if(curHP < maxHP * 0.35)
                    setAnimation("idle_damaged");
                restartAnimation();
                break;
            }
        }
    }
}

void Entity::handleDeath() // when hp goes under 0
{
    switch(bh_death)
    {
        case Behavior::Death::BH_DEATH_REMOVE_INSTANT: {
            forRemoval = true;

            if(bh_loot == Behavior::Loot::BH_LOOT_DROP_INSTANT)
                handleLoot();

            break;
        }
        case Behavior::Death::BH_DEATH_KIRAJIN: {
            dead = true;

            if(!death_start)
            {
                death_start = true;
                dead_timer.restart();
                hspeed = 700;
                vspeed = -1200;
                setAnimation("stagger");
                restartAnimation();
                setAnimationLoop(false);
            }
            else
            {
                if(dead_timer.getElapsedTime().asMilliseconds() > 1000)
                {
                    if(getAnimation() != "death" && getAnimation() != "death_despawn")
                    {
                        setAnimation("death");
                        restartAnimation();
                        setAnimationLoop(false);
                    }
                }

                if(dead_timer.getElapsedTime().asMilliseconds() > 4000)
                {
                    if(getAnimation() == "death")
                    {
                        setAnimation("death_despawn");
                        restartAnimation();
                        setAnimationLoop(false);

                        if(bh_loot == Behavior::Loot::BH_LOOT_DROP_FADEAWAY)
                            handleLoot();
                    }
                }

                if(dead_timer.getElapsedTime().asMilliseconds() > 6000)
                {
                    forRemoval = true;
                }
            }

            if(bh_loot == Behavior::Loot::BH_LOOT_DROP_INSTANT)
                handleLoot();

            break;
        }

    }
}

void Entity::handleNoise() // when fever is activated
{

}

void Entity::handleApproach() // when player gets close
{
    if(!approached)
    {
        switch(bh_approach)
        {
            case Behavior::Approach::BH_APPROACH_FAR: {

                    if(distanceToPlayer <= 2000)
                    {
                        approached = true;
                    }

                break;
            }
        }
    }
}

void Entity::handleCommand() // on player command inputted
{

}

// handlers - self
void Entity::handleDecisions() // what should the entity do?
{
    if(!dead)
    {
        if(bh_approach != Behavior::Approach::BH_APPROACH_DO_NOTHING)
        {
            if(!approached)
            {
                return;
            }
        }

        switch(bh_decision)
        {
            case Behavior::Decision::BH_DECISION_YARI_SLOW: {
                if(decisionTimer.getElapsedTime().asMilliseconds() > 8000)
                {
                    if(distanceToPlayer > 1400)
                    {
                        if(action > 3)
                        {
                            hspeed = -600;
                            setAnimation("walk_yari_focused");
                        }
                    }
                    else if(distanceToPlayer < 800)
                    {
                        if(action > 3)
                        {
                            hspeed = 600;
                            setAnimation("walk_yari_focused");
                        }
                    }
                    else // in range
                    {
                        if(action <= 4)
                        {
                            handleAttack();
                            if(attackTimer.getElapsedTime().asMilliseconds() > 1500)
                            {
                                threw = false;
                                attackTimer.restart();
                                setAnimation("attack_prefever");
                                restartAnimation();
                            }
                        }
                    }

                    if(actionTimer.getElapsedTime().asMilliseconds() >= 1000)
                    {
                        hspeed = 0;
                        actionTimer.restart();
                        action = rand() % 5;
                        setAnimation("idle_armed_focused");
                    }
                }

                break;
            }

            case Behavior::Decision::BH_DECISION_YARI_FAST: {

                if(distanceToPlayer > 1400)
                {
                    if(action > 3)
                    {
                        hspeed = -600;
                        setAnimation("walk_yari_focused");
                    }
                }
                else if(distanceToPlayer < 1100)
                {
                    if(action > 3)
                    {
                        hspeed = 600;
                        setAnimation("walk_yari_focused");
                    }
                }
                else // in range
                {
                    if(action <= 4)
                    {
                        handleAttack();
                        if(attackTimer.getElapsedTime().asMilliseconds() > 1500)
                        {
                            threw = false;
                            attackTimer.restart();
                            setAnimation("attack_prefever");
                        }
                    }
                }

                if(actionTimer.getElapsedTime().asMilliseconds() >= 1000)
                {
                    hspeed = 0;
                    actionTimer.restart();
                    action = rand() % 5;
                    setAnimation("idle_armed_focused");
                }

                break;
            }
        }
    }
}

void Entity::handleSpawn() // on entity creation
{
    switch(bh_spawn)
    {
        case Behavior::Spawn::BH_SPAWN_IDLE: {
            setAnimation("idle");
            break;
        }
    }
}

void Entity::handleAttack() // entity's attack
{
    if(attackTimer.getElapsedTime().asMilliseconds() > 500)
    {
        if(!threw)
        {
            float prj_xPos = global_x+local_x+hPos;
            float prj_yPos = global_y+local_y+vPos-90;
            float prj_hSpeed = -1800 - (rand()%50);
            float prj_vSpeed = -1800 - (rand()%70);
            auto prj = CoreManager::getInstance().getMissionController()->SendProjectile(prj_xPos, prj_yPos, prj_hSpeed, prj_vSpeed, wpn, true);
            prj->damage = minDmg + (rand() % std::min(int(minDmg-maxDmg),1));

            threw = true;
            attackTimer.restart();
        }
    }
}

void Entity::handleIdle() // when entity is idling
{

}

void Entity::handleFlee() // entity's flee
{

}

void Entity::handleLoot()
{
    if(!loot_sent)
    {
        switch(bh_loot)
        {
            case Behavior::Loot::BH_LOOT_DROP_INSTANT: {
                for(auto id : loot_table)
                    CoreManager::getInstance().getMissionController()->SendItemDrop(id.order_id, getGlobalPosition().x+getLocalPosition().x, 2160-600);
                break;
            }
            case Behavior::Loot::BH_LOOT_DROP_FADEAWAY: {
                for(auto id : loot_table)
                    CoreManager::getInstance().getMissionController()->SendItemDrop(id.order_id, getGlobalPosition().x+getLocalPosition().x, 2160-600);
                break;
            }
        }

        loot_sent = true;
    }
}

void Entity::Draw()
{
    float fps = CoreManager::getInstance().getCore()->getFPS();

    if(curHP <= 0)
    {
        handleDeath();
    }

    hPos += hspeed / fps;
    vPos += vspeed / fps;

    if(hspeed > 0)
        hspeed -= 400 / fps;
    if(hspeed < 0)
        hspeed += 400 / fps;

    vspeed += 2000 / fps;

    if(getLocalPosition().y >= 0)
        vspeed = 0;

    auto mc = CoreManager::getInstance().getMissionController();

    distanceToPlayer = 9999999;

    for(auto& pon : mc->yaripons)
    {
        distanceToPlayer = fmax(0, fmin(distanceToPlayer, abs( (pon->global_x+pon->local_x+pon->gap_x) - (global_x + local_x + hPos))));
    }

    handleApproach();
    handleDecisions();

    setGlobalPosition(sf::Vector2f(global_x + local_x, global_y + local_y + cam_offset));
    setLocalPosition(sf::Vector2f(local_x + hPos, local_y + vPos));

    AnimatedObject::Draw();

    if (toggleDebug)
    {
        auto strRepo = CoreManager::getInstance().getStrRepo();
        debugText.reset();
        debugText.append(std::format("{{size 36}}{{outline 2 255 255 255}}o{{n}}{}{{n}}curHP{{n}}{}{{n}}maxHP{{n}}{}", orderID, curHP, maxHP));
        debugText.setGlobalOrigin(debugText.getGlobalBounds().size.x / 2, debugText.getGlobalBounds().size.y);
        debugText.setGlobalPosition(global_x + local_x + hPos - 20, global_y + local_y + vPos + cam_offset - 100);
        debugText.draw();
    }

    if (toggleBehaviorDebug)
    {
        auto strRepo = CoreManager::getInstance().getStrRepo();
        debugText.reset();
        debugText.append(std::format("{{size 36}}{{outline 2 255 255 255}}anim{{n}}{}{{n}}frame{{n}}{}{{n}}hspeed{{n}}{}{{n}}vspeed{{n}}{}{{n}}hPos{{n}}{}{{n}}vPos{{n}}{}{{n}}action{{n}}{}{{n}}threw{{n}}{}{{n}}dtp{{n}}{}", getAnimation(), getAnimationFrame(), hspeed, vspeed, hPos, vPos, action, threw, distanceToPlayer));
        debugText.setGlobalOrigin(debugText.getGlobalBounds().size.x / 2, debugText.getGlobalBounds().size.y);
        debugText.setGlobalPosition(global_x + local_x + hPos - 20, global_y + local_y + vPos + cam_offset - 100);
        debugText.draw();
    }
}