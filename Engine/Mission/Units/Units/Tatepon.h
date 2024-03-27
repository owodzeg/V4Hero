#ifndef TATEPON_H
#define TATEPON_H

#include "../PlayableUnit.h"
#include <SFML/Graphics.hpp>

class Tatepon : public PlayableUnit
{
public:
    int action = 0;
    float attack_speed = 1;
    sf::Clock attack_clock;
    bool attacked = false;
    bool stopAttack = false;
    bool canAttack = true;
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
        ATTACK = 2,
        DEFEND = 3,
        JUMP = 4
    };

    float near_attack_range = 0;
    float far_attack_range = 0;
    float defend_range = 0;

    sf::Clock partyClock;

    sf::SoundBuffer sword_swing;
    sf::SoundBuffer hit_1, hit_2, hit_3, s_dead;

    Tatepon();
    void startAttack();
    void LoadConfig();
    bool doAttack();
    void doRhythm(std::string current_song = "", std::string current_drum = "", int combo = 0);
    void doMissionEnd();
    void Draw(sf::RenderWindow& window);
    void OnCollide(CollidableObject* otherObject, int collidedWith = -1, vector<string> collisionData = {});
};

#endif // TATEPON_H
