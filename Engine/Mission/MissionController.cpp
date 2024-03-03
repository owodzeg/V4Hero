#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "MissionController.h"
#include "../Math/PVector.h"
#include "../Utils.h"
#include "Units/HitboxFrame.h"
#include "Units/Projectile.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <spdlog/spdlog.h>
#include <string>
#include <thread>
#include "../CoreManager.h"
#include "../StateManager.h"

using json = nlohmann::json;

using namespace std;

///move to Func::numDigits later
template<class T>
int numDigits(T number) //stolen from stackoverflow
{
    int digits = 0;
    if (number < 0)
        digits = 1;
    while (number)
    {
        number /= 10;
        digits++;
    }
    return digits;
}

MissionController::MissionController()
{
}

float MissionController::Smoothstep(float time) ///use time from 0.00 to 1.00
{
    time = Clamp(time, 0.0, 1.0);
    return time * time * (3 - 2 * time);
}

float MissionController::Clamp(float x, float lowerlimit, float upperlimit)
{
    if (x < lowerlimit)
        x = lowerlimit;
    if (x > upperlimit)
        x = upperlimit;

    return x;
}

int MissionController::layerStr2Enum(string layer)
{
    transform(layer.begin(), layer.end(), layer.begin(), [](unsigned char c) { return std::tolower(c); }); // https://stackoverflow.com/questions/313970
    if (layer == "buildings")
    {
        return BUILDINGS;
    } else if (layer == "nature")
    {
        return NATURE;
    } else if (layer == "animals")
    {
        return ANIMALS;
    } else if (layer == "ui")
    {
        return UI;
    }
}

void MissionController::addDmgCounter(int type, int damage, float baseX, float baseY, int q, int r)
{
    DamageCounter tmp;
    tmp.type = type;
    tmp.damage = damage;

    SPDLOG_DEBUG("MissionController::addDmgCounter({}, {}, {}, {})", type, damage, baseX, baseY);

    int digits = numDigits(damage);
    string sdigits = to_string(damage);

    SPDLOG_DEBUG("Digits: {} {}", digits, sdigits);

    int separator = 0;
    float init_scale = 1;
    float dg_scale = 4;

    if (damage < 100)
        type = 2;
    else
        type = 3;

    switch (type)
    {
        case 0: {
            separator = 12;
            init_scale = 0.8;
            dg_scale = 2.8;
            break;
        }

        case 1: {
            separator = 12;
            init_scale = 0.8;
            dg_scale = 2.8;
            break;
        }

        case 2: {
            separator = 16;
            init_scale = 0.8;
            dg_scale = 3.5;
            break;
        }

        case 3: {
            separator = 18;
            init_scale = 0.8;
            dg_scale = 3.9;
            break;
        }

        case 4: {
            separator = 20;
            init_scale = 0.8;
            dg_scale = 4.1;
            break;
        }
    }

    for (int i = 0; i < digits; i++)
    {
        string sdigit = string() + sdigits[i];
        int digit = atoi(sdigit.c_str());

        PSprite dg_spr;
        dg_spr.setTexture(dmg_spritesheet.t);
        dg_spr.setTextureRect(dmg_spritesheet.get_bounds((digit * 5) + type)); ///rect of the specific damage digit from spritesheet
        dg_spr.setOrigin(dg_spr.getLocalBounds().width / 2, dg_spr.getLocalBounds().height);
        dg_spr.qualitySetting = q;
        dg_spr.resSetting = r;

        sf::Vector2f dg_pos(baseX + (i * separator), baseY);

        tmp.spr.push_back(dg_spr);
        tmp.pos.push_back(dg_pos);
        tmp.scale.push_back(dg_scale);
        tmp.scale_goal.push_back(init_scale);
        tmp.mode.push_back(true);
        tmp.alpha.push_back(0);
    }

    SPDLOG_TRACE("Adding new dmgCounter to the vector... ");
    dmgCounters.push_back(tmp);
    SPDLOG_TRACE("Added new dmgCounter successfully");
}

void MissionController::addItemsCounter(int id, float baseX, float baseY)
{
}

json parseLootArray(mt19937& gen, uniform_real_distribution<double>& roll, json loot)
{
    SPDLOG_DEBUG("Parse loot array");

    if (!loot.is_array())
    {
        return loot;
    }

    int total = 0;
    for (int i = 1; i < loot.size(); i++) // Chances -> Intervals for second loop
    {
        SPDLOG_DEBUG("Loot loop #1 (i={})", i);
        if (loot[i].is_array())
        {
            loot[i][0] += total;
            total = loot[i][0];
        } else if (loot[i].is_object() && loot[i].size() == 2)
        {
            int tmp = loot[i]["chance"];
            loot[i].erase("chance");
            total += tmp;
            loot[i]["chance"] = total;
        } else
        {
			SPDLOG_WARN("Undefined behavior detected while parsing loot: {} | (Element of array is neither an array nor an object)", loot);
        }
    }

    if (total < 100)
    {
		SPDLOG_WARN("Undefined behavior detected while parsing loot: {} | (Total chances in array less than 100)", loot);
    } else if (total > 100)
    {
		SPDLOG_WARN("Undefined behavior detected while parsing loot: {} | (Total chances in array more than 100)", loot);
    }

    SPDLOG_DEBUG("Roll");
    float n = roll(gen);
    for (int i = 1; i < loot.size(); i++) // Because of the first loop, values are neatly sorted
    {
        SPDLOG_DEBUG("Loot loop #2 (i={})", i);
        if (loot[i].is_array())
        {
            SPDLOG_DEBUG("Is array");
            if (n <= float(loot[i][0]) / 100)
            {
                return parseLootArray(gen, roll, loot[i]);
                break;
            }
        } else if (loot[i].is_object() && loot[i].size() == 2)
        {
            SPDLOG_DEBUG("Is object and loot == 2");
            if (n <= float(loot[i]["chance"]) / 100)
            {
                return loot[i];
                break;
            }
        } else
        {
			SPDLOG_WARN("Undefined behavior detected while parsing loot: {} | (Element of array is neither an array nor an object)", loot);
        }
    }
}


void MissionController::parseEntityLoot(mt19937& gen, uniform_real_distribution<double>& roll, json loot, vector<Entity::Loot>& to_drop)
{
    SPDLOG_DEBUG("Start parsing entity loot");

    if (loot.is_array())
    {
        SPDLOG_DEBUG("Is array");
        if (roll(gen) <= float(loot[0]) / 100)
        {
            Entity::Loot tmp;
            json parsedArray = parseLootArray(gen, roll, loot);
            tmp.order_id = CoreManager::getInstance().getSaveReader()->itemReg.getItemByName(parsedArray["item"])->order_id;
            to_drop.push_back(tmp);
        }
    } else if (loot.is_object() && loot.size() > 1)
    {
        SPDLOG_DEBUG("Is object and loot > 1");
        try
        {
            for (auto it = loot.begin(); it != loot.end(); ++it) // Assume it's an object containing two items to be parsed
            {
                parseEntityLoot(gen, roll, it.value(), to_drop);
            }
        } catch (const exception& e)
        {
            SPDLOG_DEBUG("Exception");
            if (roll(gen) <= float(loot["chance"]) / 100) // Assume it's the below else if
            {
                Entity::Loot tmp;
                tmp.order_id = CoreManager::getInstance().getSaveReader()->itemReg.getItemByName(loot["item"])->order_id;
                to_drop.push_back(tmp);
            }
        }
    } else if (loot.is_object() && loot.size() == 2)
    {
        SPDLOG_DEBUG("Is object with loot == 2");
        if (roll(gen) <= float(loot["chance"]) / 100)
        {
            Entity::Loot tmp;
            tmp.order_id = CoreManager::getInstance().getSaveReader()->itemReg.getItemByName(loot["item"])->order_id;
            to_drop.push_back(tmp);
        }
    }
}

