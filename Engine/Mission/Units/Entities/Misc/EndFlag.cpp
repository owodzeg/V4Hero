#include "EndFlag.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include "../../../../Func.h"
#include <sstream>
#include "../../../../V4Core.h"
EndFlag::EndFlag()
{

}
void EndFlag::LoadConfig(Config *thisConfigs)
{
    AnimatedObject::LoadConfig(thisConfigs,"resources\\units\\entity\\end_flag.p4a");
    current_animation = "idle";
    end_sound.loadFromFile("resources\\sfx\\level\\endflag.ogg");
    s_end.setBuffer(end_sound);
    s_end.setVolume(float(thisConfigs->GetInt("masterVolume"))*(float(thisConfigs->GetInt("sfxVolume"))/100.f));
}
void EndFlag::Draw(sf::RenderWindow& window)
{
    if(missionEnd)
    {

    }

    AnimatedObject::Draw(window);
}
void EndFlag::OnCollide(CollidableObject* otherObject, int collidedWith, vector<string> collisionData)
{
    if(collidedWith == -1)
    {
        if(AnimatedObject::getAnimationSegment() == "idle")
        {
            AnimatedObject::setAnimationSegment("triggered",true);
            thisConfig->thisCore->currentController.missionEnd = true;
            thisConfig->thisCore->currentController.rhythm.Stop();

            endMissionClock.restart();

            missionEnd = true;
            thisConfig->thisCore->currentController.missionEndTimer.restart();

            s_end.play();
        }
    }
}
