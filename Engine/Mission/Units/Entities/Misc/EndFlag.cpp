#include "EndFlag.h"
#include "../../../../Func.h"
#include "../../../../V4Core.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "../../../../CoreManager.h"

EndFlag::EndFlag()
{

}

void EndFlag::LoadConfig()
{
    AnimatedObject::LoadConfig("resources/units/entity/end_flag.p4a");
    current_animation = "idle";
    end_sound.loadFromFile("resources/sfx/level/endflag.ogg");
    s_end.setBuffer(end_sound);
    s_end.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));
}

void EndFlag::Draw(sf::RenderWindow& window)
{
    if (missionEnd)
    {

    }

    AnimatedObject::Draw(window);
}
void EndFlag::OnCollide(CollidableObject* otherObject, int collidedWith, vector<string> collisionData)
{
    if (collidedWith == -1)
    {
        if (!CoreManager::getInstance().getMissionController()->missionEnd)
        {
            if (AnimatedObject::getAnimationSegment() == "idle")
            {
                AnimatedObject::setAnimationSegment("triggered", true);
                CoreManager::getInstance().getMissionController()->missionEnd = true;
                CoreManager::getInstance().getMissionController()->rhythm.Stop();

                endMissionClock.restart();

                missionEnd = true;
                CoreManager::getInstance().getMissionController()->missionEndTimer.restart();

                s_end.play();
            }
        }
    }
}