void MissionController::spawnEntity(int id, bool collidable, bool attackable, int xpos, int xrange, bool cloneable, float clone_delay, float spawnrate, float stat_mult, int mindmg, int maxdmg, int hp, float ypos, float baseY, sf::Color color, int layer, int parent, nlohmann::json loot, nlohmann::json additional_data)
{
    sf::Clock bm; ///benchmark for spawnEntity function

    uniform_real_distribution<double> roll(0.0, 1.0);

    int mission_level = CoreManager::getInstance().getSaveReader()->mission_levels[curMissionID];
    float mission_diff = 0.85 + mission_level * 0.15;

    ///need to somehow optimize this to not copy paste the same code over and over

    SPDLOG_INFO("Spawning a new entity: ID {} hp {} xpos {} xrange {} ypos {} spawnrate {} stat_mult {}", id, hp, xpos, xrange, ypos, spawnrate, stat_mult);

    bool spawn = false;

    if (roll(CoreManager::getInstance().getCore()->gen) <= spawnrate / 100)
    {
        spawn = true;
    }

    cout << "Additional data: " << additional_data << endl;

    for (int i = 0; i < additional_data.size(); i++)
    {
        SPDLOG_DEBUG("Checking additional data entry #{}", i);

        ///Force entity to spawn when mission is on a specific level
        if (additional_data.contains("forceSpawnOnLvl"))
        {
            SPDLOG_DEBUG("Force Spawn on Level");
            if (mission_level == additional_data["forceSpawnOnLvl"])
            {
                spawn = true;
            }
        }

        ///Force entity to spawn when specific item is not obtained
        if (additional_data.contains("forceSpawnIfNotObtained"))
        {
            SPDLOG_DEBUG("Force Spawn If Not Obtained");
            if (!CoreManager::getInstance().getSaveReader()->invData.checkItemObtainedByName(additional_data["forceSpawnIfNotObtained"]))
            {
                spawn = true;
            }
        }

        ///Cache item if force drop is called
        if (additional_data.contains("forceDropIfNotObtained"))
        {
            SPDLOG_DEBUG("Force Drop If Not Obtained");
            Item* itm = CoreManager::getInstance().getSaveReader()->itemReg.getItemByName(additional_data["forceDropIfNotObtained"][0]);
            
            PSprite ps_itm = ResourceManager::getInstance().getSprite("resources/graphics/item/textures/" + itm->spritesheet + "/" + Func::num_padding(itm->spritesheet_id, 4) + ".png");
            sf::Image i_itm = ps_itm.s.getTexture()->copyToImage();

            DroppableCache tmp;
            tmp.img = i_itm;
            tmp.cached = true;

            droppable_cache[itm->spritesheet][itm->spritesheet_id] = tmp;
        }
    }

    SPDLOG_INFO("Entity additional data: {}", additional_data);

    if (spawn)
    {
        switch (id)
        {
            case 0: {
                unique_ptr<EndFlag> entity = make_unique<EndFlag>();
                entity->setEntityID(id); ///id must be set before LoadConfig so loadAnim can get the right cache ID
                entity.get()->LoadConfig();
                if (additional_data.size() >= 1)
                {
                    entity.get()->parseAdditionalData(additional_data);
                }

                //if (!CoreManager::getInstance().getCore()->isCached[id])
                {
                    //CoreManager::getInstance().getCore()->cacheEntity(id, entity.get()->all_swaps_img, entity.get()->animation_spritesheet, entity.get()->objects);
                }

                ///To be replaced with param file
                entity.get()->entityType = Entity::EntityTypes::PASSIVE;
                entity.get()->entityCategory = Entity::EntityCategories::MISC;

                if (!cloneable)
                    tangibleLevelObjects.push_back(std::move(entity));

                break;
            }
            case 1: {
                unique_ptr<FeverWorm> entity = make_unique<FeverWorm>();
                entity->setEntityID(id); ///id must be set before LoadConfig so loadAnim can get the right cache ID
                entity.get()->LoadConfig();
                if (additional_data.size() >= 1)
                {
                    entity.get()->parseAdditionalData(additional_data);
                }

                //if (!CoreManager::getInstance().getCore()->isCached[id])
                {
                    //CoreManager::getInstance().getCore()->cacheEntity(id, entity.get()->all_swaps_img, entity.get()->animation_spritesheet, entity.get()->objects);
                }

                ///To be replaced with param file
                entity.get()->entityType = Entity::EntityTypes::PASSIVE;
                entity.get()->entityCategory = Entity::EntityCategories::MISC;

                if (!cloneable)
                    tangibleLevelObjects.push_back(std::move(entity));

                break;
            }
            case 2: {
                unique_ptr<Kacheek> entity = make_unique<Kacheek>();
                entity->setEntityID(id); ///id must be set before LoadConfig so loadAnim can get the right cache ID
                entity.get()->LoadConfig();
                if (additional_data.size() >= 1)
                {
                    entity.get()->parseAdditionalData(additional_data);
                }

                //if (!CoreManager::getInstance().getCore()->isCached[id])
                {
                    //CoreManager::getInstance().getCore()->cacheEntity(id, entity.get()->all_swaps_img, entity.get()->animation_spritesheet, entity.get()->objects);
                }

                ///To be replaced with param file
                entity.get()->entityType = Entity::EntityTypes::HOSTILE;
                entity.get()->entityCategory = Entity::EntityCategories::ANIMAL;

                if (!cloneable)
                    tangibleLevelObjects.push_back(std::move(entity));

                break;
            }
            case 3: {
                unique_ptr<Grass1> entity = make_unique<Grass1>();
                entity->setEntityID(id); ///id must be set before LoadConfig so loadAnim can get the right cache ID
                entity.get()->LoadConfig();
                if (additional_data.size() >= 1)
                {
                    entity.get()->parseAdditionalData(additional_data);
                }

                //if (!CoreManager::getInstance().getCore()->isCached[id])
                {
                    //CoreManager::getInstance().getCore()->cacheEntity(id, entity.get()->all_swaps_img, entity.get()->animation_spritesheet, entity.get()->objects);
                }

                ///To be replaced with param file
                entity.get()->entityType = Entity::EntityTypes::NEUTRAL;
                entity.get()->entityCategory = Entity::EntityCategories::NATURE;

                if (!cloneable)
                    tangibleLevelObjects.push_back(std::move(entity));

                break;
            }
            case 4: {
                unique_ptr<Grass2> entity = make_unique<Grass2>();
                entity->setEntityID(id); ///id must be set before LoadConfig so loadAnim can get the right cache ID
                entity.get()->LoadConfig();
                if (additional_data.size() >= 1)
                {
                    entity.get()->parseAdditionalData(additional_data);
                }

                //if (!CoreManager::getInstance().getCore()->isCached[id])
                {
                    //CoreManager::getInstance().getCore()->cacheEntity(id, entity.get()->all_swaps_img, entity.get()->animation_spritesheet, entity.get()->objects);
                }

                ///To be replaced with param file
                entity.get()->entityType = Entity::EntityTypes::NEUTRAL;
                entity.get()->entityCategory = Entity::EntityCategories::NATURE;

                if (!cloneable)
                    tangibleLevelObjects.push_back(std::move(entity));

                break;
            }
            case 5: {
                unique_ptr<DroppedItem> entity = make_unique<DroppedItem>();
                entity->setEntityID(id); ///id must be set before LoadConfig so loadAnim can get the right cache ID
                entity.get()->LoadConfig();

                //if (!CoreManager::getInstance().getCore()->isCached[id])
                {
                    //CoreManager::getInstance().getCore()->cacheEntity(id, entity.get()->all_swaps_img, entity.get()->animation_spritesheet, entity.get()->objects);
                }

                ///To be replaced with param file
                entity.get()->entityType = Entity::EntityTypes::NEUTRAL;
                entity.get()->entityCategory = Entity::EntityCategories::MISC;

                string item_group = additional_data["spritesheet"];
                int item_id = additional_data["spritesheet_id"];
                string picked_item = additional_data["picked_item"];

                entity->item_group = item_group;
                entity->item_id = item_id;
                entity->picked_item = picked_item;

                if (!cloneable)
                    tangibleLevelObjects.push_back(std::move(entity));

                break;
            }
            case 6: {
                unique_ptr<Kirajin_Yari_1> entity = make_unique<Kirajin_Yari_1>();
                entity->setEntityID(id); ///id must be set before LoadConfig so loadAnim can get the right cache ID
                entity.get()->LoadConfig();
                if (additional_data.size() >= 1)
                {
                    entity.get()->parseAdditionalData(additional_data);
                }

                //if (!CoreManager::getInstance().getCore()->isCached[id])
                {
                    //CoreManager::getInstance().getCore()->cacheEntity(id, entity.get()->all_swaps_img, entity.get()->animation_spritesheet, entity.get()->objects);
                }

                ///To be replaced with param file
                entity.get()->entityType = Entity::EntityTypes::HOSTILE;
                entity.get()->entityCategory = Entity::EntityCategories::ENEMYUNIT;

                if (!cloneable)
                    tangibleLevelObjects.push_back(std::move(entity));

                break;
            }
            case 7: {
                unique_ptr<TreasureChest> entity = make_unique<TreasureChest>();
                entity->setEntityID(id); ///id must be set before LoadConfig so loadAnim can get the right cache ID
                entity.get()->LoadConfig();
                if (additional_data.size() >= 1)
                {
                    entity.get()->parseAdditionalData(additional_data);
                }

                //if (!CoreManager::getInstance().getCore()->isCached[id])
                {
                    //CoreManager::getInstance().getCore()->cacheEntity(id, entity.get()->all_swaps_img, entity.get()->animation_spritesheet, entity.get()->objects);
                }

                ///To be replaced with param file
                entity.get()->entityType = Entity::EntityTypes::HOSTILE;
                entity.get()->entityCategory = Entity::EntityCategories::OBSTACLE_IRON;

                if (!cloneable)
                    tangibleLevelObjects.push_back(std::move(entity));

                break;
            }
            case 8: {
                unique_ptr<RockBig> entity = make_unique<RockBig>();
                entity->setEntityID(id); ///id must be set before LoadConfig so loadAnim can get the right cache ID
                entity.get()->LoadConfig();
                if (additional_data.size() >= 1)
                {
                    entity.get()->parseAdditionalData(additional_data);
                }

                //if (!CoreManager::getInstance().getCore()->isCached[id])
                {
                    //CoreManager::getInstance().getCore()->cacheEntity(id, entity.get()->all_swaps_img, entity.get()->animation_spritesheet, entity.get()->objects);
                }

                ///To be replaced with param file
                entity.get()->entityType = Entity::EntityTypes::HOSTILE;
                entity.get()->entityCategory = Entity::EntityCategories::OBSTACLE_ROCK;

                if (!cloneable)
                    tangibleLevelObjects.push_back(std::move(entity));

                break;
            }
            case 9: {
                unique_ptr<RockSmall> entity = make_unique<RockSmall>();
                entity->setEntityID(id); ///id must be set before LoadConfig so loadAnim can get the right cache ID
                entity.get()->LoadConfig();
                if (additional_data.size() >= 1)
                {
                    entity.get()->parseAdditionalData(additional_data);
                }

                //if (!CoreManager::getInstance().getCore()->isCached[id])
                {
                    //CoreManager::getInstance().getCore()->cacheEntity(id, entity.get()->all_swaps_img, entity.get()->animation_spritesheet, entity.get()->objects);
                }

                ///To be replaced with param file
                entity.get()->entityType = Entity::EntityTypes::HOSTILE;
                entity.get()->entityCategory = Entity::EntityCategories::OBSTACLE_ROCK;

                if (!cloneable)
                    tangibleLevelObjects.push_back(std::move(entity));

                break;
            }
            case 10: {
                unique_ptr<WoodenSpikes> entity = make_unique<WoodenSpikes>();
                entity->setEntityID(id); ///id must be set before LoadConfig so loadAnim can get the right cache ID
                entity.get()->LoadConfig();
                if (additional_data.size() >= 1)
                {
                    entity.get()->parseAdditionalData(additional_data);
                }

                //if (!CoreManager::getInstance().getCore()->isCached[id])
                {
                    //CoreManager::getInstance().getCore()->cacheEntity(id, entity.get()->all_swaps_img, entity.get()->animation_spritesheet, entity.get()->objects);
                }

                ///To be replaced with param file
                entity.get()->entityType = Entity::EntityTypes::HOSTILE;
                entity.get()->entityCategory = Entity::EntityCategories::OBSTACLE_WOOD;

                if (!cloneable)
                    tangibleLevelObjects.push_back(std::move(entity));

                break;
            }
            case 11: {
                unique_ptr<RockPile> entity = make_unique<RockPile>();
                entity->setEntityID(id); ///id must be set before LoadConfig so loadAnim can get the right cache ID
                entity.get()->LoadConfig();
                if (additional_data.size() >= 1)
                {
                    entity.get()->parseAdditionalData(additional_data);
                }

                //if (!CoreManager::getInstance().getCore()->isCached[id])
                {
                    //CoreManager::getInstance().getCore()->cacheEntity(id, entity.get()->all_swaps_img, entity.get()->animation_spritesheet, entity.get()->objects);
                }

                ///To be replaced with param file
                entity.get()->entityType = Entity::EntityTypes::HOSTILE;
                entity.get()->entityCategory = Entity::EntityCategories::OBSTACLE_ROCK;

                if (!cloneable)
                    tangibleLevelObjects.push_back(std::move(entity));

                break;
            }
            case 12: {
                unique_ptr<KirajinHut> entity = make_unique<KirajinHut>();
                entity->setEntityID(id); ///id must be set before LoadConfig so loadAnim can get the right cache ID
                entity.get()->LoadConfig();
                if (additional_data.size() >= 1)
                {
                    entity.get()->parseAdditionalData(additional_data);
                }

                //if (!CoreManager::getInstance().getCore()->isCached[id])
                {
                    //CoreManager::getInstance().getCore()->cacheEntity(id, entity.get()->all_swaps_img, entity.get()->animation_spritesheet, entity.get()->objects);
                }

                ///To be replaced with param file
                entity.get()->entityType = Entity::EntityTypes::HOSTILE;
                entity.get()->entityCategory = Entity::EntityCategories::BUILDING_REGULAR;

                if (!cloneable)
                    tangibleLevelObjects.push_back(std::move(entity));

                break;
            }
            case 13: {
                unique_ptr<KirajinGuardTower> entity = make_unique<KirajinGuardTower>();
                entity->setEntityID(id); ///id must be set before LoadConfig so loadAnim can get the right cache ID
                entity.get()->LoadConfig();
                if (additional_data.size() >= 1)
                {
                    entity.get()->parseAdditionalData(additional_data);
                }

                //if (!CoreManager::getInstance().getCore()->isCached[id])
                {
                    //CoreManager::getInstance().getCore()->cacheEntity(id, entity.get()->all_swaps_img, entity.get()->animation_spritesheet, entity.get()->objects);
                }

                ///To be replaced with param file
                entity.get()->entityType = Entity::EntityTypes::HOSTILE;
                entity.get()->entityCategory = Entity::EntityCategories::BUILDING_REGULAR;

                if (!cloneable)
                    tangibleLevelObjects.push_back(std::move(entity));

                break;
            }
            case 14: {
                unique_ptr<KirajinPoweredTowerSmall> entity = make_unique<KirajinPoweredTowerSmall>();
                entity->setEntityID(id); ///id must be set before LoadConfig so loadAnim can get the right cache ID
                entity.get()->LoadConfig();
                if (additional_data.size() >= 1)
                {
                    entity.get()->parseAdditionalData(additional_data);
                }

                //if (!CoreManager::getInstance().getCore()->isCached[id])
                {
                    //CoreManager::getInstance().getCore()->cacheEntity(id, entity.get()->all_swaps_img, entity.get()->animation_spritesheet, entity.get()->objects);
                }

                ///To be replaced with param file
                entity.get()->entityType = Entity::EntityTypes::HOSTILE;
                entity.get()->entityCategory = Entity::EntityCategories::BUILDING_IRON;

                if (!cloneable)
                    tangibleLevelObjects.push_back(std::move(entity));

                break;
            }
            case 15: {
                unique_ptr<KirajinPoweredTowerBig> entity = make_unique<KirajinPoweredTowerBig>();
                entity->setEntityID(id); ///id must be set before LoadConfig so loadAnim can get the right cache ID
                entity.get()->LoadConfig();
                if (additional_data.size() >= 1)
                {
                    entity.get()->parseAdditionalData(additional_data);
                }

                //if (!CoreManager::getInstance().getCore()->isCached[id])
                {
                    //CoreManager::getInstance().getCore()->cacheEntity(id, entity.get()->all_swaps_img, entity.get()->animation_spritesheet, entity.get()->objects);
                }

                ///To be replaced with param file
                entity.get()->entityType = Entity::EntityTypes::HOSTILE;
                entity.get()->entityCategory = Entity::EntityCategories::BUILDING_IRON;

                if (!cloneable)
                    tangibleLevelObjects.push_back(std::move(entity));

                break;
            }
            case 16: {
                unique_ptr<Kirajin_Yari_2> entity = make_unique<Kirajin_Yari_2>();
                entity->setEntityID(id); ///id must be set before LoadConfig so loadAnim can get the right cache ID
                entity.get()->LoadConfig();
                if (additional_data.size() >= 1)
                {
                    entity.get()->parseAdditionalData(additional_data);
                }

                //if (!CoreManager::getInstance().getCore()->isCached[id])
                {
                    //CoreManager::getInstance().getCore()->cacheEntity(id, entity.get()->all_swaps_img, entity.get()->animation_spritesheet, entity.get()->objects);
                }

                ///To be replaced with param file
                entity.get()->entityType = Entity::EntityTypes::HOSTILE;
                entity.get()->entityCategory = Entity::EntityCategories::ENEMYUNIT;

                if (!cloneable)
                    tangibleLevelObjects.push_back(std::move(entity));
                else
                    kirajins.push_back(std::move(entity));

                break;
            }
        }

        Entity* entity = new Entity;

        if (!cloneable)
        {
            entity = tangibleLevelObjects[tangibleLevelObjects.size() - 1].get();
        } else
        {
            if (id == 16)
                entity = kirajins[kirajins.size() - 1].get();
        }

        entity->spawnOrderID = spawnOrder;
        spawnOrder++;

        if (id != 5) ///ID 5 = dropped item, it has an exclusive loading type
        {
            entity->setEntityID(id);

            entity->floorY = baseY; ///in case Y gets overriden, save the position where the floor is
			baseY = ypos;

            if (xrange != 0)
                entity->setGlobalPosition(sf::Vector2f(xpos + (rand() % xrange), ypos));
            else
                entity->setGlobalPosition(sf::Vector2f(xpos, ypos));

            entity->setColor(color);

            entity->cloneable = cloneable;
            entity->respawnTime = clone_delay;

            entity->spawn_x = entity->getGlobalPosition().x;

            entity->isCollidable = collidable;
            entity->isAttackable = attackable;

            vector<Entity::Loot> new_loot;
            parseEntityLoot(CoreManager::getInstance().getCore()->gen, roll, loot, new_loot);

            entity->loot_table = new_loot;

            /// hacky code to obtain sf::Image for item cache
            for (int i = 0; i < new_loot.size(); i++)
            {
                vector<int> loot_id = new_loot[i].order_id;
                Item* itm = CoreManager::getInstance().getSaveReader()->itemReg.getItemByID(loot_id);

                //ps_itm.loadFromFile("resources/graphics/item/textures/" + itm->spritesheet + "/" + Func::num_padding(itm->spritesheet_id, 4) + ".png", qualitySetting, resSetting);
                
                PSprite ps_itm = ResourceManager::getInstance().getSprite("resources/graphics/item/textures/" + itm->spritesheet + "/" + Func::num_padding(itm->spritesheet_id, 4) + ".png");
                sf::Image i_itm = ps_itm.s.getTexture()->copyToImage();

                DroppableCache tmp;
                tmp.img = i_itm;
                tmp.cached = true;

                droppable_cache[itm->spritesheet][itm->spritesheet_id] = tmp;
            }

            entity->curHP = hp * mission_diff;
            entity->maxHP = hp * mission_diff;

            if (!entity->custom_dmg)
            {
                entity->mindmg = mindmg;
                entity->maxdmg = maxdmg;
            }

            entity->stat_multiplier = 1 + ((mission_diff - 1) * 0.333);

            if (additional_data.contains("hidden"))
            {
                entity->layer = -layer;
            } else
            {
                entity->layer = layer;
            }

            entity->parent = parent;
            entity->additional_data = additional_data;
        } else
        {
            entity->setEntityID(id);

            ///This unique entity needs to be loaded differently, read additional data for spritesheet info to be passed from the item registry.
            string item_group = additional_data["spritesheet"];
            int item_id = additional_data["spritesheet_id"];

            /// create a fake animation
            std::string img_key = "droppeditem_" + item_group + "_" + to_string(item_id) + "_";

            for (int fr = 0; fr <= 3; fr++)
            {
                TextureManager::getInstance().loadImageFromMemory(img_key + to_string(fr), droppable_cache[item_group][item_id].img);
            }

            entity->custom_img_key = img_key;

            entity->qualitySetting = qualitySetting;

            entity->setGlobalPosition(sf::Vector2f(xpos, ypos));

            entity->setColor(color);

            entity->isCollidable = collidable;
            entity->isAttackable = attackable;

            entity->layer = 9999;
            entity->parent = -1;
        }
    }

    SPDLOG_INFO("Loading finished. Loading took {} seconds", bm.getElapsedTime().asSeconds());
}

void MissionController::spawnProjectile(PSprite& sprite, float xPos, float yPos, float speed, float hspeed, float vspeed, float angle, float max_dmg, float min_dmg, float crit, bool enemy)
{
    unique_ptr<Spear> p = make_unique<Spear>(sprite);

    p.get()->xPos = xPos;
    p.get()->yPos = yPos;
    p.get()->speed = speed;
    p.get()->hspeed = hspeed;
    p.get()->vspeed = vspeed;
    p.get()->angle = angle;
    p.get()->max_dmg = max_dmg;
    p.get()->min_dmg = min_dmg;
    p.get()->crit = crit;
    p.get()->enemy = enemy;

    levelProjectiles.push_back(std::move(p));
}

void MissionController::addPickedItem(std::string spritesheet, int spritesheet_id, std::string picked_item)
{
    SPDLOG_INFO("Adding new PickedItem instance: spritesheet {} spritesheet_id {} picked_item {}", spritesheet, spritesheet_id, picked_item);

    if (picked_item != "potion_1" && picked_item != "potion_2") ///Check for potions
    {
        PickedItem tmp;
        tmp.circle.setFillColor(sf::Color(255, 255, 255, 192));
        //tmp.circle.setRadius(50 * resRatioX);
        ///set radius in draw loop to get appropriate resratiox size
        tmp.item_name = picked_item;

        Item* itm = CoreManager::getInstance().getSaveReader()->itemReg.getItemByName(tmp.item_name);

        ///This unique entity needs to be loaded differently, read additional data for spritesheet info to be passed from the item registry.
        //vector<char> di_data = droppeditem_spritesheet[spritesheet].retrieve_char();

        sf::Texture tex_obj;
        tex_obj.loadFromImage(droppable_cache[itm->spritesheet][itm->spritesheet_id].img);
        tex_obj.setSmooth(true);

        tmp.item.setTexture(tex_obj);
        //tmp.item.setTextureRect(droppeditem_spritesheet[spritesheet].retrieve_rect_as_map()[spritesheet_id - 1]);
        //tmp.bounds = sf::Vector2f(droppeditem_spritesheet[spritesheet].retrieve_rect_as_map()[spritesheet_id - 1].width, droppeditem_spritesheet[spritesheet].retrieve_rect_as_map()[spritesheet_id - 1].height);

        tmp.bounds = sf::Vector2f(droppable_cache[itm->spritesheet][itm->spritesheet_id].img.getSize().x, droppable_cache[itm->spritesheet][itm->spritesheet_id].img.getSize().y);

        tmp.item.qualitySetting = qualitySetting;
        tmp.item.resSetting = resSetting;

        pickedItems.push_back(tmp);
    } else
    {
        float heal_factor = 0;

        if (picked_item == "potion_1")
            heal_factor = 0.2;
        if (picked_item == "potion_2")
            heal_factor = 0.5;

        for (int i = 0; i < units.size(); i++)
        {
            PlayableUnit* unit = units[i].get();

            unit->current_hp += unit->max_hp * heal_factor;

            if (unit->current_hp >= unit->max_hp)
                unit->current_hp = unit->max_hp;

            SPDLOG_DEBUG("Healed unit {} hp by {}", i, unit->max_hp * heal_factor);
        }
    }
}

void MissionController::submitPickedItems()
{
    for (int i = 0; i < pickedItems.size(); i++)
    {
        InventoryData::InventoryItem invItem;
        CoreManager::getInstance().getSaveReader()->invData.addItem(CoreManager::getInstance().getSaveReader()->itemReg.getItemByName(pickedItems[i].item_name)->order_id, CoreManager::getInstance().getSaveReader()->itemReg);
        if (pickedItems[i].item_name == "item_soggy_map") ///Grubby map
        {
            ///Check if Patapine Grove missions doesnt exist, and if Patapine Grove is not unlocked already
            if (!CoreManager::getInstance().getSaveReader()->isMissionUnlocked(3) && !CoreManager::getInstance().getSaveReader()->isMissionUnlocked(2) && !CoreManager::getInstance().getSaveReader()->isLocationUnlocked(2))
            {
                ///Add first patapine mission and unlock second location
                CoreManager::getInstance().getSaveReader()->missions_unlocked.push_back(2);
                CoreManager::getInstance().getSaveReader()->locations_unlocked.push_back(2);
            }
        }

        if (pickedItems[i].item_name == "item_digital_blueprint")
        {
            ///Check if Ejiji Cliffs missions doesnt exist, and if Ejiji Cliffs is not unlocked already
            if (!CoreManager::getInstance().getSaveReader()->isMissionUnlocked(5) && !CoreManager::getInstance().getSaveReader()->isMissionUnlocked(4) && !CoreManager::getInstance().getSaveReader()->isLocationUnlocked(3))
            {
                ///Add first patapine mission and unlock second location
                CoreManager::getInstance().getSaveReader()->missions_unlocked.push_back(4);
                CoreManager::getInstance().getSaveReader()->locations_unlocked.push_back(3);
            }
        }
    }
}

