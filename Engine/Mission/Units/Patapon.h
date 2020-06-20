#ifndef PATAPON_H
#define PATAPON_H

#include <SFML/Graphics.hpp>
#include "../../Config.h"
#include "PlayableUnit.h"

using namespace std;

class Patapon : public PlayableUnit
{
    public:
    int action = 0;
    float attack_speed = 1;
    sf::Clock attack_clock;
    bool threw = false;
    bool stopthrew = false;
    bool canThrow = true;

    int attackmode = -1;

    float vspeed = 0;
    float gravity = 981;
    float gclosest = 9999;
    float lclosest = 9999;

    bool focus = false;
    bool getback = false;

    enum Actions
    {
        IDLE = 0,
        WALK = 1,
        ATTACK = 2
    };

    Config *thisConfig;
    Patapon();
    void startAttack();
    void LoadConfig(Config *thisConfigs);
    bool doAttack();
    void doRhythm(std::string current_song, std::string current_drum);
    void Draw(sf::RenderWindow& window);
};

#endif // PATAPON_H
