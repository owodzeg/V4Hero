#include "Wall.h"

Wall::Wall()
{
}
void Wall::LoadConfig()
{
    AnimatedObject::LoadConfig("resources/graphics/units/kacheek/");
}
void Wall::Draw()
{
    AnimatedObject::Draw();
}
void Wall::OnCollide(CollidableObject otherObject)
{
    /// A wall is unyielding, so it does nothing when collided with.

    /// note we don't call the parent function. It does nothing, it just serves
    /// as an incomplete function to be overridden by child classes.
}