void MissionController::updateMissions()
{
    ///When this function is called, the mission has been completed successfully
    SPDLOG_DEBUG("Update mission unlocks after completing mission ID {}", curMissionID);

    switch (curMissionID)
    {
        case 2: {
            if (!CoreManager::getInstance().getSaveReader()->isMissionUnlocked(3))
            {
                CoreManager::getInstance().getSaveReader()->missions_unlocked.push_back(3);
                CoreManager::getInstance().getSaveReader()->mission_levels[3] = 2;

                auto it = std::find(CoreManager::getInstance().getSaveReader()->missions_unlocked.begin(), CoreManager::getInstance().getSaveReader()->missions_unlocked.end(), 2);
                CoreManager::getInstance().getSaveReader()->missions_unlocked.erase(it);
            }

            break;
        }

        case 4: {
            if (!CoreManager::getInstance().getSaveReader()->isMissionUnlocked(5))
            {
                CoreManager::getInstance().getSaveReader()->missions_unlocked.push_back(5);
                CoreManager::getInstance().getSaveReader()->mission_levels[5] = 2;

                auto it = std::find(CoreManager::getInstance().getSaveReader()->missions_unlocked.begin(), CoreManager::getInstance().getSaveReader()->missions_unlocked.end(), 4);
                CoreManager::getInstance().getSaveReader()->missions_unlocked.erase(it);
            }

            break;
        }
    }

    if (CoreManager::getInstance().getSaveReader()->mission_levels[curMissionID] != 0)
        CoreManager::getInstance().getSaveReader()->mission_levels[curMissionID] += 1;
}

void MissionController::addUnitThumb(int unit_class)
{
    UnitThumb tmp;
    tmp.unit_class = unit_class;
    tmp.hpbar_back.loadFromFile("resources/graphics/mission/hpbar_back.png", qualitySetting, 1);
    tmp.hpbar_ins.loadFromFile("resources/graphics/mission/hpbar_ins.png", qualitySetting, 1);
    tmp.unit_count.createText(f_font, 26, sf::Color::White, "", qualitySetting, 1);
    tmp.unit_count_shadow.createText(f_font, 26, sf::Color::Black, "", qualitySetting, 1);
    tmp.width = tmp.hpbar_ins.getLocalBounds().width;
    unitThumbs.push_back(tmp);
}

void MissionController::StartMission(std::string missionFile, bool showCutscene, int missionID, float mission_multiplier)
{
    int q = CoreManager::getInstance().getConfig()->GetInt("textureQuality");
    qualitySetting = q;
    resSetting = 1;

    dmg_spritesheet.load("resources/graphics/mission/damagesheet.png", q, 1);

    missionEnd = false;
    failure = false;

    //ctor
    f_font.loadFromFile(CoreManager::getInstance().getConfig()->fontPath);

    if (CoreManager::getInstance().getConfig()->fontPath == "resources/fonts/p4kakupop-pro.ttf")
        f_moji.loadFromFile("resources/fonts/mojipon.otf");
    else
        f_moji.loadFromFile(CoreManager::getInstance().getConfig()->fontPath);

    f_unicode.loadFromFile("resources/fonts/p4kakupop-pro.ttf");

    //f_font.loadFromFile("resources/fonts/arial.ttf");
    t_timerMenu.setFont(f_font);
    t_timerMenu.setCharacterSize(38);
    t_timerMenu.setFillColor(sf::Color::White);
    //f_font.loadFromFile("resources/fonts/arial.ttf");
    //t_cutscene_text.setFont(f_font);

    //t_cutscene_text.setCharacterSize(35);
    //t_cutscene_text.setFillColor(sf::Color::White);
    //t_cutscene_text.setString(Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("intro_cutscene_1")));
    //t_cutscene_text.setOrigin(t_cutscene_text.getGlobalBounds().width/2,t_cutscene_text.getGlobalBounds().height/2);

    std::vector<std::string> commands;
    commands.push_back("patapata");
    commands.push_back("ponpon");
    commands.push_back("chakachaka");
    commands.push_back("ponchaka");
    commands.push_back("ponpata");
    commands.push_back("donchaka");
    commands.push_back("party");
    commands.push_back("dondon");
    commands.push_back("chakapata");
    command_padding = 720 * 0.2; ///Like 2% screen width or smth idk

    command_descs.clear();
    command_inputs.clear();

    std::vector<std::string> command_lang_keys = {"nav_onward", "nav_attack", "nav_defend", "nav_charge", "nav_retreat", "nav_jump", "nav_party", "nav_summon"};
    std::vector<std::string> command_lang_buttons = {"□-□-□-〇", "〇-〇-□-〇", "△-△-□-〇", "〇-〇-△-△", "〇-□-〇-□", "×-×-△-△", "□-〇-×-△", "×-××-××"};

    ///first four
    for (int i = 0; i < 4; i++)
    {

        PText t_command_desc;
        t_command_desc.createText(f_font, 28, sf::Color(128, 128, 128, 255), Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString(command_lang_keys[i])) + ":", qualitySetting, 1);
        command_descs.push_back(t_command_desc);

        PText t_command;
        t_command.createText(f_unicode, 28, sf::Color(128, 128, 128, 255), Func::ConvertToUtf8String(command_lang_buttons[i]), qualitySetting, 1);
        command_inputs.push_back(t_command);
    }

    ///second four
    for (int i = 4; i < 8; i++)
    {

        PText t_command_desc;
        t_command_desc.createText(f_font, 28, sf::Color(128, 128, 128, 255), Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString(command_lang_keys[i])) + ":", qualitySetting, 1);
        command_descs.push_back(t_command_desc);

        PText t_command;
        t_command.createText(f_unicode, 28, sf::Color(128, 128, 128, 255), Func::ConvertToUtf8String(command_lang_buttons[i]), qualitySetting, 1);
        command_inputs.push_back(t_command);
    }

    // this is outside the loop
    startAlpha = 255;
    endAlpha = 0;
    targetTime = sf::seconds(10);


    fade.setPosition(sf::Vector2f(0, 0));
    fade.setFillColor(sf::Color(0, 0, 0, 0));
    fade.setSize(sf::Vector2f(800, 600));
    currentCutsceneId = 0;

    sb_win_jingle.loadFromFile("resources/sfx/level/victory.ogg");
    sb_lose_jingle.loadFromFile("resources/sfx/level/failure.ogg");

    sb_cheer1.loadFromFile("resources/sfx/level/cheer1.ogg");
    sb_cheer2.loadFromFile("resources/sfx/level/cheer2.ogg");
    sb_cheer3.loadFromFile("resources/sfx/level/cheer1.ogg");

    t_win.createText(f_moji, 56, sf::Color(222, 83, 0, 255), Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("mission_complete")), q, 1);
    t_win_outline.createText(f_moji, 56, sf::Color(255, 171, 0, 255), Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("mission_complete")), q, 1);
    t_win_outline.setOutlineColor(sf::Color(255, 171, 0, 255));
    t_win_outline.setOutlineThickness(10);
    t_lose.createText(f_moji, 56, sf::Color(138, 15, 26, 255), Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("mission_failed")), q, 1);
    t_lose_outline.createText(f_moji, 56, sf::Color(254, 48, 55, 255), Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("mission_failed")), q, 1);
    t_lose_outline.setOutlineColor(sf::Color(254, 48, 55, 255));
    t_lose_outline.setOutlineThickness(10);

    t_win.setOrigin(t_win.getLocalBounds().width / 2, t_win.getLocalBounds().height / 2);
    t_win_outline.setOrigin(t_win_outline.getLocalBounds().width / 2, t_win_outline.getLocalBounds().height / 2);

    t_lose.setOrigin(t_lose.getLocalBounds().width / 2, t_lose.getLocalBounds().height / 2);
    t_lose_outline.setOrigin(t_lose_outline.getLocalBounds().width / 2, t_lose_outline.getLocalBounds().height / 2);

    bar_win.loadFromFile("resources/graphics/mission/bar_win.png", q, 1);
    bar_lose.loadFromFile("resources/graphics/mission/bar_lose.png", q, 1);

    bar_win.setOrigin(bar_win.getLocalBounds().width / 2, bar_win.getLocalBounds().height / 2);
    bar_lose.setOrigin(bar_lose.getLocalBounds().width / 2, bar_lose.getLocalBounds().height / 2);

    ifstream sprdata("resources/graphics/item/itemdata/item_spritesheets.dat");
    string buff = "";

    while (getline(sprdata, buff))
    {
        if (buff.find("#") == std::string::npos)
        {
            ///Valid spritesheet. Load it
            SPDLOG_DEBUG("Loading item spritesheet resources/graphics/item/itemdata/{}.png", buff);
            droppeditem_spritesheet[buff].load("resources/graphics/item/itemdata/" + buff + ".png", qualitySetting, resSetting);
        }
    }
    ctrlTips.create(110, f_font, 28, "Onward: □-□-□-O	   Attack: O-O-□-O		Defend: △-△-□-O			  Charge: O-O-△-△\nRetreat: O-□-O-□		  Jump: X-X-△-△		  Party: □-O-X-△		  Summon: X-XX-XX", q, sf::Color(128, 128, 128, 255));

    spear_hit_enemy.loadFromFile("resources/sfx/level/spear_hit_enemy.ogg");
    spear_hit_iron.loadFromFile("resources/sfx/level/spear_hit_iron.ogg");
    spear_hit_rock.loadFromFile("resources/sfx/level/spear_hit_rock.ogg");
    spear_hit_solid.loadFromFile("resources/sfx/level/spear_hit_solid.ogg");
    s_heal.loadFromFile("resources/sfx/level/picked_heal.ogg");

    SPDLOG_INFO("Mission initialization finished");

    SPDLOG_INFO("Starting mission");

    curMissionID = missionID;

    fade_alpha = 255;
    missionEnd = false;
    playJingle = false;
    textBounce = false;
    textCurX = -1280;
    barCurX = 1920;
    textDestX = 640;
    barDestX = 640;
    textCurScale = 1;
    textDestScale = 1;
    fade_alpha = 255;
    fadeout_alpha = 0;
    playCheer[0] = false;
    playCheer[1] = false;
    playCheer[2] = false;
    spawnOrder = 0;


    //sf::Context context;
    int quality = CoreManager::getInstance().getConfig()->GetInt("textureQuality");
    float ratioX, ratioY;

    army_x = 0;
    camera.camera_x = 480;
    camera.zoom_point_y = CoreManager::getInstance().getWindow()->getSize().y - 110;
    showTimer = false;

    switch (quality)
    {
        case 0: ///low
        {
            ratioX = CoreManager::getInstance().getConfig()->GetInt("resX") / float(640);
            ratioY = CoreManager::getInstance().getConfig()->GetInt("resY") / float(360);
            break;
        }

        case 1: ///med
        {
            ratioX = CoreManager::getInstance().getConfig()->GetInt("resX") / float(1280);
            ratioY = CoreManager::getInstance().getConfig()->GetInt("resY") / float(720);
            break;
        }

        case 2: ///high
        {
            ratioX = CoreManager::getInstance().getConfig()->GetInt("resX") / float(1920);
            ratioY = CoreManager::getInstance().getConfig()->GetInt("resY") / float(1080);
            break;
        }

        case 3: ///ultra
        {
            ratioX = CoreManager::getInstance().getConfig()->GetInt("resX") / float(3840);
            ratioY = CoreManager::getInstance().getConfig()->GetInt("resY") / float(2160);
            break;
        }
    }

    patapon_y = 720 - 141;
    floor_y = 720 - 100;

    /**
	if(showCutscene)
	{
		cutscene_text_identifiers.push_back("intro_cutscene_1");
		cutscene_text_identifiers.push_back("intro_cutscene_2");
		cutscene_text_identifiers.push_back("intro_cutscene_3");
		cutscene_text_identifiers.push_back("intro_cutscene_4");
		cutscene_text_identifiers.push_back("intro_cutscene_5");
		cutscene_lengths.push_back(4);
		cutscene_lengths.push_back(4);
		cutscene_lengths.push_back(3);
		cutscene_lengths.push_back(3);
		cutscene_lengths.push_back(2);
		cutscene_blackscreens.push_back(true);
		cutscene_blackscreens.push_back(true);
		cutscene_blackscreens.push_back(true);
		cutscene_blackscreens.push_back(true);
		cutscene_blackscreens.push_back(true);
		currentCutsceneId=0;
		cutscenesLeft=true;
		isFinishedLoading=true;

	}
	else
	{
		inCutscene = false;
		cutscene_blackscreens.clear();
		cutscene_lengths.clear();
		cutscene_text_identifiers.clear();
		cutscenesLeft=false;
	}*/

    tangibleLevelObjects.clear();
    levelProjectiles.clear();

    string bgName;       ///background
    string songName;     ///bgm
    string missionName;  ///rpc_name
    string missionImg;   ///rpc_img
    string forceWeather; // Force Weather

    ifstream elist("resources/units/entitylist.dat");

    vector<string> entity_list;

    while (getline(elist, buff))
    {
        if (buff.back() == '\r')
        {
            buff.pop_back();
        }

        if (buff[0] != '#')
        {
            if (!buff.empty())
            {
                entity_list.push_back(buff.substr(buff.find_last_of(',') + 1));
            }
        }
    }

    elist.close();

    ifstream mf("resources/missions/" + missionFile);
    SPDLOG_INFO("Attempting to read a mission resources/missions/{}", missionFile);

    bool accepted = false;
    float ver = 0.0;

    json mjson;
    mf >> mjson;

    mf.close();

    try
    {
        json mission_data = mjson["params"];

        missionName = mission_data["name"];
        bgName = mission_data["background"];
        songName = mission_data["bgm"];
        missionImg = mission_data["rich_presence"];
        if (mission_data.contains("force_weather"))
        {
            forceWeather = mission_data["force_weather"];
        } else
        {
            forceWeather = "";
        }
    } catch (const exception& e)
    {
        SPDLOG_ERROR("[ERROR] An error occured while loading mission: resources/missions/{}. Error: {}", missionFile, e.what());
        // Somehow return to obelisk
        return;
    }

    try
    {
        json mission_cutscenes = mjson["cutscenes"];

        //for(int i = 0; i < mission_cutscenes.size(); i++)
        //{
        //	Cutscene temp;
        //	temp.load(mission_cutscenes);
        //	cutscenes.push_back(temp);
        //}
    } catch (const exception& e)
    {
        // Continue regardless of error,  cutscenes might not occur
    }

    try
    {
        json mission_entities = mjson["entities"];

        int layer_idx_buildings = 0;
        int layer_idx_nature = 0;
        int layer_idx_animals = 0;

        for (int i = 0; i < mission_entities.size(); i++)
        {
            json cur_entity = mission_entities[i];

            int id;
            bool collidable;
            bool attackable;

            if (cur_entity.contains("id"))
            {
                id = cur_entity["id"];
            } else
            {
                id = 2; // Kacheek by default
            }

            try
            {
                ifstream ent_default_params("resources/units/entity/" + entity_list[id] + ".p4p");
                json ent_defaults;
                ent_default_params >> ent_defaults;

                try
                {
                    json ent_params = cur_entity["params"];

                    float xpos;
                    float xrange;
                    bool cloneable;
                    float clone_delay;
                    float spawnrate;
                    float stat_mult;
                    int mindmg;
                    int maxdmg;
                    int hp;
                    float ypos;
                    float baseY;
                    int alpha;
                    json loot;
                    int layer;
                    int parent;
                    sf::Color* color;
                    json ent_custom_params; // update spawnEntity to handle json for this instead

                    if (ent_params.contains("xpos"))
                    {
                        xpos = ent_params["xpos"];
                    } else
                    {
                        xpos = 1000;
                    }
                    if (ent_params.contains("xrange"))
                    {
                        xrange = ent_params["xrange"];
                    } else
                    {
                        xrange = 0;
                    }
                    if (ent_params.contains("cloneable"))
                    {
                        cloneable = ent_params["cloneable"];
                    } else
                    {
                        cloneable = false;
                    }
                    if (ent_params.contains("clone_delay"))
                    {
                        clone_delay = ent_params["clone_delay"];
                    } else
                    {
                        clone_delay = 10;
                    }
                    if (ent_params.contains("spawnrate"))
                    {
                        spawnrate = ent_params["spawnrate"];
                    } else
                    {
                        spawnrate = 100;
                    }
                    if (ent_params.contains("stat_mult"))
                    {
                        stat_mult = ent_params["stat_mult"];
                    } else
                    {
                        stat_mult = 1;
                    }

                    if (ent_params.contains("mindmg"))
                    {
                        mindmg = ent_params["mindmg"];
                    } else
                    {
                        if (ent_defaults.contains("mindmg"))
                        {
                            mindmg = ent_defaults["mindmg"];
                        } else
                        {
                            mindmg = 1;
                        }
                    }
                    if (ent_params.contains("maxdmg"))
                    {
                        maxdmg = ent_params["maxdmg"];
                    } else
                    {
                        if (ent_defaults.contains("maxdmg"))
                        {
                            maxdmg = ent_defaults["maxdmg"];
                        } else
                        {
                            maxdmg = 1;
                        }
                    }

                    if (ent_params.contains("hp"))
                    {
                        hp = ent_params["hp"];
                    } else
                    {
                        hp = ent_defaults["baseHP"];
                    }
                    if (ent_params.contains("ypos"))
                    {
                        ypos = ent_params["ypos"];
                    } else
                    {
                        ypos = ent_defaults["baseY"];
                    }
                    if (ent_defaults.contains("baseY"))
                    {
                        baseY = ent_defaults["baseY"];
                    } else
                    {
                        baseY = patapon_y;
                    };
                    if (ent_params.contains("alpha"))
                    {
                        alpha = ent_params["alpha"];
                    } else
                    {
                        alpha = 255;
                    }
                    if (ent_params.contains("color"))
                    {
                        string clr = ent_params["color"];
                        int r = stoi(clr.substr(0, 2), 0, 16);
                        int g = stoi(clr.substr(2, 2), 0, 16);
                        int b = stoi(clr.substr(4, 2), 0, 16);
                        color = new sf::Color(r, g, b, alpha);
                    } else
                    {
                        color = new sf::Color(255, 255, 255, 255);
                    }
                    if (ent_params.contains("layer"))
                    {
                        switch (layerStr2Enum(ent_params["layer"]))
                        {
                            case BUILDINGS: {
                                if (layer_idx_buildings >= 50) // 0-49
                                {
                                    layer_idx_buildings = 0;
                                }
                                layer = BUILDINGS + layer_idx_buildings;
                                layer_idx_buildings++;
                                break;
                            }
                            case NATURE: {
                                if (layer_idx_nature >= 100) // 50-149
                                {
                                    layer_idx_nature = 0;
                                }
                                layer = NATURE + layer_idx_nature;
                                layer_idx_nature++;
                                break;
                            }
                            default:
                            case ANIMALS: {
                                if (layer_idx_animals >= 500) // 150-649
                                {
                                    layer_idx_animals = 0;
                                }
                                layer = ANIMALS + layer_idx_animals;
                                layer_idx_animals++;
                                break;
                            }
                            case UI: {
                                layer = 9999;
                                break;
                            }
                        }
                    } else
                    {
                        if (layer_idx_animals >= 500) // 150-649
                        {
                            layer_idx_animals = 0;
                        }
                        layer = ANIMALS + layer_idx_animals;
                        layer_idx_animals++;
                    }

                    if (ent_params.contains("parent"))
                    {
                        parent = ent_params["parent"];
                    } else
                    {
                        parent = -1;
                    }

                    if (ent_params.contains("loot"))
                    {
                        loot = ent_params["loot"];
                    } else
                    {
                        loot = ent_defaults["loot"];
                    }

                    collidable = ent_defaults["collidable"];
                    attackable = ent_defaults["attackable"];

                    if (ent_params.contains("custom_params"))
                    {
                        ent_custom_params = ent_params["custom_params"];
                    } else
                    {
                        // Do nothing
                    }

                    spawnEntity(id, collidable, attackable, xpos, xrange, cloneable, clone_delay, spawnrate, stat_mult, mindmg, maxdmg, hp, ypos, baseY, *color, layer, parent, loot, ent_custom_params);
                } catch (const exception& e)
                {
					SPDLOG_ERROR("An error occured while loading mission entity params from: resources/missions/{}. Error: {}", missionFile, e.what());
                }

                ent_default_params.close();
            } catch (const exception& e)
            {
				SPDLOG_ERROR("An error occured while loading default entity params from: resources/units/entity/{}. Error: {}", entity_list[id], e.what());
            }
        }
    } catch (const exception& e)
    {
		SPDLOG_ERROR("An error occured while loading mission entities from: resources/missions/{}. Error: {}", missionFile, e.what());
    }

    ///make this unit load based on how the army is built later
    int army_size = CoreManager::getInstance().getSaveReader()->ponReg.pons.size();

    // TO-DO: fix animated object to work with new system
    unique_ptr<Hatapon> wip_hatapon = make_unique<Hatapon>(); ///Hatapon's a special snowflake and isn't in the saveReader.ponreg.pons vector lol
    wip_hatapon.get()->LoadConfig();
    wip_hatapon.get()->setUnitID(0);
    units.push_back(std::move(wip_hatapon));
    //TO-DO: addUnitThumb(0);

    for (int i = 0; i < army_size; i++)
    {
        SPDLOG_DEBUG("Trying to find pon: {}", i);
        Pon* current_pon = CoreManager::getInstance().getSaveReader()->ponReg.GetPonByID(i);
        SPDLOG_DEBUG("Making pon with class: {}", current_pon->pon_class);
        switch (current_pon->pon_class)
        {
            case -1: ///this was earlier 0 which i dont understand because pon class 0 = yaripon lol
            {
                SPDLOG_WARN("Hatapon detected in saveReader.ponreg.pons");
                break;
            }
            case 0: {
                unique_ptr<Yaripon> wip_pon = make_unique<Yaripon>();
                wip_pon.get()->entityID = -1001; ///lets say entity IDs for units will be -1000 and below, so -1001 is yaripon, -1002 will be tatepon etc
                wip_pon.get()->setUnitID(current_pon->pon_class + 1); ///have to set unit ID from 0 to 1 because 0 is already occupied by Hatapon
                wip_pon.get()->LoadConfig();

                // TO-DO: what to do with cache now?
                //if (!CoreManager::getInstance().getCore()->isCached[wip_pon.get()->entityID])
                //{
                    //CoreManager::getInstance().getCore()->cacheEntity(wip_pon.get()->entityID, wip_pon.get()->all_swaps_img, wip_pon.get()->animation_spritesheet, wip_pon.get()->objects);
                //}

                wip_pon.get()->mindmg = current_pon->pon_min_dmg;
                wip_pon.get()->maxdmg = current_pon->pon_max_dmg;
                wip_pon.get()->current_hp = current_pon->pon_hp;
                wip_pon.get()->max_hp = current_pon->pon_hp;

                if (current_pon->slots[0] != -1)
                {
                    //TO-DO: wip_pon.get()->applyEquipment(CoreManager::getInstance().getSaveReader()->invData.items[current_pon->slots[0]].item->order_id, 0);
                } else
                {
                    SPDLOG_ERROR("Yaripon has an empty equipment slot 1");
                }

                if (current_pon->slots[1] != -1)
                {
                    //TO-DO: wip_pon.get()->applyEquipment(CoreManager::getInstance().getSaveReader()->invData.items[current_pon->slots[1]].item->order_id, 1);
                } else
                {
                    SPDLOG_ERROR("Yaripon has an empty equipment slot 2");
                }

                units.push_back(std::move(wip_pon));
                break;
            }
            case 2: {
                /*
				unique_ptr<Tatepon> wip_pon = make_unique<Tatepon>();
				wip_pon.get()->LoadConfig(thisConfig);
				wip_pon.get()->setUnitID(current_pon->pon_class);
				wip_pon.get()->mindmg = current_pon->pon_min_dmg;
				wip_pon.get()->maxdmg = current_pon->pon_max_dmg;
				wip_pon.get()->current_hp = current_pon->pon_hp;
				wip_pon.get()->max_hp = current_pon->pon_hp;
				wip_pon.get()->applyEquipment(CoreManager::getInstance().getSaveReader()->invData.items[current_pon->slots[0]].item->order_id, 0);
				wip_pon.get()->applyEquipment(CoreManager::getInstance().getSaveReader()->invData.items[current_pon->slots[1]].item->order_id, 1);
				units.push_back(std::move(wip_pon));
				break;*/
                break;
            }
                /*case 3:
			{
				unique_ptr<Yumipon> wip_pon = make_unique<Yumipon>();
				break;
			}
			case 4:
			{
				unique_ptr<Hero> wip_pon = make_unique<Hero>();
				break;
			}*/
                ///For later lol
        }
        /*if(current_pon->pon_class != 0)
		{
			wip_pon.get()->LoadConfig(thisConfig);
			wip_pon.get()->setUnitID(current_pon->pon_class);
			wip_pon.get()->mindmg = current_pon->pon_min_dmg;
			wip_pon.get()->maxdmg = current_pon->pon_max_dmg;
			wip_pon.get()->current_hp = current_pon->pon_hp;
			wip_pon.get()->max_hp = current_pon->pon_hp;
			wip_pon.get()->applyWeapon(CoreManager::getInstance().getSaveReader()->invData.GetItemByInvID(current_pon->weapon_invItem_id).item->item_type, CoreManager::getInstance().getSaveReader()->invData.GetItemByInvID(current_pon->weapon_invItem_id).item->item_id, 1);
			wip_pon.get()->applyHelm(CoreManager::getInstance().getSaveReader()->invData.GetItemByInvID(current_pon->armour_invItem_id).item->item_id);
			units.push_back(std::move(wip_pon));
		}*/
        ///-_-

        ///what the hell is this
        /** bool has_thumb = false;
        for (int i = 0; i < unitThumbs.size(); i++)
        {
            if (unitThumbs[i].unit_class == current_pon->pon_class)
                has_thumb = true;
        }
        if (!has_thumb)
        {
            addUnitThumb(current_pon->pon_class);
        }**/
    }

    // TO-DO: this whole section needs a rewrite to the new code mechanics
    /*
    addUnitThumb(1);

	SPDLOG_DEBUG("Mission loading finished."); */

    SPDLOG_DEBUG("Set rich presence to {}", missionImg);

    string fm = "Playing mission: " + missionName;
    CoreManager::getInstance().getCore()->changeRichPresence(fm.c_str(), missionImg.c_str(), "logo");
    
    SPDLOG_INFO("Loading background {}", bgName);
    mission_bg.Load(bgName);

    Rhythm* rhythm = CoreManager::getInstance().getRhythm();
    rhythm->LoadTheme(songName);

    missionTimer.restart();

    missionThreads.push_back(std::thread(&MissionController::DoRhythm, this));

    isFinishedLoading = true;
    initialized = true;

    //TO-DO: start rhythm when tips are clicked
    //rhythm->Start();
}

