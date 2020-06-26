#include "EndFlag.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include "../../../Func.h"
#include <sstream>
#include "../../../V4Core.h"
EndFlag::EndFlag()
{

}
void EndFlag::LoadConfig(Config *thisConfigs)
{
    AnimatedObject::LoadConfig(thisConfigs,"resources\\units\\end_flag.p4a");
    current_animation = "idle";
    end_sound.loadFromFile("resources\\sfx\\level\\endflag.ogg");
    s_end.setBuffer(end_sound);
}
void EndFlag::Draw(sf::RenderWindow& window)
{
    if(missionEnd)
    {
        if(endMissionClock.getElapsedTime().asMilliseconds() > 19000)
        {
            cout << "End mission" << endl;
            /// A wall is unyielding, so it does nothing when collided with.

            /// note we don't call the parent function. It does nothing, it just serves
            /// as an incomplete function to be overridden by child classes.
            /// end the mission

            thisConfig->thisCore->currentController.StopMission();

            cout << "Go to Patapolis" << endl;

            sf::Thread loadingThreadInstance(thisConfig->thisCore->LoadingThread,thisConfig->thisCore);
            thisConfig->thisCore->continueLoading=true;
            thisConfig->thisCore->window.setActive(false);
            loadingThreadInstance.launch();

            thisConfig->thisCore->mainMenu.patapolisMenu.doWaitKeyPress = false;
            thisConfig->thisCore->mainMenu.patapolisMenu.Show();
            thisConfig->thisCore->mainMenu.patapolisMenu.isActive = true;

            if (!thisConfig->thisCore->mainMenu.patapolisMenu.initialised)
            {
                /// patapolis might not be initialised because we could be running the pre-patapolis scripted first mission.
                cout << "[ENDFLAG] Initialize Patapolis for the first time" << endl;
                thisConfig->thisCore->mainMenu.patapolisMenu.Initialise(thisConfig,thisConfig->thisCore->mainMenu.keyMapping,thisConfig->thisCore,&thisConfig->thisCore->mainMenu);
            }
            else
            {
                cout << "Don't initialize Patapolis, just show it again" << endl;
            }

            thisConfig->thisCore->mainMenu.patapolisMenu.location = 3;
            thisConfig->thisCore->mainMenu.patapolisMenu.SetTitle(3);
            thisConfig->thisCore->mainMenu.patapolisMenu.camPos = thisConfig->thisCore->mainMenu.patapolisMenu.locations[3];
            thisConfig->thisCore->mainMenu.patapolisMenu.fade_alpha = 255;

            while(endMissionClock.getElapsedTime().asMilliseconds() < 21000)
            {
                ///halt loading for a second
            }

            thisConfig->thisCore->LoadingWaitForKeyPress();

            thisConfig->thisCore->continueLoading=false;

            thisConfig->thisCore->ChangeRichPresence("In Patapolis", "logo", "");
        }
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
