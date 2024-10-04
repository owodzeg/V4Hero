#include "Entity.h"

#include "../../CoreManager.h"

Entity::Entity()
{
    force_drop_item.push_back(0);
}

void Entity::setEntityID(int new_entityID)
{
    entityID = new_entityID;
    //AnimatedObject::entityID = new_entityID;
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
    SPDLOG_WARN("Entity::doRhythm() was not overriden by child class");
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
    SPDLOG_WARN("Entity::LoadConfig() was not overriden by child class");
}

void Entity::parseAdditionalData(nlohmann::json additional_data)
{
    SPDLOG_WARN("Entity::parseAdditionalData() was not overriden by child class");
}

void Entity::dropItem()
{

}
