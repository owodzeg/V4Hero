#ifndef TREASURECHEST_H
#define TREASURECHEST_H

#include <SFML/Graphics.hpp>
#include "../../../../Config.h"
#include "../../Entity.h"

using namespace std;

class TreasureChest : public Entity
{
    public:
    bool droppeditem = false;
    sf::Clock death_timer;

    sf::SoundBuffer s_broken;

    TreasureChest();
    void LoadConfig(Config *thisConfigs);
    void parseAdditionalData(nlohmann::json additional_data);
    void Draw(sf::RenderWindow& window);
    void OnCollide(CollidableObject* otherObject, int collidedWith=-1, vector<string> collisionData = {});
};

#endif // TREASURECHEST_H
