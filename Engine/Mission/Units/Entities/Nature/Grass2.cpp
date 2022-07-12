#include "Grass2.h"

Grass2::Grass2()
{
}
void Grass2::LoadConfig()
{
    /// all (normal) kacheeks have the same animations, so we load them from a hardcoded file
    AnimatedObject::LoadConfig("resources/units/entity/grass_2.p4a");
}
void Grass2::Draw(sf::RenderWindow& window)
{
    /// call the parent function to draw the animations
    AnimatedObject::Draw();
}
void Grass2::OnCollide(CollidableObject* otherObject, int collidedWith, vector<string> collisionData)
{
}
