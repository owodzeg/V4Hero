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
    AnimatedObject::LoadConfig(thisConfigs,"resources/graphics/units/endflag/");
}
void EndFlag::Draw(sf::RenderWindow& window)
{
    AnimatedObject::Draw(window);
}
void EndFlag::OnCollide(CollidableObject* otherObject)
{
    cout << "End mission" << endl;
    /// A wall is unyielding, so it does nothing when collided with.

    /// note we don't call the parent function. It does nothing, it just serves
    /// as an incomplete function to be overridden by child classes.
    /// end the mission
    thisConfig->thisCore->currentController.StopMission();
    thisConfig->thisCore->mainMenu.patapolisMenu.Show();
}
