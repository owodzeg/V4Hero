#ifndef PLAYABLEUNIT_H
#define PLAYABLEUNIT_H

#include <SFML/Graphics.hpp>
#include "../../Config.h"
#include "CollidableObject.h"

class PlayableUnit : public CollidableObject
{
    public:

    enum Classes
    {
        DUMMY = -1,
        HATAPON = 0,
        YARIPON = 1
    };

    int unitID = DUMMY;

    ///Individual unit stats
    float current_hp = 100;
    float max_hp = 100;

    int mindmg = 1;
    int maxdmg = 1;


    bool dead = false;
    sf::Clock deathClock;

    PlayableUnit();
    virtual void setUnitID(int new_unitID);
    virtual int getUnitID();
    virtual float getUnitHP();
    virtual float getUnitMaxHP();
    virtual void setUnitHP(float hp);
    virtual bool doAttack();
    virtual void doRhythm(std::string current_song="", std::string current_drum="", int combo=0);
    virtual void doMissionEnd();
};

#endif // PLAYABLEUNIT_H
