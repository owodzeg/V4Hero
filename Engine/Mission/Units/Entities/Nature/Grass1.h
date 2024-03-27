#ifndef GRASS1_H
#define GRASS1_H

#include "../../Entity.h"
#include <SFML/Graphics.hpp>

class Grass1 : public Entity
{
public:
    Grass1();
    void LoadConfig();
    void Draw();
    void OnCollide(CollidableObject* otherObject, int collidedWith = -1, vector<string> collisionData = {});
};

#endif // GRASS1_H
