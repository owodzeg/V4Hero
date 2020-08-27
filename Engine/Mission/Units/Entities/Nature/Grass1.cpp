#include "Grass1.h"

Grass1::Grass1()
{

}
void Grass1::LoadConfig(Config *thisConfigs)
{
    /// all (normal) kacheeks have the same animations, so we load them from a hardcoded file
    AnimatedObject::LoadConfig(thisConfigs,"resources\\units\\entity\\grass_1.p4a");
}
void Grass1::Draw(sf::RenderWindow& window)
{
    /// call the parent function to draw the animations
    AnimatedObject::Draw(window);
}
void Grass1::OnCollide(CollidableObject* otherObject, int collidedWith, vector<string> collisionData)
{

}