void MissionController::StopMission()
{
    Rhythm* rhythm = CoreManager::getInstance().getRhythm();

    rhythm->Stop();
    initialized = false;
}
void MissionController::DoKeyboardEvents()
{
    InputController* inputCtrl = CoreManager::getInstance().getInputController();

    /**
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
	{
		missionEnd = true;
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::M))
	{
		if(!debug_map_drop)
		{
			auto item = CoreManager::getInstance().getSaveReader()->itemreg.GetItemByID(23);
			vector<string> data = {item->spritesheet, to_string(item->spritesheet_id), to_string(23)};

			spawnEntity("droppeditem",5,0,500,0,600,0,0,1,sf::Color::White,0,0,vector<Entity::Loot>(), data);

			debug_map_drop = true;
		}
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::L))
	{
		for(int i=0; i<units.size(); i++)
		{

				units[i].get()->setUnitHP(units[i].get()->getUnitHP() - (25.0/fps));

				if(units[i].get()->getUnitHP() <= 0)
				{
					units[i].get()->setUnitHP(0);
				}
		}
	}**/

    if (!missionEnd)
    {
        if ((inputCtrl->isKeyHeld(Input::Keys::LTRIGGER)) && (inputCtrl->isKeyHeld(Input::Keys::RTRIGGER)) && (inputCtrl->isKeyHeld(Input::Keys::SQUARE)))
        {
            if (inputCtrl->isKeyPressed(Input::Keys::SELECT))
            {
                std::vector<sf::String> a = {"Show hitboxes", "Hide hitboxes", "Heal units", "Kill all player units", "Kill Hatapon", "Enable verbose logging", "Mission complete", "Toggle rhythm debug UI", "Toggle song debug"};

                PataDialogBox db;
                db.Create(f_font, "Debug menu", a, CoreManager::getInstance().getConfig()->GetInt("textureQuality"));
                db.id = 999;
                dialog_boxes.push_back(db);
            }
        } else if (inputCtrl->isKeyPressed(Input::Keys::START))
        {
            std::vector<sf::String> a = {Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("nav_yes")), Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("nav_no"))};

            PataDialogBox db;
            db.Create(f_font, Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("mission_backtopatapolis")), a, CoreManager::getInstance().getConfig()->GetInt("textureQuality"));
            dialog_boxes.push_back(db);
        }
    }
}

vector<MissionController::CollisionEvent> MissionController::DoCollisions(HitboxFrame* currentObjectHitBoxFrame, float currentObjectX, float currentObjectY, int collisionObjectID, vector<CollidableObject*> collisionObjects, vector<string> collisionData)
{
    ///Added vector because there can be more than just one collision events! Like colliding with grass AND with opponent
    vector<MissionController::CollisionEvent> collisionEvents;

    for (auto& collisionObject : collisionObjects)
    {
        if (!collisionObject->offbounds && collisionObject->isCollidable)
        {
            auto objectCollisionEvents = computeCollisions(currentObjectHitBoxFrame, currentObjectX, currentObjectY, collisionObjectID, collisionData, collisionObject);
            collisionEvents.insert(collisionEvents.end(), objectCollisionEvents.begin(), objectCollisionEvents.end());
        }
    }

    return collisionEvents;
}
vector<MissionController::CollisionEvent> MissionController::computeCollisions(HitboxFrame* currentObjectHitBoxFrame, float currentObjectX, float currentObjectY, int collisionObjectID, vector<string>& collisionData, CollidableObject* collisionObject)
{
    std::vector<CollisionEvent> collisionEvents;



    for (int h = 0; h < collisionObject->hitboxes.size(); h++)
    {
        //cout << "tangibleLevelObjects[" << i << "][" << h << "]" << endl;

        /// NEW COLLISION SYSTEM:
        /// Separating axis theorem
        /// we check an axis at a time
        /// 8 axes in total, aligned with the normal of each face of each shape
        /// thankfully because we are only using rectangles, there are two pairs of parallel sides
        /// so we only need to check 4 axes, as the other 4 are all parallel.
        ///
        /// in each axis we calculate the vector projection onto the axis between the origin and each corner of each box
        /// and find the maximum projection and minimum projection for each shape
        /// then we check if min2>max1 or min1>max2 there has been a collision in this axis
        /// there has to be a collision in ALL axes for actual collision to be confirmed,
        /// so we can stop checking if we find a single non-collision.

        if (collisionObject->hitboxes[h].hitboxObject.max_width < 0) {
            // not sure why kirajins werent getting their hitbox width updated anywhere else
            // figured it was something to do with them jumping from behind a bush
            // in any case, it seems best to double check and do the calculation here to make sure so we don't have to worry about other edge cases popping up
            //collisionObject->hitboxes[h].hitboxObject.calcMaxWidth();
            continue;
        }

        // new optimisation:
        // check if the two objects are more than their diagonals added together away from each other
        // if they are, then they can't possibly collide so we dont need to check

        float max_distance_collision = currentObjectHitBoxFrame->max_width + collisionObject->hitboxes[h].hitboxObject.max_width;
        float x_distance_apart = abs((collisionObject->getGlobalPosition().x + collisionObject->hitBox.left) - currentObjectX);
        if (x_distance_apart>max_distance_collision)
        {
        //    // objects too far apart - skip checking this collision
            continue;
        }

        
        // Here we make a collision step for projectile at x = currentObjectX against object at x = tangibleLevelObjects[i]->global_x
        /// axis 1: obj1 "sideways" We start with sideways because it is less likely to contain a collision
        float currentAxisAngle = currentObjectHitBoxFrame->rotation;

        bool isCollision = DoCollisionStepInAxis(currentAxisAngle, &(collisionObject->hitboxes[h].hitboxObject), collisionObject, currentObjectHitBoxFrame, currentObjectX, currentObjectY);
        if (!isCollision)
        {
            continue;
        }

        /// axis 2: obj1 "up"
        currentAxisAngle = 3.14159265358 / 2 + currentObjectHitBoxFrame->rotation;
        bool isCollision2 = DoCollisionStepInAxis(currentAxisAngle, &(collisionObject->hitboxes[h].hitboxObject), collisionObject, currentObjectHitBoxFrame, currentObjectX, currentObjectY);
        if (!isCollision2)
        {
            continue;
        }

        /// axis 3: obj2 "up" (we add the 90 degrees from before to its current rotation)
        currentAxisAngle = collisionObject->hitboxes[h].hitboxObject.rotation + 3.14159265358 / 2;

        bool isCollision3 = DoCollisionStepInAxis(currentAxisAngle, &(collisionObject->hitboxes[h].hitboxObject), collisionObject, currentObjectHitBoxFrame, currentObjectX, currentObjectY);
        if (!isCollision3)
        {
            continue;
        }

        /// axis 4: obj2 "sideways"
        currentAxisAngle = collisionObject->hitboxes[h].hitboxObject.rotation;

        bool isCollision4 = DoCollisionStepInAxis(currentAxisAngle, &(collisionObject->hitboxes[h].hitboxObject), collisionObject, currentObjectHitBoxFrame, currentObjectX, currentObjectY);
        if (!isCollision4)
        {
            continue;
        }

        /// we have a collision
        if (isCollision && isCollision2 && isCollision3 && isCollision4)
        {
            PlayableUnit* unit;
            Entity* entity;
            CollisionEvent cevent;
            cevent.collided = true;
            cevent.isAttackable = collisionObject->isAttackable;
            cevent.isCollidable = collisionObject->isCollidable;

            if ((unit = dynamic_cast<PlayableUnit*>(collisionObject)))
            {
                cevent.collidedEntityCategory = 2;

                if (unit->defend)
                {
                    if (unit->charged)
                        cevent.defend_factor = 0.25;
                    else
                        cevent.defend_factor = 0.5;
                }
            } else if ((entity = dynamic_cast<Entity*>(collisionObject)))
            {
                cevent.collidedEntityCategory = entity->entityCategory;
            }

            collisionObject->OnCollide(collisionObject, collisionObjectID, collisionData);

            collisionEvents.push_back(cevent);
        } 
        else
        {
            // should never happen
            SPDLOG_ERROR("Found object with no collisions on any axes");
        }
    }

    return collisionEvents;
}

