#ifndef KIRAJINPOWEREDTOWERSMALL_H
#define KIRAJINPOWEREDTOWERSMALL_H

#include "../../Entity.h"
#include <SFML/Graphics.hpp>

class KirajinPoweredTowerSmall : public Entity
{
public:
    bool droppeditem = false;
    float shake = 0;
    bool negative = false;
    sf::Clock death_timer;
    sf::Clock shake_timer;
    float shake_delay = 80;

    sf::SoundBuffer s_broken;

    KirajinPoweredTowerSmall();
    void LoadConfig();
    void parseAdditionalData(nlohmann::json additional_data);
    void Draw();
    void OnCollide(CollidableObject* otherObject, int collidedWith = -1, vector<string> collisionData = {});
};

#endif // KIRAJINPOWEREDTOWERSMALL_H
