#include "Wall.h"
#include "../../../../Func.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include <sstream>
Wall::Wall()
{
}
void Wall::LoadConfig()
{
    AnimatedObject::LoadConfig("resources/graphics/units/kacheek/");
}
void Wall::Draw(sf::RenderWindow& window)
{
    AnimatedObject::Draw(window);
}
void Wall::OnCollide(CollidableObject otherObject)
{
    /// A wall is unyielding, so it does nothing when collided with.

    /// note we don't call the parent function. It does nothing, it just serves
    /// as an incomplete function to be overridden by child classes.
}