bool MissionController::DoCollisionStepInAxis(float currentAxisAngle, HitboxFrame* currentHitboxFrame, AnimatedObject* targetObject, HitboxFrame* currentObjectHitBoxFrame, float currentObjectX, float currentObjectY)
{
    std::vector<sf::Vector2f> currentVertices = currentObjectHitBoxFrame->getCurrentVertices();

    if (currentVertices.size() < 4)
	{
		SPDLOG_WARN("Vertices alert!!! {}", currentVertices.size());
	}

    if (currentVertices.size() >= 4)
    {
        PVector pv1 = PVector::getVectorCartesian(0, 0, currentVertices[0].x + currentObjectX, currentVertices[0].y + currentObjectY);
        PVector pv2 = PVector::getVectorCartesian(0, 0, currentVertices[1].x + currentObjectX, currentVertices[1].y + currentObjectY);
        PVector pv3 = PVector::getVectorCartesian(0, 0, currentVertices[2].x + currentObjectX, currentVertices[2].y + currentObjectY);
        PVector pv4 = PVector::getVectorCartesian(0, 0, currentVertices[3].x + currentObjectX, currentVertices[3].y + currentObjectY);
        pv1.angle = -atan2(currentVertices[0].y + currentObjectY, currentVertices[0].x + currentObjectX);
        pv2.angle = -atan2(currentVertices[1].y + currentObjectY, currentVertices[1].x + currentObjectX);
        pv3.angle = -atan2(currentVertices[2].y + currentObjectY, currentVertices[2].x + currentObjectX);
        pv4.angle = -atan2(currentVertices[3].y + currentObjectY, currentVertices[3].x + currentObjectX);

        float proj1 = pv1.GetScalarProjectionOntoAxis(currentAxisAngle);
        float proj2 = pv2.GetScalarProjectionOntoAxis(currentAxisAngle);
        float proj3 = pv3.GetScalarProjectionOntoAxis(currentAxisAngle);
        float proj4 = pv4.GetScalarProjectionOntoAxis(currentAxisAngle);


        float maxProjectionObj1 = max(max(max(proj1, proj2), proj3), proj4);
        float minProjectionObj1 = min(min(min(proj1, proj2), proj3), proj4);

        float maxProjectionObj2 = currentHitboxFrame->maxProjection(currentAxisAngle, targetObject->getGlobalPosition().x, targetObject->getGlobalPosition().y);
        float minProjectionObj2 = currentHitboxFrame->minProjection(currentAxisAngle, targetObject->getGlobalPosition().x, targetObject->getGlobalPosition().y);
        if (maxProjectionObj1 > minProjectionObj2 && minProjectionObj1 < maxProjectionObj2)
        {
            return true;
        } else
        {
            return false;
        }
    } else
    {
        return false;
    }
}

void MissionController::DoMovement()
{
    /** Make Patapon walk (temporary) **/
    float booster = 1.0;

    // TO-DO: replace with callback potentially
    //if (rhythm->current_perfect == 4)
    //{
    //    booster = 1.2;
    //}

    if (missionEnd)
    {
        booster = 1.0;
    }

    /** Find the farthest unit in your army (for calculations) **/
    /** Patapon movement **/

    if (!units.empty())
    {
        PlayableUnit* farthest_unit = std::max_element(units.begin(),
                                                       units.end(),
                                                       less_by([](const unique_ptr<PlayableUnit>& unit) {
                                                           return unit->getGlobalPosition().x + unit->local_x;
                                                       }))
                                              ->get();

        bool foundCollision = false;
        for (auto& tangibleLevelObject : tangibleLevelObjects)
        {
            if (!tangibleLevelObject->offbounds)
            {
                foundCollision = foundCollision || isColliding(farthest_unit, tangibleLevelObject);
            }
        }

        if ((camera.walk) || ((missionEnd) && (!failure)))
        {
            float pataDistance = 240 * booster;

            if (walkBackwards)
                pataDistance = -pataDistance;

            float diff = (Smoothstep(walkClock.getElapsedTime().asSeconds() / 2) * pataDistance) - (Smoothstep(prevTime / 2) * pataDistance);
            prevTime = walkClock.getElapsedTime().asSeconds();

            float proposedXPos = farthest_unit->getGlobalPosition().x + diff;

            camera.pataSpeed = (2 * 60 * booster);

            if (walkBackwards)
                camera.pataSpeed = -camera.pataSpeed;

            //cout << "global_x: " << farthest_unit->global_x << endl;
            //cout << "proposedXPos = " << proposedXPos << endl;
            /// use the right hand side of the patapon sprite to check for collisions. This should be changed if the patapon walks to the left
            //float proposedXPosRight = proposedXPos + farthest_unit->hitBox.left + farthest_unit->hitBox.width;
            /// need to have it check for collision and stop if blocked by kacheek here.

            /// right now it is very basic checking only in X axis. Jumping over a
            /// kacheek will not be possible.

            /// if the new position is inside a kacheek, don't move. If we found anything,
            if (!foundCollision)
            {
                if (!missionEnd)
                    army_x += diff;
                else
                    army_x += 120.0 / fps;
            }
        }
    }

    /** Set global positions for the units **/

    for (int i = 0; i < units.size(); i++)
    {
        PlayableUnit* unit = units[i].get();

        bool foundCollision = false;

        for (auto& tangibleLevelObject : tangibleLevelObjects)
        {
            if (!tangibleLevelObject->offbounds)
            {
                foundCollision = foundCollision || isColliding(unit, tangibleLevelObject);
            }
        }

        if (!unit->dead)
        {
            if (unit->local_x < unit->dest_local_x)
            {
                if (!foundCollision)
                    unit->local_x += 200.0 / fps;

                /*if(unit->enemy_in_range)
				{
					if(unit->getAnimationSegment() != "walk_focused")
					unit->setAnimationSegment("walk_focused", true);
				}
				else
				{
					if(unit->getAnimationSegment() != "walk")
					unit->setAnimationSegment("walk", true);
				}*/
            }
            if (unit->local_x > unit->dest_local_x)
            {
                unit->local_x -= 200.0 / fps;

                /*if(unit->enemy_in_range)
				{
					if(unit->getAnimationSegment() != "walk_focused")
					unit->setAnimationSegment("walk_focused", true);
				}
				else
				{
					if(unit->getAnimationSegment() != "walk")
					unit->setAnimationSegment("walk", true);
				}*/
            }
        }

        float x_offset = 0;
        float y_value = 0;

        if (unit->getUnitID() == 0)
        { // Hatapon
            y_value = 500;
        } else
        {
            y_value = patapon_y;
        }

        switch (unit->getUnitID())
        {
            case 1: // Yaripon
            {
                x_offset = 100 + (50 * i);
                break;
            }
            case 2: // Tatepon
            {
                x_offset = 200 + (50 * i);
                break;
            }
        }

        unit->setGlobalPosition(sf::Vector2f(army_x + x_offset, y_value));
    }
}

bool MissionController::isColliding(PlayableUnit* unit, const unique_ptr<Entity>& entity)
{
    auto hitboxFrame = unit->hitboxes[0].getRect();
    const float unitX = unit->getGlobalPosition().x;
    const float unitY = unit->getGlobalPosition().y;
    const int collisionObjectID = -1;
    std::vector<string> collisionData;
    CollidableObject* collisionObject = entity.get();
    std::vector<CollisionEvent> collisionEvents = computeCollisions(&hitboxFrame, unitX, unitY, collisionObjectID, collisionData, collisionObject);

    collisionEvents.erase(std::remove_if(collisionEvents.begin(), collisionEvents.end(), [](const auto& event) { return !event.isCollidable; }), collisionEvents.end());

    for (int i=0; i<collisionEvents.size(); i++)
    {
        SPDLOG_TRACE("Is collidable? {} vs {}", collisionEvents[i].isCollidable, collisionObject->isCollidable);
    }

    bool foundCollision = !collisionEvents.empty();
    bool forceCollision = false;

    if (collisionObject->entityID != 5)
    {
        if (collisionObject->isCollidable)
        {
            forceCollision = unit->getGlobalPosition().x >= collisionObject->getGlobalPosition().x + collisionObject->hitboxes[0].o_x;
        }
    }

    if (!collisionObject->isCollidable)
        foundCollision = false;

    return foundCollision || forceCollision;
}

void MissionController::DoRhythm()
{
    while(!missionEnd)
    {
        /** Call Rhythm functions **/
        InputController* inputCtrl = CoreManager::getInstance().getInputController();
        RhythmController* rhythmController = CoreManager::getInstance().getRhythmController();
        Rhythm* rhythm = CoreManager::getInstance().getRhythm();

        if ((rhythm->current_song == "patapata") || (rhythm->current_song == "chakapata"))
        {
            //cout << "set walk true" << endl;
            camera.walk = true;

            if (rhythm->current_song == "chakapata")
                walkBackwards = true;
            else
                walkBackwards = false;

            if (!startWalking)
            {
                walkClock.restart();
                prevTime = 0;

                startWalking = true;
            }
        } else
        {
            //cout << "set walk false" << endl;
            camera.walk = false;

            startWalking = false;
        }

        if ((rhythmController->current_drum == "pata") || (rhythmController->current_drum == "pon") || (rhythmController->current_drum == "chaka") || (rhythmController->current_drum == "don"))
        {
            rhythmController->current_drum = "";
            rhythm->current_song = "";
        }

        rhythm->doRhythm();
        std::this_thread::sleep_for(1ms);
    }
}

void MissionController::ClearMissionMemory()
{
    vector<std::unique_ptr<Entity>>().swap(tangibleLevelObjects);
    vector<std::unique_ptr<PlayableUnit>>().swap(units);
    vector<std::unique_ptr<Kirajin_Yari_2>>().swap(kirajins);

    levelProjectiles.clear();

    droppeditem_spritesheet.clear();
    dmgCounters.clear();
    droppedItems.clear();
    pickedItems.clear();
    unitThumbs.clear();
    droppable_cache.clear();
}

void MissionController::DoMissionEnd()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();

    /** Make the missionEndTimer unusable until the mission is not finished **/
    if (!missionEnd)
        missionEndTimer.restart();

    /** Mission end cheering **/

    if (missionEnd)
    {
        if (!failure)
        {
            if (missionEndTimer.getElapsedTime().asMilliseconds() >= 2500)
            {
                if (!playCheer[0])
                {
                    s_cheer.stop();
                    s_cheer.setBuffer(sb_cheer1);
                    s_cheer.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));
                    s_cheer.play();
                    playCheer[0] = true;
                }
            }

            if (missionEndTimer.getElapsedTime().asMilliseconds() >= 4500)
            {
                if (!playCheer[1])
                {
                    s_cheer.stop();
                    s_cheer.setBuffer(sb_cheer2);
                    s_cheer.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));
                    s_cheer.play();
                    playCheer[1] = true;
                }
            }

            if (missionEndTimer.getElapsedTime().asMilliseconds() >= 6500)
            {
                if (!playCheer[2])
                {
                    s_cheer.stop();
                    s_cheer.setBuffer(sb_cheer3);
                    s_cheer.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));
                    s_cheer.play();
                    playCheer[2] = true;
                }
            }

            if (missionEndTimer.getElapsedTime().asMilliseconds() >= 8000)
            {
                if (!playJingle)
                {
                    s_jingle.setBuffer(sb_win_jingle);
                    s_jingle.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));
                    s_jingle.play();
                    playJingle = true;
                }
            }
        } else
        {
            if (!playJingle)
            {
                s_jingle.setBuffer(sb_lose_jingle);
                s_jingle.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));
                s_jingle.play();
                playJingle = true;
            }
        }
    }

    /** Make the camera follow Patapons until the jingle is played **/

    if (missionEndTimer.getElapsedTime().asMilliseconds() < 7700)
    {
        camera.followobject_x = army_x * (window->getSize().x / float(1280));
    }

    /** Mission fade in and fade out **/

    if (!missionEnd)
    {
        if (fade_alpha > 0)
        {
            fade_alpha -= float(500) / fps;
        }

        if (fade_alpha <= 0)
        {
            fade_alpha = 0;
        }
    } else
    {
        if (!failure)
        {
            if (missionEndTimer.getElapsedTime().asMilliseconds() >= 11000)
            {
                if (fade_alpha < 255)
                {
                    fade_alpha += float(250) / fps;
                }

                if (fade_alpha >= 255)
                {
                    fade_alpha = 255;
                }
            }
        } else
        {
            if (missionEndTimer.getElapsedTime().asMilliseconds() >= 1000)
            {
                if (fade_alpha < 255)
                {
                    fade_alpha += float(250) / fps;
                }

                if (fade_alpha >= 255)
                {
                    fade_alpha = 255;
                }
            }
        }
    }

    fade_box.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
    fade_box.setFillColor(sf::Color(0, 0, 0, fade_alpha));
    window->draw(fade_box);

    /** Mission end event (Mission complete/Mission failed screen + transition to Patapolis **/

    if (!failure) ///Victory
    {
        if (missionEndTimer.getElapsedTime().asMilliseconds() >= 11500)
        {
            if (!textBounce)
            {
                if (missionEndTimer.getElapsedTime().asMilliseconds() >= 13050)
                {
                    textCurScale = 1.4;
                    textBounce = true;
                }
            }

            t_win.setOrigin(t_win.getLocalBounds().width / 2, t_win.getLocalBounds().height / 2);
            t_win_outline.setOrigin(t_win_outline.getLocalBounds().width / 2, t_win_outline.getLocalBounds().height / 2);

            if (barCurX > barDestX)
            {
                barCurX -= (abs(barCurX - barDestX) * 5) / fps;
            } else
            {
                barCurX = barDestX;
            }

            if (textCurX < textDestX)
            {
                textCurX += (abs(textCurX - textDestX) * 5) / fps;
            } else
            {
                textCurX = textDestX;
            }
            if (textCurScale > textDestScale)
            {
                textCurScale -= (abs(textCurScale - textDestScale) * 5) / fps;
            } else
            {
                textCurScale = textDestScale;
            }

            t_win.setScale(textCurScale);
            t_win_outline.setScale(textCurScale);

            bar_win.setPosition(barCurX, 360);
            t_win.setPosition(textCurX - 7, 360 - 14);
            t_win_outline.setPosition(textCurX + 2, 360 - 4);

            bar_win.draw(window);
            t_win_outline.draw(window);
            t_win.draw(window);

            if (missionEndTimer.getElapsedTime().asMilliseconds() >= 17000)
            {
                if (fadeout_alpha < 255)
                {
                    fadeout_alpha += float(250) / fps;
                }

                if (fadeout_alpha >= 255)
                {
                    fadeout_alpha = 255;
                }

                fadeout_box.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
                fadeout_box.setFillColor(sf::Color(0, 0, 0, fadeout_alpha));
                window->draw(fadeout_box);
            }

            if (missionEndTimer.getElapsedTime().asMilliseconds() > 19000)
            {
                SPDLOG_INFO("End mission");

                StopMission();

                SPDLOG_INFO("Add the picked up items to item repository");
                submitPickedItems();
                updateMissions();
                ClearMissionMemory();

                SPDLOG_INFO("Go to Patapolis");
                returnToPatapolis = true;
            }
        }
    } else ///Failure
    {
        if (missionEndTimer.getElapsedTime().asMilliseconds() >= 2500)
        {
            t_lose.setOrigin(t_lose.getLocalBounds().width / 2, t_lose.getLocalBounds().height / 2);
            t_lose_outline.setOrigin(t_lose_outline.getLocalBounds().width / 2, t_lose_outline.getLocalBounds().height / 2);

            if (barCurX > barDestX)
            {
                barCurX -= (abs(barCurX - barDestX) * 5) / fps;
            } else
            {
                barCurX = barDestX;
            }

            if (textCurX < textDestX)
            {
                textCurX += (abs(textCurX - textDestX) * 5) / fps;
            } else
            {
                textCurX = textDestX;
            }

            t_lose.setScale(textCurScale);
            t_lose_outline.setScale(textCurScale);

            bar_lose.setPosition(barCurX, 360);
            t_lose.setPosition(textCurX - 7, 360 - 14);
            t_lose_outline.setPosition(textCurX + 2, 360 - 4);

            bar_lose.draw(window);
            t_lose_outline.draw(window);
            t_lose.draw(window);

            if (missionEndTimer.getElapsedTime().asMilliseconds() >= 6000)
            {
                if (fadeout_alpha < 255)
                {
                    fadeout_alpha += float(250) / fps;
                }

                if (fadeout_alpha >= 255)
                {
                    fadeout_alpha = 255;
                }

                fadeout_box.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
                fadeout_box.setFillColor(sf::Color(0, 0, 0, fadeout_alpha));
                window->draw(fadeout_box);
            }

            if (missionEndTimer.getElapsedTime().asMilliseconds() >= 8000)
            {
                /** End flag executes the mission victory code, so mission failed code needs to be executed separately here. **/

                SPDLOG_INFO("End mission");

                StopMission();
                ClearMissionMemory();

                SPDLOG_INFO("Go to Patapolis");
                returnToPatapolis = true;
            }
        }
    }
}

