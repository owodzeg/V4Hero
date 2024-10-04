#include "PlayableUnit.h"
#include <spdlog/spdlog.h>

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
    SPDLOG_WARN("PlayableUnit::doAttack() was not overriden by child class");
    return false;
}

void PlayableUnit::doRhythm(std::string current_song, std::string current_drum, int combo)
{
    SPDLOG_WARN("PlayableUnit::doRhythm() was not overriden by child class");
}

void PlayableUnit::doMissionEnd()
{
    SPDLOG_WARN("PlayableUnit::doAttack() was not overriden by child class");
}

void PlayableUnit::UpdateRhythm(std::string current_song, std::string current_drum, int combo)
{
    SPDLOG_WARN("PlayableUnit::UpdateRhythm() was not overriden by child class");
}
