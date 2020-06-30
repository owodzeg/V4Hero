#ifndef GRASS2_H
#define GRASS2_H

#include <SFML/Graphics.hpp>
#include "../../../Config.h"
#include "../Entity.h"

using namespace std;

class Grass2 : public Entity
{
    public:
    Grass2();
    void LoadConfig(Config *thisConfigs);
    void Draw(sf::RenderWindow& window);
    void OnCollide(CollidableObject* otherObject, int collidedWith=-1, vector<string> collisionData = {});
};

#endif // GRASS2_H