void MissionController::DoVectorCleanup(vector<int> units_rm, vector<int> dmg_rm, vector<int> tlo_rm, vector<int> pr_rm)
{
    //cout << "MissionController::DoVectorCleanup" << endl;
    //cout << units_rm.size() << " " << dmg_rm.size() << " " << tlo_rm.size() << " " << pr_rm.size() << endl;

    if (units_rm.size() > 0)
        SPDLOG_DEBUG("[MissionController::DoVectorCleanup] There are {} units to clean up", units_rm.size());
    if (dmg_rm.size() > 0)
        SPDLOG_DEBUG("[MissionController::DoVectorCleanup] There are {} dmgCounters to clean up", dmg_rm.size());
    if (tlo_rm.size() > 0)
        SPDLOG_DEBUG("[MissionController::DoVectorCleanup] There are {} tangibleLevelObjects to clean up", tlo_rm.size());
    if (pr_rm.size() > 0)
        SPDLOG_DEBUG("[MissionController::DoVectorCleanup] There are {} levelProjectiles to clean up", pr_rm.size());

    for (int i = 0; i < units_rm.size(); i++)
    {
        SPDLOG_TRACE("Erasing unit {}", units_rm[i]);
        units.erase(units.begin() + (units_rm[i] - i));
        SPDLOG_TRACE("Erased unit {}", units_rm[i]);
    }

    for (int i = 0; i < dmg_rm.size(); i++)
    {
        SPDLOG_TRACE("Erasing dmgCounter {}", dmg_rm[i]);
        dmgCounters.erase(dmgCounters.begin() + (dmg_rm[i] - i));
        SPDLOG_TRACE("Erased dmgCounter {}", dmg_rm[i]);
    }

    for (int i = 0; i < tlo_rm.size(); i++)
    {
        SPDLOG_TRACE("Erasing tangibleLevelObject {}", tlo_rm[i]);
        tangibleLevelObjects.erase(tangibleLevelObjects.begin() + (tlo_rm[i] - i));
        SPDLOG_TRACE("Erased tangibleLevelObject {}", tlo_rm[i]);
    }

    for (int i = 0; i < pr_rm.size(); i++)
    {
        SPDLOG_TRACE("Erasing levelProjectile {}", pr_rm[i]);
        levelProjectiles.erase(levelProjectiles.begin() + (pr_rm[i] - i));
        SPDLOG_TRACE("Erased levelProjectile {}", pr_rm[i]);
    }
}

std::vector<int> MissionController::DrawProjectiles()
{
    /** Projectile management **/

    vector<int> pr_e; ///projectiles to erase

    /// step 1: all projectiles have gravity applied to them
    for (int i = 0; i < levelProjectiles.size(); i++)
    {
        Projectile* p = levelProjectiles[i].get();
        float xspeed = p->GetXSpeed();
        float yspeed = p->GetYSpeed();
        yspeed += (gravity / fps);
        p->SetNewSpeedVector(xspeed, yspeed);
        p->Update();
    }

    /// step 3: any projectiles that hit any collidableobject are informed
    for (int i = 0; i < levelProjectiles.size(); i++)
    {
        bool removeProjectile = false;

        Projectile* p = levelProjectiles[i].get();
        float ypos = p->yPos;
        float xpos = p->xPos;
        HitboxFrame tmp;
        tmp.time = 0;
        tmp.g_x = 0;
        tmp.g_y = 0;
        tmp.clearVertices();
        tmp.addVertex(-3, -1); /// "top left"
        tmp.addVertex(3, -1);  /// "top right"
        tmp.addVertex(-3, 1);  /// "bottom left"
        tmp.addVertex(3, 1);   /// "bottom right"

        tmp.rotation = -p->angle;

        // projectile hitboxes are constant size, so their max width is constant too. Precalculated to 6.32456 (rounded up for good measure)
        tmp.calcMaxWidth(6.325);

        if (ypos > floor_y)
        {
            ///create hit sound
            projectile_sounds.emplace_back();

            projectile_sounds[projectile_sounds.size() - 1].setBuffer(spear_hit_solid);

            projectile_sounds[projectile_sounds.size() - 1].setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));
            projectile_sounds[projectile_sounds.size() - 1].play();

            removeProjectile = true;
        }

        ///calculate projectile damage
        ///and pass it to a special vector called collisionData
        ///which passes whatever you'd like to the collided animation object
        ///so you can put anything and react with it in the individual entity classes
        ///in projectiles' case, im transferring the damage dealt

        if (!removeProjectile)
        {
            int min_dmg = p->min_dmg;
            int max_dmg = p->max_dmg;
            int bound = max_dmg - min_dmg + 1;
            int ranDmg = rand() % bound;
            int total = min_dmg + ranDmg;

            ///sending damage dealt
            vector<string> collisionData = {to_string(total)};

            ///retrieve collision event
            vector<CollisionEvent> cevent;

            ///check whether the projectile was on enemy side or ally side
            if (!p->enemy)
            {
                ///Do collisions for all entities (ally projectiles)
                std::vector<CollidableObject*> objectPointers;
                objectPointers.reserve(tangibleLevelObjects.size());
                for (auto& object : tangibleLevelObjects)
                {
                    objectPointers.push_back(object.get());
                }
                cevent = DoCollisions(&tmp, xpos, ypos, p->projectile_id, objectPointers, collisionData);
            } else
            {
                ///Do collisions for all units (enemy projectiles)
                std::vector<CollidableObject*> unitPointers;
                unitPointers.reserve(units.size());
                for (auto& unit : units)
                {
                    unitPointers.push_back(unit.get());
                }
                cevent = DoCollisions(&tmp, xpos, ypos, p->projectile_id, unitPointers, collisionData);
            }

            for (int e = 0; e < cevent.size(); e++)
            {
                if (cevent[e].collided)
                {
                    ///add damage counter
                    if ((cevent[e].isAttackable) && (cevent[e].isCollidable))
                    {
                        ///create hit sound
                        projectile_sounds.emplace_back();

                        ///locate the right buffer
                        switch (cevent[e].collidedEntityCategory)
                        {
                            case -1: {
                                break;
                            }

                            case Entity::EntityCategories::ANIMAL: {
                                projectile_sounds[projectile_sounds.size() - 1].setBuffer(spear_hit_enemy);
                                break;
                            }

                            case Entity::EntityCategories::ENEMYUNIT: {
                                projectile_sounds[projectile_sounds.size() - 1].setBuffer(spear_hit_enemy);
                                break;
                            }

                            case Entity::EntityCategories::BUILDING_REGULAR: {
                                projectile_sounds[projectile_sounds.size() - 1].setBuffer(spear_hit_solid);
                                break;
                            }

                            case Entity::EntityCategories::OBSTACLE_IRON: {
                                projectile_sounds[projectile_sounds.size() - 1].setBuffer(spear_hit_iron);
                                break;
                            }

                            case Entity::EntityCategories::BUILDING_IRON: {
                                projectile_sounds[projectile_sounds.size() - 1].setBuffer(spear_hit_iron);
                                break;
                            }

                            case Entity::EntityCategories::OBSTACLE_ROCK: {
                                projectile_sounds[projectile_sounds.size() - 1].setBuffer(spear_hit_rock);
                                break;
                            }

                            case Entity::EntityCategories::OBSTACLE_WOOD: {
                                projectile_sounds[projectile_sounds.size() - 1].setBuffer(spear_hit_solid);
                                break;
                            }
                        }

                        projectile_sounds[projectile_sounds.size() - 1].setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));
                        projectile_sounds[projectile_sounds.size() - 1].play();

                        addDmgCounter(0, total * cevent[e].defend_factor, xpos, ypos, qualitySetting, resSetting);

                        removeProjectile = true;
                        break; ///break the for loop here to prevent double hits
                    }
                }
            }
        }

        p->Draw();

        if (removeProjectile)
            pr_e.push_back(i);
    }

    return pr_e;
}

void MissionController::drawCommandList(sf::RenderWindow& window)
{
    if (!missionEnd)
    {
        ///first four
        for (int i = 0; i < 4; i++)
        {
            command_inputs[i].setOrigin(command_inputs[i].getLocalBounds().width, command_inputs[i].getLocalBounds().height / 2);
            command_inputs[i].setPosition(-21 + (i + 1) * ((1280 - 42) / 4.f), 644);

            command_descs[i].setOrigin(command_descs[i].getLocalBounds().width, command_descs[i].getLocalBounds().height / 2);
            command_descs[i].setPosition(-21 + (i + 1) * ((1280 - 42) / 4.f) - command_inputs[i].getLocalBounds().width - 8, 644);

            command_descs[i].draw(window);
            command_inputs[i].draw(window);
        }

        ///second four
        for (int i = 4; i < 8; i++)
        {
            command_inputs[i].setOrigin(command_inputs[i].getLocalBounds().width, command_inputs[i].getLocalBounds().height / 2);
            command_inputs[i].setPosition(-21 + (i - 4 + 1) * ((1280 - 42) / 4.f), 673);

            command_descs[i].setOrigin(command_descs[i].getLocalBounds().width, command_descs[i].getLocalBounds().height / 2);
            command_descs[i].setPosition(-21 + (i - 4 + 1) * ((1280 - 42) / 4.f) - command_inputs[i].getLocalBounds().width - 8, 673);

            command_descs[i].draw(window);
            command_inputs[i].draw(window);
        }
    }
}

void MissionController::DrawUnitThumbs(sf::RenderWindow& window)
{
    for (int i = 0; i < unitThumbs.size(); i++)
    {
        float resRatioX = window.getSize().x / float(1280);
        float resRatioY = window.getSize().y / float(720);

        int farthest_id = -1;
        float temp_pos = -9999;

        int curunits = 0;
        float maxunithp = 0;
        float curunithp = 0;

        for (int u = 0; u < units.size(); u++)
        {
            if (units[u].get()->getUnitID() == unitThumbs[i].unit_class)
            {
                curunits++;

                PlayableUnit* unit = units[u].get();

                if (temp_pos <= unit->getGlobalPosition().x)
                {
                    temp_pos = unit->getGlobalPosition().x;
                    farthest_id = u;
                }

                maxunithp += unit->getUnitMaxHP();
                curunithp += unit->getUnitHP();
            }
        }

        if (farthest_id != -1)
        {
            float hp_percentage = curunithp / maxunithp;

            unitThumbs[i].circle.setRadius(28 * resRatioX);
            unitThumbs[i].circle.setOrigin(unitThumbs[i].circle.getLocalBounds().width / 2, unitThumbs[i].circle.getLocalBounds().height / 2);
            unitThumbs[i].circle.setPosition((52 + (64 * i)) * resRatioX, (72 * resRatioY));
            window.draw(unitThumbs[i].circle);

            // The code from earlier copied sprites from farthest_units per frame
            // The new code will instead just draw the same sprite but with modified parameters
            // So we don't waste time on copying three(!) sprites per frame per unit. Very wasteful

            //unitThumbs[i].thumb = (*obj)[0].s_obj;
            //unitThumbs[i].thumb.setScale(0.7,0.7);

            //Get Farthest unit and save old stats
            PlayableUnit* farthest_unit = units[farthest_id].get();
            //float old_scale = (*obj)[0].s_obj.scaleX;
            //float old_x = (*obj)[0].s_obj.getPosition().x;
            //float old_y = (*obj)[0].s_obj.getPosition().y;
            vector<Object>* obj;
            obj = farthest_unit->objects.get();

            (*obj)[0].s_obj.setScale(0.7, 0.7);

            int manual_x, manual_y;
            switch (unitThumbs[i].unit_class)
            {
                case 0: {
                    manual_x = 6;
                    manual_y = -40;
                    break;
                }
                case 1: {
                    manual_x = 0;
                    manual_y = 14;
                    break;
                }
                case 2: {
                    manual_x = 0;
                    manual_y = 14;
                    break;
                }
            }

            //unitThumbs[i].thumb.setPosition(52+(64*i)+manual_x, 60+manual_y);
            //unitThumbs[i].thumb.draw(window);

            //Draw the same object without copying it
            (*obj)[0].s_obj.setPosition(52 + (64 * i) + manual_x, 60 + manual_y);
            (*obj)[0].s_obj.draw(window);

            //Bring the old stats back
            //(*obj)[0].s_obj.setPosition(old_x, old_y);
            //(*obj)[0].s_obj.setScale(old_scale, old_scale);

            if (unitThumbs[i].unit_class != 0)
            {

                //unitThumbs[i].equip_1 = (*obj)[1].s_obj;
                //unitThumbs[i].equip_2 = (*obj)[2].s_obj;

                (*obj)[1].s_obj.setPosition(52 + (64 * i) + manual_x + (((*obj)[1].s_obj.getPosition().x - farthest_unit->getGlobalPosition().x) * 0.7), 60 + manual_y + (((*obj)[1].s_obj.getPosition().y - farthest_unit->getGlobalPosition().y) * 0.7));
                (*obj)[2].s_obj.setPosition(52 + (64 * i) + manual_x + (((*obj)[2].s_obj.getPosition().x - farthest_unit->getGlobalPosition().x) * 0.7), 60 + manual_y + (((*obj)[2].s_obj.getPosition().y - farthest_unit->getGlobalPosition().y) * 0.7));

                (*obj)[1].s_obj.setScale(0.7, 0.7);
                (*obj)[2].s_obj.setScale(0.7, 0.7);

                (*obj)[1].s_obj.draw(window);
                (*obj)[2].s_obj.draw(window);
            }

            unitThumbs[i].hpbar_back.setOrigin(unitThumbs[i].hpbar_back.getLocalBounds().width / 2, unitThumbs[i].hpbar_back.getLocalBounds().height / 2);
            unitThumbs[i].hpbar_back.setPosition(52 + (64 * i), 32);
            unitThumbs[i].hpbar_back.draw(window);

            unitThumbs[i].hpbar_ins.setOrigin(0, unitThumbs[i].hpbar_ins.getLocalBounds().height / 2);
            unitThumbs[i].hpbar_ins.setTextureRect(sf::IntRect(0, 0, unitThumbs[i].width * hp_percentage, unitThumbs[i].hpbar_ins.getLocalBounds().height));
            unitThumbs[i].hpbar_ins.setPosition(52 + (64 * i) - 27, 32);

            if (hp_percentage > 0.70)
                unitThumbs[i].hpbar_ins.setColor(sf::Color(0, 255, 0, 255));
            else if (hp_percentage > 0.35)
                unitThumbs[i].hpbar_ins.setColor(sf::Color(245, 230, 66, 255));
            else
                unitThumbs[i].hpbar_ins.setColor(sf::Color(212, 0, 0, 255));

            unitThumbs[i].hpbar_ins.draw(window);

            if (unitThumbs[i].unit_class != 0)
            {
                unitThumbs[i].unit_count_shadow.setString(to_string(curunits));
                unitThumbs[i].unit_count_shadow.setOrigin(unitThumbs[i].unit_count_shadow.getLocalBounds().width / 2, unitThumbs[i].unit_count_shadow.getLocalBounds().height / 2);
                unitThumbs[i].unit_count_shadow.setPosition(52 + (64 * i) + 28, 98);
                unitThumbs[i].unit_count_shadow.draw(window);

                unitThumbs[i].unit_count.setString(to_string(curunits));
                unitThumbs[i].unit_count.setOrigin(unitThumbs[i].unit_count.getLocalBounds().width / 2, unitThumbs[i].unit_count.getLocalBounds().height / 2);
                unitThumbs[i].unit_count.setPosition(52 + (64 * i) + 26, 96);
                unitThumbs[i].unit_count.draw(window);
            }
        }
    }
}

void MissionController::DrawPickedItems(sf::RenderWindow& window)
{
    for (int i = 0; i < pickedItems.size(); i++)
    {
        float resRatioX = window.getSize().x / float(1280);
        float resRatioY = window.getSize().y / float(720);

        pickedItems[i].circle.setRadius(25 * resRatioX);
        pickedItems[i].circle.setOrigin(pickedItems[i].circle.getLocalBounds().width / 2, pickedItems[i].circle.getLocalBounds().height / 2);
        pickedItems[i].circle.setPosition((1230 - 54 * i) * resRatioX, (50 * resRatioY));
        window.draw(pickedItems[i].circle);

        pickedItems[i].item.setOrigin(pickedItems[i].bounds.x / 2, pickedItems[i].bounds.y / 2);
        pickedItems[i].item.setPosition(1230 - 54 * i, 50);
        pickedItems[i].item.setScale(0.8, 0.8);
        pickedItems[i].item.draw(window);
    }
}

