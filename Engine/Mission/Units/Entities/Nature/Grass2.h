#ifndef GRASS2_H
#define GRASS2_H

#include "../../../../Config.h"
#include "../../Entity.h"
#include <SFML/Graphics.hpp>

using namespace std;

class Grass2 : public Entity
{
public:
    Grass2();
    void LoadConfig();
    void Draw(sf::RenderWindow& window);
    void OnCollide(CollidableObject* otherObject, int collidedWith = -1, vector<string> collisionData = {});
};

#endif // GRASS2_H
