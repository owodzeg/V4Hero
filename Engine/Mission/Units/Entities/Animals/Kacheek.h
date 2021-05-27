#ifndef KACHEEK_H
#define KACHEEK_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
    sf::Clock react_timer; ///giving kacheek time to reevaluate it's life

    float react_time = 2;

    float tumble_x=0, tumble_y=0, tumble_angle=0;
    float tumble_xdest=0, tumble_ydest=0, tumble_angledest=0;

    sf::SoundBuffer s_startle, s_dead;

    Kacheek();
    void LoadConfig(Config *thisConfigs);
    void parseAdditionalData(std::vector<std::string> additional_data);
    void Draw(sf::RenderWindow& window);
    void OnCollide(CollidableObject* otherObject, int collidedWith=-1, vector<string> collisionData = {});
};

#endif // KACHEEK_H
