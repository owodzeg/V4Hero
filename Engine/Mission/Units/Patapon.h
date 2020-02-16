#ifndef PATAPON_H
#define PATAPON_H

#include <SFML/Graphics.hpp>
#include "../../Config.h"
#include "CollidableObject.h"

using namespace std;

class Patapon : public CollidableObject
{
    public:
    int action = 0;
    float attack_speed = 2.00;
    sf::Clock attack_clock;
    bool threw = false;
    bool stopthrew = false;

    float vspeed = 0;
    float gravity = 981;
    float gclosest = 9999;
    float lclosest = 9999;

    bool focus = false;
    bool getback = false;

    enum Actions
    {
        WALK = 1,
        ATTACK = 2
    };

    Config *thisConfig;
    Patapon();
    void startAttack();
    void LoadConfig(Config *thisConfigs);
    void Draw(sf::RenderWindow& window);
};

#endif // PATAPON_H
