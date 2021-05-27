#ifndef ROCKBIG_H
#define ROCKBIG_H

#include <SFML/Graphics.hpp>
#include "../../../../Config.h"
#include "../../Entity.h"

using namespace std;

class RockBig : public Entity
{
    public:
    bool droppeditem = false;
    sf::Clock death_timer;

    sf::SoundBuffer s_broken;

    RockBig();
    void LoadConfig(Config *thisConfigs);
    void parseAdditionalData(std::vector<std::string> additional_data);
    void Draw(sf::RenderWindow& window);
    void OnCollide(CollidableObject* otherObject, int collidedWith=-1, vector<string> collisionData = {});
};

#endif // ROCKBIG_H
