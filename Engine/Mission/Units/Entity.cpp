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
        if(entity["behavior"].contains("hit"))
            bh_hit = behavior.convStringToHitEnum(entity["behavior"]["hit"]);
        if(entity["behavior"].contains("death"))
            bh_death = behavior.convStringToDeathEnum(entity["behavior"]["death"]);
        if(entity["behavior"].contains("spawn"))
            bh_spawn = behavior.convStringToSpawnEnum(entity["behavior"]["spawn"]);
        if(entity["behavior"].contains("loot"))
            bh_loot = behavior.convStringToLootEnum(entity["behavior"]["loot"]);
    }

    yPos = 1735;
    if(entity.contains("position"))
    {
        if(entity["position"].contains("y"))
            yPos = entity["position"]["y"].get<float>();
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

}

void Entity::handleCommand() // on player command inputted
{

}

// handlers - self
void Entity::handleDecisions() // what should the entity do?
{

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

    setLocalPosition(sf::Vector2f(getLocalPosition().x + hspeed / fps, getLocalPosition().y + vspeed / fps));

    if(hspeed > 0)
        hspeed -= 400 / fps;
    if(hspeed < 0)
        hspeed += 400 / fps;

    vspeed += 2000 / fps;

    if(getLocalPosition().y >= 0)
        vspeed = 0;

    AnimatedObject::Draw();

    if(toggleDebug)
    {
        auto strRepo = CoreManager::getInstance().getStrRepo();
        debugText.setFont(strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage()));
        debugText.setCharacterSize(12);
        debugText.setString(std::format("{{outline 2 255 255 255}}o{{n}}{}{{n}}curHP{{n}}{}{{n}}maxHP{{n}}{}", orderID, curHP, maxHP));
        debugText.setOrigin(debugText.getLocalBounds().width/2, debugText.getLocalBounds().height);
        debugText.setPosition(getGlobalPosition().x+getLocalPosition().x-20, getGlobalPosition().y+getLocalPosition().y-100);
        debugText.draw();
    }
}