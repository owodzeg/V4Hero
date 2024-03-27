#ifndef ROCKBIG_H
#define ROCKBIG_H

#include "../../Entity.h"
#include <SFML/Graphics.hpp>

class RockBig : public Entity
{
public:
    bool droppeditem = false;
    sf::Clock death_timer;

    sf::SoundBuffer s_broken;

    RockBig();
    void LoadConfig();
    void parseAdditionalData(nlohmann::json additional_data);
    void Draw();
    void OnCollide(CollidableObject* otherObject, int collidedWith = -1, vector<string> collisionData = {});
};

#endif // ROCKBIG_H
