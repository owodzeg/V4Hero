#ifndef YARIPON_H
#define YARIPON_H

#include "../../../Config.h"
#include "../PlayableUnit.h"
#include <SFML/Graphics.hpp>

using namespace std;

class Yaripon : public PlayableUnit
{
public:
    int action = 0;
    float attack_speed = 1;
    sf::Clock attack_clock;
    bool attacked = false;
    bool stopAttack = false;
    bool canAttack = true;

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

    sf::SoundBuffer spear_throw;
    sf::SoundBuffer hit_1, hit_2, hit_3, s_dead;

    Yaripon();
    void startAttack();
    void LoadConfig();
    bool doAttack();
    void doRhythm(std::string current_song = "", std::string current_drum = "", int combo = 0);
    void doMissionEnd();
    void Draw();
    void OnCollide(CollidableObject* otherObject, int collidedWith = -1, vector<string> collisionData = {});
    void Update() override;
    void UpdateRhythm(std::string current_song, std::string current_drum, int combo) override;

private:
    enum class AttackMode
    {
        Preparing = -2,
        Ready = -1,
        Start = 0,
        Attacking = 1,
        AfterAttack = 2,
    };

    bool dying = false;
    AttackMode attackMode = AttackMode::Preparing;
};

#endif // YARIPON_H
