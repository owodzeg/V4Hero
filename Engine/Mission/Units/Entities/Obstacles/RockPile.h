#ifndef ROCKPILE_H
#define ROCKPILE_H

#include <SFML/Graphics.hpp>
#include "../../../../Config.h"
#include "../../Entity.h"

using namespace std;

class RockPile : public Entity
{
    public:
    bool dead = false;
    bool droppeditem = false;
    sf::Clock death_timer;

    RockPile();
    void LoadConfig(Config *thisConfigs);
    void Draw(sf::RenderWindow& window);
    void OnCollide(CollidableObject* otherObject, int collidedWith=-1, vector<string> collisionData = {});
};

#endif // WOODENSPIKES_H
