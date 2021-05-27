#ifndef PLAYABLEUNIT_H
#define PLAYABLEUNIT_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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

    float entity_distance = 0;
    float unit_distance = 0;

    bool dead = false;
    sf::Clock deathClock;
    sf::Clock walkClock;

    float dest_local_x = 0;
    float prev_dest_local_x = 0;

    int army_id = 0; ///ID the unit has in the army, not it's global ID
    bool enemy_in_range = false;

    bool charged = false;
    bool charge_m1 = false; ///count a measure
    bool defend = false;

    bool isFever = false;

    sf::Sound cur_sound;

    std::string old_current_song = "";

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