void MissionController::DrawHitboxes(sf::RenderWindow& window)
{
    for (int i = 0; i < units.size(); i++)
    {
        PlayableUnit* unit = units[i].get();

        for (int h = 0; h < unit->hitboxes.size(); h++)
        {
            HitboxFrame* currentHitbox = &(unit->hitboxes[h].hitboxObject);

            sf::ConvexShape convex;
            convex.setFillColor(sf::Color(150, 50, 250));
            // resize it to 5 points
            std::vector<sf::Vector2f> currentVertices = currentHitbox->getCurrentVertices();
            convex.setPointCount(currentVertices.size());

            for (int j = 0; j < currentVertices.size(); j++)
            {
                sf::Vector2f currentPoint = currentVertices[j];
                currentPoint.x = currentPoint.x + currentHitbox->g_x + unit->global_x + unit->local_x;
                currentPoint.y = currentPoint.y + currentHitbox->g_y + unit->global_y + unit->local_y;
                //cout<<"DRAWING POINT: "<<currentVertices.size()<<" x: "<<currentPoint.x<<" y: "<<currentPoint.y<<endl;
                sf::CircleShape shape(5);
                shape.setFillColor(sf::Color(100, 250, 50));
                shape.setPosition(currentPoint.x - 2.5, currentPoint.y - 2.5);
                window.draw(shape);
                convex.setPoint(j, currentPoint);
                //cout << "convex.setPoint(" << j << ", " << currentPoint.x << " " << currentPoint.y << ");" << endl;
            }

            window.draw(convex);
        }
    }

    for (int i = 0; i < tangibleLevelObjects.size(); i++)
    {
        Entity* entity = tangibleLevelObjects[i].get();

        for (int h = 0; h < entity->hitboxes.size(); h++)
        {
            HitboxFrame* currentHitbox = &(entity->hitboxes[h].hitboxObject);

            sf::ConvexShape convex;
            convex.setFillColor(sf::Color(150, 50, 250));
            // resize it to 5 points
            std::vector<sf::Vector2f> currentVertices = currentHitbox->getCurrentVertices();
            convex.setPointCount(currentVertices.size());

            for (int j = 0; j < currentVertices.size(); j++)
            {

                sf::Vector2f currentPoint = currentVertices[j];
                currentPoint.x = currentPoint.x + currentHitbox->g_x + entity->global_x + entity->local_x;
                currentPoint.y = currentPoint.y + currentHitbox->g_y + entity->global_y + entity->local_y;
                //cout<<"DRAWING POINT: "<<currentVertices.size()<<" x: "<<currentPoint.x<<" y: "<<currentPoint.y<<endl;
                sf::CircleShape shape(5);
                shape.setFillColor(sf::Color(100, 250, 50));
                shape.setPosition(currentPoint.x - 2.5, currentPoint.y - 2.5);
                window.draw(shape);
                convex.setPoint(j, currentPoint);
                //cout << "convex.setPoint(" << j << ", " << currentPoint.x << " " << currentPoint.y << ");" << endl;
            }

            window.draw(convex);
        }
    }
}

std::vector<int> MissionController::DrawDamageCounters()
{
    vector<int> dmg_rm;

    for (int i = 0; i < dmgCounters.size(); i++)
    {
        float a = 0;

        for (int d = 0; d < dmgCounters[i].spr.size(); d++)
        {
            if (dmgCounters[i].display_timer.getElapsedTime().asMilliseconds() > 70 * d)
            {
                float curScale = dmgCounters[i].scale[d];
                float destScale = dmgCounters[i].scale_goal[d];

                if (dmgCounters[i].mode[d])
                {
                    curScale -= float(14) / fps;
                    dmgCounters[i].alpha[d] += float(1800) / fps;

                    if (curScale <= destScale)
                    {
                        dmgCounters[i].mode[d] = false;
                        destScale = 1;
                    }

                    if (dmgCounters[i].alpha[d] >= 255)
                        dmgCounters[i].alpha[d] = 255;
                }

                if (!dmgCounters[i].mode[d])
                {
                    if (!dmgCounters[i].mode[d])
                    {
                        curScale += float(8) / fps;

                        if (curScale >= destScale)
                        {
                            curScale = destScale;
                        }
                    }
                }

                if (dmgCounters[i].display_timer.getElapsedTime().asMilliseconds() > 70 * d + 1000)
                {
                    if (!dmgCounters[i].mode[d])
                    {
                        dmgCounters[i].pos[d].y += float(60) / fps;
                        dmgCounters[i].alpha[d] -= float(500) / fps;

                        if (dmgCounters[i].alpha[d] <= 0)
                            dmgCounters[i].alpha[d] = 0;
                    }
                }

                dmgCounters[i].scale[d] = curScale;
                dmgCounters[i].scale_goal[d] = destScale;

                dmgCounters[i].spr[d].setPosition(dmgCounters[i].pos[d].x, dmgCounters[i].pos[d].y - ((curScale - 1) * 10));
                dmgCounters[i].spr[d].setScale(curScale, curScale);
                dmgCounters[i].spr[d].setColor(sf::Color(255, 255, 255, dmgCounters[i].alpha[d]));

                dmgCounters[i].spr[d].draw();

                a += dmgCounters[i].alpha[d];
            }
        }

        if (a <= 1)
            dmg_rm.push_back(i);
    }

    return dmg_rm;
}

std::vector<int> MissionController::DrawEntities()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();

    //cout << "[MissionController::DrawEntities] Start" << endl;
    vector<int> tlo_rm;

    /** Find the farthest unit in your army (for calculations) **/
    float farthestUnitPosition = (*std::max_element(units.begin(),
                                                    units.end(),
                                                    less_by([](const std::unique_ptr<PlayableUnit>& unit) {
                                                        return unit->getGlobalPosition().x;
                                                    })))
                                         ->getGlobalPosition()
                                         .x;

    for (int i = 0; i < tangibleLevelObjects.size(); i++)
    {
        //cout << "[MissionController::DrawEntities] Process entity " << i << endl;
        Entity* entity = tangibleLevelObjects[i].get();

        entity->fps = fps;

        if (entity->getEntityID() == 1)
        {
            RhythmController* rhythmController = CoreManager::getInstance().getRhythmController();
            RhythmGUI* rhythmGUI = CoreManager::getInstance().getRhythmGUI();

            //TO-DO: replace with rhythm callback system!!!!!!
            //entity->doRhythm(rhythm->current_song, rhythmController->current_drum, rhythm->GetCombo(), rhythm->GetRealCombo(), rhythm->advanced_prefever, rhythmGUI->beatBounce, rhythm->GetSatisfaction());

            if (!missionEnd)
            {
                entity->Update();
                entity->Draw();
            }
        } else
        {
            ///Check if entity is off bounds, if yes, don't render it.
            entity->offbounds = false;

            if (entity->getGlobalPosition().x > (camera.followobject_x) / (window->getSize().x / float(1280)) + 2400)
                entity->offbounds = true;

            if (entity->getGlobalPosition().x < (camera.followobject_x) / (window->getSize().x / float(1280)) - 1000)
                entity->offbounds = true;

            entity->distance_to_unit = abs(farthestUnitPosition - entity->getGlobalPosition().x);

            ///Check for entity attack measures
            if (entity->doAttack())
            {
                ///ID 6,16 = Kirajin Yari
                if ((entity->getEntityID() == 6) || (entity->getEntityID() == 16))
                {
                    SPDLOG_DEBUG("Entity {} threw a spear!", i);

                    float rand_hs = (rand() % 1000) / float(10);
                    float rand_vs = (rand() % 1000) / float(10);

                    float rand_rad = (rand() % 200000000) / float(1000000000);

                    float mindmg = entity->mindmg * entity->stat_multiplier;
                    float maxdmg = entity->maxdmg * entity->stat_multiplier;

                    float xpos = entity->getGlobalPosition().x + entity->hitBox.left + entity->hitBox.width / 2;
                    float ypos = entity->getGlobalPosition().y + entity->hitBox.top + entity->hitBox.height / 2;

                    vector<Object>* obj;
                    obj = entity->objects.get();

                    spawnProjectile((*obj)[1].s_obj, xpos, ypos, 800, -450 - rand_hs, -450 + rand_vs, -2.58 + rand_rad, maxdmg, mindmg, 0, true);
                }
            }

            ///Check if entity's parent is still alive, if not, kill the entity
            int parent = entity->parent;

            ///Check if parent is defined
            if (parent != -1)
            {
                ///Look for parent
                auto it = find_if(tangibleLevelObjects.begin(), tangibleLevelObjects.end(), [&parent](const std::unique_ptr<Entity>& obj) { return obj.get()->spawnOrderID == parent; });

                if (it != tangibleLevelObjects.end())
                {
                    ///Parent has been found!

                    auto index = std::distance(tangibleLevelObjects.begin(), it);
                    //cout << "My parent is currently residing at " << index << endl;

                    ///Check if it's dead
                    if (tangibleLevelObjects[index].get()->dead)
                    {
                        ///Kill the entity
                        entity->die();
                    }
                } else ///Parent can't be found
                {
                    ///Kill the entity
                    entity->die();
                }
            }

            //cout << "[MissionController::DrawEntities] Draw entity" << endl;
            entity->Update();
            entity->Draw();
        }

        //cout << "[MissionController::DrawEntities] Check if finished" << endl;
        if (entity->ready_to_erase)
            tlo_rm.push_back(i);
    }

    //cout << "[MissionController::DrawEntities] End" << endl;
    return tlo_rm;
}

std::vector<int> MissionController::DrawUnits()
{
    Rhythm* rhythm = CoreManager::getInstance().getRhythm();

    vector<int> units_rm;

    bool hatapon = false;
    if (!units.empty())
    {
        auto max_distance = std::max_element(units.begin(),
                                             units.end(),
                                             less_by([](const std::unique_ptr<PlayableUnit>& unit) {
                                                 return unit->global_x;
                                             }));

        /** Units draw loop and entity range detection **/
        int closest_entity_id = -1;
        int closest_entity_pos = 9999;

        for (int i = 0; i < tangibleLevelObjects.size(); i++)
        {
            Entity* entity = tangibleLevelObjects[i].get();

            if ((entity->entityType == Entity::EntityTypes::HOSTILE) && (!entity->dead))
            {
                if (entity->getGlobalPosition().x + entity->hitboxes[0].o_x < closest_entity_pos)
                {
                    closest_entity_pos = entity->getGlobalPosition().x + entity->hitboxes[0].o_x;
                    closest_entity_id = i;
                }
            }
        }
        bool inRange = true;

        if (closest_entity_id == -1)
        {
            inRange = false;
        } else
        {
            Entity* closest_entity = tangibleLevelObjects[closest_entity_id].get();

            //cout << "Check if entity is in range" << endl;

            for (int i = 0; i < units.size(); i++)
            {
                if (units[i].get()->getUnitID() != 0)
                {
                    if ((closest_entity->entityType == Entity::EntityTypes::HOSTILE) && (!closest_entity->dead))
                    {
                        //cout << "Range of unit " << i << ": " << abs((units[i].get()->getGlobalPosition().x) - closest_entity->getGlobalPosition().x) - 110 << endl;
                        //cout << "Dest local x: " << units[i].get()->dest_local_x << endl;

                        if (abs((units[i].get()->global_x) - (closest_entity->getGlobalPosition().x + closest_entity->hitboxes[0].o_x)) > 900)
                            inRange = false;
                    }
                }
            }

            //cout << "In range: " << inRange << endl;
        }

        /** Draw the units **/
        for (int i = 0; i < units.size(); i++)
        {
            PlayableUnit* unit = units[i].get();

            if (closest_entity_id != -1)
            {
                Entity* closest_entity = tangibleLevelObjects[closest_entity_id].get();

                unit->entity_distance = abs((unit->global_x) - (closest_entity->getGlobalPosition().x + (closest_entity->hitboxes[0].o_x - closest_entity->hitboxes[0].o_width / 2)));
                //cout << "Distance to nearest entity for unit " << i << ": " << unit->entity_distance << " (" << unit->global_x << " " << closest_entity->getGlobalPosition().x << " " << closest_entity->hitboxes[0].o_x << ")" << endl;

                if ((closest_entity->entityType == Entity::EntityTypes::HOSTILE) && (!closest_entity->dead) && (inRange) && (closest_entity->getAnimationSegment() != "death"))
                {
                    unit->enemy_in_range = true;
                    //cout << "Unit " << i << " distance to closest enemy is " << unit->entity_distance << " pixels" << endl;
                } else
                {
                    unit->enemy_in_range = false;
                    unit->dest_local_x = 0;
                }
            } else
            {
                unit->enemy_in_range = false;
            }

            if (unit->getUnitID() != 0)
            {
                float unit_distance = 9999;

                for (int a = 0; a < units.size(); a++)
                {
                    if (a != i)
                    {
                        if (units[a].get()->getUnitID() == 1)
                        {
                            float gx = units[a].get()->getGlobalPosition().x;
                            float my_gx = unit->getGlobalPosition().x;

                            float dis = abs(gx - my_gx);

                            if (dis < unit_distance)
                                unit_distance = dis;
                        }
                    }
                }

                unit->unit_distance = unit_distance;

                //cout << "Unit " << i << " distance to another unit is " << unit_distance << " pixels" << endl;
            }

            /** Verify if Hatapon exists **/
            if (unit->getUnitID() == 0)
            {
                if (unit->getUnitHP() > 0)
                    hatapon = true;
            }

            /** Execute unit features when mission is not over **/
            if (!missionEnd)
            {
                RhythmController* rhythmController = CoreManager::getInstance().getRhythmController();

                //TO-DO: replace with rhythm callbacks!!!!!
                //unit->UpdateRhythm(rhythm->current_song, rhythmController->current_drum, rhythm->GetCombo());
                //unit->doRhythm(rhythm->current_song, rhythmController->current_drum, rhythm->GetCombo());

                if (unit->getUnitID() != 0) /// Yaripon
                {
                    if (unit->doAttack())
                    {
                        SPDLOG_DEBUG("Unit {} threw a spear!", i);

                        float rand_hs = (rand() % 1000) / float(10);
                        float rand_vs = (rand() % 1000) / float(10);

                        float rand_rad = (rand() % 200000000) / float(1000000000);

                        //TO-DO: should this even be here?
                        int rhythm_acc = rhythm->current_perfect; ///Check how many perfect measures has been done to improve the spears throwing
                        float fever_boost = 0.8;

                        if (rhythm->GetCombo() >= 11) ///Check for fever to boost the spears damage
                            fever_boost = 1.0;

                        float mindmg = unit->mindmg * (0.8 + (rhythm_acc * 0.05)) * fever_boost;
                        float maxdmg = unit->maxdmg * (0.8 + (rhythm_acc * 0.05)) * fever_boost;

                        if (unit->defend)
                        {
                            mindmg = mindmg * 0.75;
                            maxdmg = maxdmg * 0.75;

                            if (unit->charged)
                            {
                                mindmg = mindmg * 1.666;
                                maxdmg = maxdmg * 1.666;
                            }
                        } else
                        {
                            if (unit->charged)
                            {
                                mindmg = mindmg * 2.2;
                                maxdmg = maxdmg * 2.2;
                            }
                        }

                        ///Make the spears be thrown with worse velocity when player is drumming bad (10% punishment)
                        rand_hs *= 0.9 + (rhythm_acc * 0.025);
                        rand_vs *= 0.9 + (rhythm_acc * 0.025);

                        ///This way, the lowest damage is dmg * 0.64 (36% punishment) and highest damage is 100% of the values

                        float xpos = unit->getGlobalPosition().x + unit->hitBox.left + unit->hitBox.width / 2;
                        float ypos = unit->getGlobalPosition().y + unit->hitBox.top + unit->hitBox.height / 2;

                        float vspeed = -450 + rand_vs;

                        if (unit->defend)
                        {
                            if (unit->isFever)
                                vspeed = fabs(vspeed) - 250;
                        }

                        vector<Object>* obj;
                        obj = unit->objects.get();

                        spawnProjectile((*obj)[1].s_obj, xpos, ypos, 800, 450 + rand_hs, vspeed, -0.58 - rand_rad, maxdmg, mindmg, 0);
                    }
                }
            }

            if (missionEnd)
            {
                if (!failure)
                    unit->doMissionEnd();
            }

            unit->fps = fps;
            unit->Update();
            unit->Draw();

            if (unit->ready_to_erase)
                units_rm.push_back(i);
        }
    }

    /** Fail the mission if Hatapon is dead or when Hatapon is the only unit remaining **/

    if ((!hatapon) || ((hatapon) && (units.size() <= 1)))
    {
        missionEnd = true;
        failure = true;
        rhythm->Stop();
    }

    return units_rm;
}

void MissionController::Update(sf::RenderWindow& window, float cfps, InputController& inputCtrl)
{
}

