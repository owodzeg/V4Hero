#include "Entity.h"

#include "../../CoreManager.h"

Entity::Entity()
{
    force_drop_item.push_back(0);
}

void Entity::setEntityID(int new_entityID)
{
    entityID = new_entityID;
    AnimatedObject::entityID = new_entityID;
}

int Entity::getEntityID()
{
    return entityID;
}

bool Entity::willDrop(vector<int> item_id)
{
    for (int i = 0; i < loot_table.size(); i++)
    {
        if (loot_table[i].order_id == item_id)
        {
            return true;
        }
    }
    return false;
}

void Entity::doRhythm(std::string current_song, std::string current_drum, int combo, int realcombo, bool advanced_prefever, float beatBounce, float satisfaction)
{
    cout << "Entity::doRhythm() was not overriden by child class" << endl;
}

bool Entity::doAttack()
{
    //cout << "Entity::doAttack() was not overriden by child class" << endl;
    return false;
}

void Entity::doMessages(sf::RenderWindow& window, float fps, InputController& inputCtrl)
{
    //cout << "Entity::doMessages() was not overriden by child class" << endl;
}

void Entity::die()
{
    ///Kills the entity, if entity has a method scripted
    ///Made so it can be called outside of the entity function and do the same work
}

void Entity::LoadConfig(std::string unitParamPath)
{
    cout << "Entity::LoadConfig() was not overriden by child class" << endl;
}

void Entity::parseAdditionalData(nlohmann::json additional_data)
{
    cout << "Entity::parseAdditionalData() was not overriden by child class" << endl;
}

void Entity::dropItem()
{
    if (!dropped_item)
    {
        if (force_drop)
        {
            std::vector<int> force_drop_id = CoreManager::getInstance().getSaveReader()->itemReg.getItemByName(force_drop_item)->order_id;

            ///check if there are level requirements
            if (force_drop_mission_lvl != 0)
            {
                ///specific mission level is required, compare it
                if (CoreManager::getInstance().getSaveReader()->mission_levels[CoreManager::getInstance().getMissionController()->curMissionID] == force_drop_mission_lvl)
                {
                    ///check if the item was obtained, if not, force drop it
                    if (!CoreManager::getInstance().getSaveReader()->invData.checkItemObtained(force_drop_id))
                    {
                        Loot tmp;
                        tmp.order_id = force_drop_id;
                        // check if the item is scheduled to drop anyway
                        if (willDrop(force_drop_id))
                        {
                            loot_table.push_back(tmp);
                        }
                    }
                }
            } else
            {
                ///there are no level requirements, just drop the item if its not obtained yet
                if (!CoreManager::getInstance().getSaveReader()->invData.checkItemObtained(force_drop_id))
                {
                    Loot tmp;
                    tmp.order_id = force_drop_id;
                    // only add to loot if the item isn't scheduled to drop
                    if (!willDrop(force_drop_id))
                    {
                        loot_table.push_back(tmp);
                    }
                }
            }
        }

        for (int i = 0; i < loot_table.size(); i++)
        {
            vector<int> cur_id = loot_table[i].order_id;

            auto item = CoreManager::getInstance().getSaveReader()->itemReg.getItemByID(loot_table[i].order_id);
            string id_out = to_string(cur_id[0]);
            for (int o = 1; o < cur_id.size(); o++)
            {
                id_out += ":" + to_string(cur_id[o]);
            }

            nlohmann::json data;
            data["spritesheet"] = item->spritesheet;
            data["spritesheet_id"] = item->spritesheet_id;
            data["picked_item"] = id_out;

            CoreManager::getInstance().getMissionController()->spawnEntity(5, false, false, getGlobalPosition().x + hitboxes[0].o_x + (hitboxes[0].o_width / 2), 0, false, 10, 100, 1, 1, 1, 0, getGlobalPosition().y + hitboxes[0].o_y + (hitboxes[0].o_height / 2) - 60, 0, sf::Color::White, 9999, -1, {}, data);
        }

        dropped_item = true;
    }
}
