#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include "../../Config.h"
#include "CollidableObject.h"

class Entity : public CollidableObject
{
    public:
    int entityID = -1;

    Entity();
    virtual void setEntityID(int new_entityID);
    virtual int getEntityID();
};

#endif // ENTITY_H
