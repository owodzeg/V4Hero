#include "PlayableUnit.h"

PlayableUnit::PlayableUnit()
{

}

void PlayableUnit::setUnitID(int new_unitID)
{
    unitID = new_unitID;
}

int PlayableUnit::getUnitID()
{
    return unitID;
}

bool PlayableUnit::doAttack()
{
    cout << "PlayableUnit::doAttack() was not overriden by child class" << endl;
}

void PlayableUnit::doRhythm(std::string current_song, std::string current_drum, int combo)
{
    cout << "PlayableUnit::doRhythm() was not overriden by child class" << endl;
}

void PlayableUnit::doMissionEnd()
{
    cout << "PlayableUnit::doMissionEnd() was not overriden by child class" << endl;
}
