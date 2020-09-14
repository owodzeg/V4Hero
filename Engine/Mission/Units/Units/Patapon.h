#ifndef PATAPON_H
#define PATAPON_H

#include <SFML/Graphics.hpp>
#include "../../../Config.h"
#include "../PlayableUnit.h"

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

    int attackmode = -2;

    float hspeed = 0;
    float vspeed = 0;
    float gravity = 981;
    float gclosest = 9999;
    float lclosest = 9999;

    bool focus = false;
    bool getback = false;
    bool dest_set = false;

    enum Actions
    {
        IDLE = 0,
        WALK = 1,
        ATTACK = 2
    };

    float high_throw_range = 0;
    float low_throw_range = 0;

    sf::Clock partyClock;

    Config *thisConfig;
    Patapon();
    void startAttack();
    void LoadConfig(Config *thisConfigs);
    bool doAttack();
    void doRhythm(std::string current_song="", std::string current_drum="", int combo=0);
    void doMissionEnd();
    void Draw(sf::RenderWindow& window);
    void OnCollide(CollidableObject* otherObject, int collidedWith=-1, vector<string> collisionData = {});
};

#endif // PATAPON_H
