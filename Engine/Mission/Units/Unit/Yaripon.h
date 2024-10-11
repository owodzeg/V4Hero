#ifndef YARIPON_H
#define YARIPON_H

#include "../AnimatedObject.h"
#include "../../../Graphics/PText.h"

class Yaripon {
public:
    AnimatedObject main;
    float local_x=0, local_y=0;
    float global_x=0, global_y=0;

    float maxHP = 100;
    float curHP = maxHP;

    int order = 0;
    int maxp = 0;

    float attackSpeed = 1000;
    float closestEnemyX = 0;
    // timer when the attack can be done. (spear throwing delay when in air)
    // walk -> jump -> throw throw throw -> fall
    float canAttackIn = 0;
    float distanceToTravel = 0;

    float attack_x = 0, gap_x = 0;
    int attackType = 0;

    bool enemyInSight = false;

    int action = 0;

    float pataSpeed = 0;
    float pataMaxSpeed = 400;
    float pataCurMaxSpeed = pataMaxSpeed;
    float accelerationFactor = 2;
    float decelerationFactor = 2;

    int globalRand = 0;

    sf::Clock attackWalkTimer;
    bool inAttackSequence = false;
    sf::Clock inAttackTimer;

    float vspeed = 0;
    float gravity = 1400;

    bool threw = false;
    bool walkBack = false;
    bool performedAttack = true;

    bool toggleDebug = true;

    PText debugText;

    Yaripon(int which, int maxpons);
    void Advance();
    void Attack(int power);
    void PerformAttack();
    void StopAttack();
    void StopAll();
    void Drum(std::string drum);
    void Draw();
};

#endif //YARIPON_H
