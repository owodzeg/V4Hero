#include "Grass1.h"

Grass1::Grass1()
{
}
void Grass1::LoadConfig()
{
    /// all (normal) kacheeks have the same animations, so we load them from a hardcoded file
    AnimatedObject::LoadConfig("resources/units/entity/grass_1.p4a");
}
void Grass1::Draw()
{
    /// call the parent function to draw the animations
    AnimatedObject::Draw();
}
void Grass1::OnCollide(CollidableObject* otherObject, int collidedWith, vector<string> collisionData)
{
}
