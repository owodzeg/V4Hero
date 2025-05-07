#ifndef COLLIDABLEOBJECT_H
#define COLLIDABLEOBJECT_H

#include "../../Config.h"
#include "AnimatedObject.h"
#include <SFML/Graphics.hpp>

class CollidableObject : public AnimatedObject
{
public:
    bool isCollidable = true;
    bool isAttackable = true;

    CollidableObject();
};

#endif // COLLIDABLEOBJECT_H
