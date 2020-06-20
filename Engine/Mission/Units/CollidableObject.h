#ifndef COLLIDABLEOBJECT_H
#define COLLIDABLEOBJECT_H

#include <SFML/Graphics.hpp>
#include "../../Config.h"
#include "AnimatedObject.h"


using namespace std;

class CollidableObject : public AnimatedObject
{
    public:

    CollidableObject();
    virtual void OnCollide(CollidableObject* otherObject, int collidedWith=-1, vector<string> collisionData = {});
};

#endif // COLLIDABLEOBJECT_H
