#ifndef WOODENSPIKES_H
#define WOODENSPIKES_H

#include <SFML/Graphics.hpp>
#include "../../../../Config.h"
#include "../../Entity.h"

using namespace std;

class WoodenSpikes : public Entity
{
    public:
    bool dead = false;
    bool droppeditem = false;
    sf::Clock death_timer;

    WoodenSpikes();
    void LoadConfig(Config *thisConfigs);
    void Draw(sf::RenderWindow& window);
    void OnCollide(CollidableObject* otherObject, int collidedWith=-1, vector<string> collisionData = {});
};

#endif // WOODENSPIKES_H
