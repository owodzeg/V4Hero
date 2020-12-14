#ifndef RAPPATA_H
#define RAPPATA_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../../../../Config.h"
#include "../../Entity.h"

using namespace std;

class Rappata : public Entity
{
    public:
    bool run = false;
    float walk_time = 2;

    sf::Clock walk_timer;
    sf::Clock death_timer;
    sf::Clock react_timer; ///giving kacheek time to reevaluate it's life

    float react_time = 2;
    //float loc_x=0, loc_y=0;
    float vspeed=0,hspeed=0;
    float gravity = 978;
    int jump_mode = 0;

    sf::SoundBuffer s_startle, s_dead;

    Rappata();
    void LoadConfig(Config *thisConfigs);
    void parseAdditionalData(std::vector<std::string> additional_data);
    void Draw(sf::RenderWindow& window);
    void OnCollide(CollidableObject* otherObject, int collidedWith=-1, vector<string> collisionData = {});
};

#endif // RAPPATA_H
