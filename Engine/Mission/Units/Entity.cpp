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
