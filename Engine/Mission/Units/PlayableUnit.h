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

    PlayableUnit();
    virtual void setUnitID(int new_unitID);
    virtual int getUnitID();
    virtual bool doAttack();
    virtual void doRhythm(std::string current_song, std::string current_drum);
};

#endif // PLAYABLEUNIT_H
