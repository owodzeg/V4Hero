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
    virtual void OnCollide(CollidableObject* otherObject, int collidedWith = -1, std::vector<std::string> collisionData = {});
};

#endif // COLLIDABLEOBJECT_H
