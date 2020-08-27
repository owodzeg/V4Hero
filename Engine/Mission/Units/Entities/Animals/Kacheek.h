#ifndef KACHEEK_H
#define KACHEEK_H

#include <SFML/Graphics.hpp>
#include "../../../../Config.h"
#include "../../Entity.h"

using namespace std;

class Kacheek : public Entity
{
    public:
    bool run = false;
    float walk_time = 2;

    sf::Clock walk_timer;
    sf::Clock death_timer;

    float tumble_x=0, tumble_y=0, tumble_angle=0;
    float tumble_xdest=0, tumble_ydest=0, tumble_angledest=0;

    Kacheek();
    void LoadConfig(Config *thisConfigs);
    void Draw(sf::RenderWindow& window);
    void OnCollide(CollidableObject* otherObject, int collidedWith=-1, vector<string> collisionData = {});
};

#endif // KACHEEK_H
