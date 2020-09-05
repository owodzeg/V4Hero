#ifndef ROCKBIG_H
#define ROCKBIG_H

#include <SFML/Graphics.hpp>
#include "../../../../Config.h"
#include "../../Entity.h"

using namespace std;

class RockBig : public Entity
{
    public:
    bool dead = false;
    bool droppeditem = false;
    sf::Clock death_timer;

    RockBig();
    void LoadConfig(Config *thisConfigs);
    void Draw(sf::RenderWindow& window);
    void OnCollide(CollidableObject* otherObject, int collidedWith=-1, vector<string> collisionData = {});
};

#endif // ROCKBIG_H
