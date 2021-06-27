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

float PlayableUnit::getUnitHP()
{
    return current_hp;
}

float PlayableUnit::getUnitMaxHP()
{
    return max_hp;
}

void PlayableUnit::setUnitHP(float hp)
{
    current_hp = hp;
}

bool PlayableUnit::doAttack()
{
    cout << "PlayableUnit::doAttack() was not overriden by child class" << endl;
    return false;
}

void PlayableUnit::doRhythm(std::string current_song, std::string current_drum, int combo)
{
    cout << "PlayableUnit::doRhythm() was not overriden by child class" << endl;
}

void PlayableUnit::doMissionEnd()
{
    cout << "PlayableUnit::doMissionEnd() was not overriden by child class" << endl;
}

void PlayableUnit::UpdateRhythm(std::string current_song, std::string current_drum, int combo)
{
}
