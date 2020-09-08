#ifndef ROCKSMALL_H
#define ROCKSMALL_H

#include <SFML/Graphics.hpp>
#include "../../../../Config.h"
#include "../../Entity.h"

using namespace std;

class RockSmall : public Entity
{
    public:
    bool droppeditem = false;
    sf::Clock death_timer;

    RockSmall();
    void LoadConfig(Config *thisConfigs);
    void Draw(sf::RenderWindow& window);
    void OnCollide(CollidableObject* otherObject, int collidedWith=-1, vector<string> collisionData = {});
};

#endif // ROCKSMALL_H
