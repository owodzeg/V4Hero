#include "Hatapon.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include "../../Func.h"
#include <sstream>

Hatapon::Hatapon()
{

}

void Hatapon::LoadConfig(Config *thisConfigs)
{
    /// all (normal) kacheeks have the same animations, so we load them from a hardcoded file
    AnimatedObject::LoadConfig(thisConfigs,"resources\\units\\hatapon.p4a");
}

void Hatapon::Draw(sf::RenderWindow& window)
{
    /// call the parent function to draw the animations
    AnimatedObject::Draw(window);
}

void Hatapon::OnCollide(CollidableObject* otherObject)
{
    /// note we don't call the parent function. It does nothing, it just serves
    /// as an incomplete function to be overridden by child classes.
}
