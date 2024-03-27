#ifndef ROCKSMALL_H
#define ROCKSMALL_H

#include "../../Entity.h"
#include <SFML/Graphics.hpp>

class RockSmall : public Entity
{
public:
    bool droppeditem = false;
    sf::Clock death_timer;

    sf::SoundBuffer s_broken;

    RockSmall();
    void LoadConfig();
    void parseAdditionalData(nlohmann::json additional_data);
    void Draw();
    void OnCollide(CollidableObject* otherObject, int collidedWith = -1, vector<string> collisionData = {});
};

#endif // ROCKSMALL_H