void MissionController::Update()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    InputController* inputCtrl = CoreManager::getInstance().getInputController();
    Rhythm* rhythm = CoreManager::getInstance().getRhythm();

    float cfps = CoreManager::getInstance().getCore()->getFPS();

    SPDLOG_TRACE("Start of MissionController update routine (NEW FRAME)");

    ///remove stopped sounds
    for (int i = projectile_sounds.size() - 1; i > 0; i--)
    {
        if (projectile_sounds[i].getStatus() == sf::Sound::Status::Stopped)
        {
            projectile_sounds.erase(projectile_sounds.begin() + i);
        } else
        {
            break;
        }
    }

    SPDLOG_TRACE("Sort tangibleLevelObjects to prioritize rendering layers");

    ///Sort tangibleLevelObjects to prioritize rendering layers
    std::sort(tangibleLevelObjects.begin(), tangibleLevelObjects.end(),
              [](const std::unique_ptr<Entity>& a, const std::unique_ptr<Entity>& b) {
                  return a.get()->layer < b.get()->layer;
              });

    ///Globally disable the controls when Dialogbox is opened, but preserve original controller for controlling the DialogBoxes later
    
    // TO-DO: what to do with this section? still needed?
    /*
    InputController o_inputCtrl;
    InputController cur_inputCtrl;

    cur_inputCtrl = inputCtrl;

    if (dialog_boxes.size() > 0)
    {
        o_inputCtrl = inputCtrl;

        InputController a;
        cur_inputCtrl = a;
    }*/

    /** Update loop, everything here happens per each frame of the game **/
    fps = cfps;

    /** Apply the keyMap from parent class **/

    /** Execute camera and background **/
    
    SPDLOG_TRACE("Handle camera and background");
    camera.missionEnd = missionEnd; ///disable camera controls when needed
    camera.Work();

    mission_bg.setCamera(camera);
    mission_bg.Draw();

    /** Execute Keyboard events and Movement **/
    
    SPDLOG_TRACE("Handle input and movement");
    DoKeyboardEvents();
    DoMovement();

    vector<int> k_e;

    SPDLOG_TRACE("Handle cloneable entities");
    /** Spawn cloneable entities **/
    for (int i = 0; i < kirajins.size(); i++)
    {
        Kirajin_Yari_2* entity = kirajins[i].get();

        ///Check if the kirajin should respawn or not
        bool respawn = true;

        for (int t = 0; t < tangibleLevelObjects.size(); t++)
        {
            if (tangibleLevelObjects[t].get()->entityID == entity->entityID)
            {
                if (tangibleLevelObjects[t].get()->cloneable)
                {
                    ///check if unit is already spawned
                    if (tangibleLevelObjects[t].get()->spawnOrderID == entity->spawnOrderID)
                        respawn = false;
                }
            }
        }

        ///Check if entity's parent is still alive, if not, kill the entity
        int parent = entity->parent;
        bool dead_parent = false;

        ///Check if parent is defined
        if (parent != -1)
        {
            ///Look for parent
            auto it = find_if(tangibleLevelObjects.begin(), tangibleLevelObjects.end(), [&parent](const std::unique_ptr<Entity>& obj) { return obj.get()->spawnOrderID == parent; });

            if (it != tangibleLevelObjects.end())
            {
                ///Parent has been found!

                auto index = std::distance(tangibleLevelObjects.begin(), it);
                //cout << "My parent is currently residing at " << index << endl;

                ///Check if it's dead
                if (tangibleLevelObjects[index].get()->dead)
                {
                    ///Kill the entity
                    dead_parent = true;
                }
            } else ///Parent can't be found
            {
                ///Kill the entity
                dead_parent = true;
            }
        }

        /** Find the farthest unit in your army (for calculations) **/
        const auto& farthestUnit = *std::min_element(units.begin(),
                                                     units.end(),
                                                     less_by([](const std::unique_ptr<PlayableUnit>& unit) {
                                                         return unit->getGlobalPosition().x;
                                                     }));
        float temp_pos = farthestUnit->getGlobalPosition().x;

        entity->distance_to_unit = abs(temp_pos - entity->getGlobalPosition().x);

        if (entity->distance_to_unit <= 1000)
        {
            if (!dead_parent)
            {
                if (respawn)
                {
                    if (entity->respawn_clock.getElapsedTime().asSeconds() > entity->respawnTime)
                    {
                        entity->respawn_clock.restart();

                        tangibleLevelObjects.push_back(make_unique<Kirajin_Yari_2>(*entity));
                    }
                }
            } else
            {
                ///remove this specific kirajin
                k_e.push_back(i);
            }
        }
    }

    for (int i = 0; i < k_e.size(); i++)
    {
        kirajins.erase(kirajins.begin() + k_e[i] - i);
    }

    /** Draw all Entities **/
    SPDLOG_TRACE("Draw all entities");
    vector<int> tlo_rm = DrawEntities();

    /** Draw all Units **/
    SPDLOG_TRACE("Draw all units");
    vector<int> units_rm = DrawUnits();

    /** Draw projectiles **/
    SPDLOG_TRACE("Draw all projectiles");
    vector<int> pr_rm = DrawProjectiles();

    /** Draw message clouds **/
    SPDLOG_TRACE("Draw all message clouds");
    for (int e = 0; e < tangibleLevelObjects.size(); e++)
    {
        //TO-DO: handle messages with new system
        //Entity* entity = tangibleLevelObjects[e].get();
        //entity->doMessages(window, fps, inputCtrl);
    }

    /** Draw hitboxes **/

    if (showHitboxes)
    {
        SPDLOG_TRACE("Draw all hitboxes");
        //TO-DO: hitboxes later bc im lazy and its a debug setting
        //DrawHitboxes(window);
    }

    /** Draw damage counters **/
    SPDLOG_TRACE("Draw all damage counters");
    vector<int> dmg_rm = DrawDamageCounters();

    /**  Draw static UI elements **/

    auto lastView = window->getView();
    window->setView(window->getDefaultView());

    /**

	if(cutscenesLeft && !inCutscene && isMoreCutscenes())
	{
		StartCutscene(cutscene_text_identifiers[currentCutsceneId],cutscene_blackscreens[currentCutsceneId],cutscene_lengths[currentCutsceneId]);
	}

	sf::Time currentTime = timer.getElapsedTime();
	int currentAlpha = startAlpha;
	if (currentTime >= targetTime && inCutscene)
	{
		// oops: currentAlpha = endAlpha; // make certain that the alpha is at its final destination
		//you are done
		if(!isMoreCutscenes())
		{
			currentAlpha = startAlpha;
			inCutscene = false;
			if(isBlackScreenCutscene)
			{
				inFadeTransition = true;
				timer.restart();
				targetTime = sf::seconds(2);
			}
			else
			{
				FinishLastCutscene();
			}
			cutscenesLeft=false;
		}
		else
		{
			/// next cutscene
			currentCutsceneId++;
			StartCutscene(cutscene_text_identifiers[currentCutsceneId],cutscene_blackscreens[currentCutsceneId],cutscene_lengths[currentCutsceneId]);
		}
	}
	else if (currentTime >= targetTime && !inCutscene && inFadeTransition)
	{
		currentAlpha = endAlpha;
		inFadeTransition = false;
		FinishLastCutscene();
	}
	else if (!inCutscene && inFadeTransition)
	{
		currentAlpha = startAlpha + (endAlpha - startAlpha) * (currentTime.asMilliseconds() / (targetTime.asMilliseconds()+0.0));
	}
	else if (inCutscene && isBlackScreenCutscene)
	{
		currentAlpha = startAlpha;
	}
	else if (inCutscene)
	{
		currentAlpha = startAlpha;
	}
	// apply alpha to whatever colour is previously set
	if((inFadeTransition || inCutscene) && isBlackScreenCutscene)
	{
		sf::Color fadeColor = fade.getFillColor();
		fadeColor.a = currentAlpha;
		fade.setFillColor(fadeColor);
		fade.setSize(sf::Vector2f(window->getSize().x,window->getSize().y));

		fade.setPosition(0,0);
		window->draw(fade);
	}
	if (inCutscene)
	{
		for (int i=0; i<t_cutscene_text.size(); i++)
		{
			sf::Text currentLine = t_cutscene_text[i];

			currentLine.setPosition(window->getSize().x/2,300 + 39*i);
			sf::Time currentTime = timer.getElapsedTime();

			window->draw(currentLine);
		}
	}*/

    /** Draw the timer (Static UI element) **/

    if (showTimer)
    {
        t_timerMenu.setString(Func::ConvertToUtf8String(std::to_string(missionTimer.getElapsedTime().asSeconds()) + " Seconds"));
        t_timerMenu.setOrigin(t_timerMenu.getGlobalBounds().width / 2, t_timerMenu.getGlobalBounds().height / 2);
        t_timerMenu.setPosition(window->getSize().x / 2, 100);
        window->draw(t_timerMenu);
    }

    /** Draw floor **/

    float resRatioX = window->getSize().x / float(1280);
    float resRatioY = window->getSize().y / float(720);
    r_floor.setSize(sf::Vector2f(1280 * resRatioX, 110 * resRatioY));
    r_floor.setFillColor(sf::Color::Black);
    r_floor.setPosition(0, 610 * resRatioY);
    window->draw(r_floor);

    SPDLOG_TRACE("Draw UI (user interface)");
    //TO-DO: draw user interface (update to new system)
    /*
    drawCommandList(window);
    DrawUnitThumbs(window);
    DrawPickedItems(window);
    */

    /** If mission isn't finished, execute and draw Rhythm **/

    if (!missionEnd)
    {
        //ctrlTips.x = 0;
        //ctrlTips.y = (720-ctrlTips.ySize);
        //ctrlTips.draw(window);

        //cout << "[MissionController] Rhythm" << endl;

        CoreManager::getInstance().getRhythmGUI()->doVisuals(0, rhythm->GetCombo());
        //rhythm->Draw();
    }

    /** Execute all mission end related things **/

    SPDLOG_TRACE("Handle mission ending");
    DoMissionEnd();

    inputCtrl->lockRhythm = false;

    SPDLOG_TRACE("Handle dialog boxes");
    if (dialog_boxes.size() > 0)
    {
        inputCtrl->lockRhythm = true;

        if (inputCtrl->isKeyPressed(Input::Keys::CROSS))
        {
            switch (dialog_boxes[dialog_boxes.size() - 1].CheckSelectedOption())
            {
                case 0: {
                    if (dialog_boxes[dialog_boxes.size() - 1].id == 0)
                    {
                        SPDLOG_DEBUG("Return to Patapolis");
                        dialog_boxes[dialog_boxes.size() - 1].Close();

                        missionEnd = true;
                        failure = true;
                        rhythm->Stop();
                    } else if (dialog_boxes[dialog_boxes.size() - 1].id == 999)
                    {
                        SPDLOG_DEBUG("Enable hitboxes");
                        showHitboxes = true;
                        dialog_boxes[dialog_boxes.size() - 1].Close();
                    }

                    break;
                }

                case 1: {
                    if (dialog_boxes[dialog_boxes.size() - 1].id == 0)
                    {
                        SPDLOG_DEBUG("Back to mission");
                        dialog_boxes[dialog_boxes.size() - 1].Close();
                    } else if (dialog_boxes[dialog_boxes.size() - 1].id == 999)
                    {
                        SPDLOG_DEBUG("Disable hitboxes");
                        showHitboxes = false;
                        dialog_boxes[dialog_boxes.size() - 1].Close();
                    }

                    break;
                }

                case 2: {
                    if (dialog_boxes[dialog_boxes.size() - 1].id == 999)
                    {
                        SPDLOG_DEBUG("Heal all units");
                        for (int u = 0; u < units.size(); u++)
                        {
                            units[u].get()->current_hp = units[u].get()->max_hp;
                        }

                        dialog_boxes[dialog_boxes.size() - 1].Close();
                    }

                    break;
                }

                case 3: {
                    if (dialog_boxes[dialog_boxes.size() - 1].id == 999)
                    {
                        SPDLOG_DEBUG("Kill all player units");
                        for (int u = 0; u < units.size(); u++)
                        {
                            if (units[u].get()->getUnitID() == 1)
                            units[u].get()->current_hp = 0;
                        }

                        dialog_boxes[dialog_boxes.size() - 1].Close();
                    }

                    break;
                }

                case 4: {
                    if (dialog_boxes[dialog_boxes.size() - 1].id == 999)
                    {
                        SPDLOG_DEBUG("Kill Hatapon");
                        for (int u = 0; u < units.size(); u++)
                        {
                            if (units[u].get()->getUnitID() == 0)
                                units[u].get()->current_hp = 0;
                        }

                        dialog_boxes[dialog_boxes.size() - 1].Close();
                    }

                    break;
                }

                case 5: {
                    if (dialog_boxes[dialog_boxes.size() - 1].id == 999)
                    {
                        SPDLOG_DEBUG("Deprecated: verbose logging");
                        verboseLogs = !verboseLogs;
                        dialog_boxes[dialog_boxes.size() - 1].Close();
                    }

                    break;
                }

                case 6: {
                    if (dialog_boxes[dialog_boxes.size() - 1].id == 999)
                    {
                        SPDLOG_DEBUG("Mission complete");

                        missionEnd = true;
                        rhythm->Stop();
                        missionEndTimer.restart();

                        dialog_boxes[dialog_boxes.size() - 1].Close();
                    }

                    break;
                }

                case 7: {
                    if (dialog_boxes[dialog_boxes.size() - 1].id == 999)
                    {
                        SPDLOG_DEBUG("Toggle rhythm debug");

                        CoreManager::getInstance().getRhythmGUI()->toggleDebugUI();
                        
                        dialog_boxes[dialog_boxes.size() - 1].Close();
                    }

                    break;
                }

                case 8: {
                    if (dialog_boxes[dialog_boxes.size() - 1].id == 999)
                    {
                        SPDLOG_DEBUG("Toggle song debug");

                        CoreManager::getInstance().getRhythm()->toggleDebug();
                        
                        dialog_boxes[dialog_boxes.size() - 1].Close();
                    }

                    break;
                }
            }
        }
    }

    vector<int> db_e; ///dialog box erase

    for (int i = 0; i < dialog_boxes.size(); i++)
    {
        dialog_boxes[i].x = 640;
        dialog_boxes[i].y = 360;
        dialog_boxes[i].Draw();

        if (dialog_boxes[i].closed)
            db_e.push_back(i);
    }

    for (int i = 0; i < db_e.size(); i++)
    {
        dialog_boxes.erase(dialog_boxes.begin() + db_e[i] - i);
    }

    window->setView(lastView);

    /** Remove vector objects that are no longer in use **/
    SPDLOG_TRACE("Perform vector cleanup");
    DoVectorCleanup(units_rm, dmg_rm, tlo_rm, pr_rm);

    SPDLOG_TRACE("Finished drawing the frame.");

    if (returnToPatapolis)
    {
        StateManager::getInstance().setState(StateManager::PATAPOLIS);
    }
}
void MissionController::FinishLastCutscene()
{
    /// runs when the last cutscene of a sequence is done
}
bool MissionController::isMoreCutscenes()
{
    /// returns true if there are more cutscenes
    return currentCutsceneId < cutscene_text_identifiers.size() - 1;
}
void MissionController::StartCutscene(const std::string& text, bool isBlackScreen, int TimeToShow)
{
    /// because the description needs to be able to go over multiple lines, we have to split it into a series of lines
    t_cutscene_text.clear();
    std::vector<std::string> wordsinDesc = Func::Split(CoreManager::getInstance().getStrRepo()->GetString(text), ' ');
    sf::String oldTotalString;
    sf::String currentTotalString;
    int maxWidth = CoreManager::getInstance().getConfig()->GetInt("resX") * 0.4;
    timer.restart();
    inCutscene = true;
    isBlackScreenCutscene = isBlackScreen;
    targetTime = sf::seconds(TimeToShow);
    /// we split it into words, then go word by word testing the width of the string
    for (int i = 0; i < wordsinDesc.size(); i++)
    {
        std::string currentWord = wordsinDesc[i];
        currentTotalString = currentTotalString + Func::ConvertToUtf8String(currentWord) + " ";
        sf::Text t_newLine;
        t_newLine.setFont(f_font);
        t_newLine.setCharacterSize(35);
        t_newLine.setFillColor(sf::Color::White);
        t_newLine.setString(currentTotalString);
        if (t_newLine.getGlobalBounds().width > maxWidth)
        {
            /// when the string is too long, we go back to the last string and lock it in, then start a new line
            currentTotalString = oldTotalString;
            t_newLine.setString(currentTotalString);
            t_newLine.setOrigin(t_newLine.getGlobalBounds().width / 2, t_newLine.getGlobalBounds().height / 2);
            t_cutscene_text.push_back(t_newLine);
            oldTotalString = currentWord + " ";
            currentTotalString = currentWord + " ";
        }
        oldTotalString = currentTotalString;
        /// if there are no more words, finish up the current line
        if (i + 1 == wordsinDesc.size())
        {
            currentTotalString = oldTotalString;
            t_newLine.setString(currentTotalString);
            t_newLine.setOrigin(t_newLine.getGlobalBounds().width / 2, t_newLine.getGlobalBounds().height / 2);
            t_cutscene_text.push_back(t_newLine);
            oldTotalString = "";
            currentTotalString = "";
        }
    }
}
MissionController::~MissionController()
{
    //dtor
    for (auto& t : missionThreads)
    {
        t.detach();
    }
}
