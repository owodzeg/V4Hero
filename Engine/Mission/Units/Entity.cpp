#include "Entity.h"

Entity::Entity()
{

}

void Entity::setEntityID(int new_entityID)
{
    entityID = new_entityID;
}

int Entity::getEntityID()
{
    return entityID;
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

void Entity::die()
{
    ///Kills the entity, if entity has a method scripted
    ///Made so it can be called outside of the entity function and do the same work
}

void Entity::LoadConfig(Config *thisConfigs, std::string unitParamPath)
{
    cout << "Entity::LoadConfig() was not overriden by child class" << endl;
}
