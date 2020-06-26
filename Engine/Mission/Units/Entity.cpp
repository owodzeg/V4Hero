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
