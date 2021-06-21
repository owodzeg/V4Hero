#ifndef GRASS1_H
#define GRASS1_H

#include "../../../../Config.h"
#include "../../Entity.h"
#include <SFML/Graphics.hpp>

using namespace std;

class Grass1 : public Entity
{
public:
    Grass1();
    void LoadConfig(Config* thisConfigs);
    void Draw(sf::RenderWindow& window);
    void OnCollide(CollidableObject* otherObject, int collidedWith = -1, vector<string> collisionData = {});
};

#endif // GRASS1_H
