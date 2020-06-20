#include "EndFlag.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include "../../Func.h"
#include <sstream>
#include "../../V4Core.h"
EndFlag::EndFlag()
{

}
void EndFlag::LoadConfig(Config *thisConfigs)
{
    AnimatedObject::LoadConfig(thisConfigs,"resources\\units\\end_flag.p4a");
    current_animation = "idle";
}
void EndFlag::Draw(sf::RenderWindow& window)
{
    AnimatedObject::Draw(window);
}
void EndFlag::OnCollide(CollidableObject* otherObject, int collidedWith, vector<string> collisionData)
{
    if(collidedWith == -1)
    {
        cout << "End mission" << endl;
        /// A wall is unyielding, so it does nothing when collided with.

        /// note we don't call the parent function. It does nothing, it just serves
        /// as an incomplete function to be overridden by child classes.
        /// end the mission
        thisConfig->thisCore->currentController.StopMission();

        cout << "Go to Patapolis" << endl;

        if (!thisConfig->thisCore->mainMenu.patapolisMenu.initialised){
            /// patapolis might not be initialised because we could be running the pre-patapolis scripted first mission.
            cout << "[ENDFLAG] Initialize Patapolis for the first time" << endl;

            thisConfig->thisCore->continueLoading=true;
            thisConfig->thisCore->window.setActive(false);
            sf::Thread loadingThreadInstance(thisConfig->thisCore->LoadingThread,thisConfig->thisCore);
            loadingThreadInstance.launch();

            thisConfig->thisCore->mainMenu.patapolisMenu.doWaitKeyPress = false;
            thisConfig->thisCore->mainMenu.patapolisMenu.Show();
            thisConfig->thisCore->mainMenu.patapolisMenu.isActive = true;
            thisConfig->thisCore->mainMenu.patapolisMenu.Initialise(thisConfig,thisConfig->thisCore->mainMenu.keyMapping,thisConfig->thisCore,&thisConfig->thisCore->mainMenu);

            thisConfig->thisCore->continueLoading=false;

        } else {
            cout << "Don't initialize Patapolis, just show it again" << endl;

            thisConfig->thisCore->mainMenu.patapolisMenu.Show();
            thisConfig->thisCore->mainMenu.patapolisMenu.isActive = true;
        }
    }
}
