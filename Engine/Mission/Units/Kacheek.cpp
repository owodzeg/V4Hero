#include "Kacheek.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include "../../Func.h"
#include <sstream>
Kacheek::Kacheek()
{

}
void Kacheek::LoadConfig(Config *thisConfigs)
{
    /// all (normal) kacheeks have the same animations, so we load them from a hardcoded file
    AnimatedObject::LoadConfig(thisConfigs,"resources\\graphics\\units\\kacheek\\");
}
void Kacheek::Draw(sf::RenderWindow& window)
{
    /// before we draw the object, check if we are walking and
    if(current_animation == "walk")
    {
        x += float(140) / fps;
        if(walk_timer.getElapsedTime().asSeconds() >= 2)
        {
            current_animation = "idle";
        }
    }

    /// call the parent function to draw the animations
    AnimatedObject::Draw(window);
}
void Kacheek::OnCollide(CollidableObject* otherObject)
{
    cout << "Kacheek::OnCollide" << endl;

    /// if the kacheek is collided with, it starts walking (running?) away
    if(current_animation != "walk")
    {
        current_animation = "walk";
        current_frame = 0;
        walk_timer.restart();
    }

    /// note we don't call the parent function. It does nothing, it just serves
    /// as an incomplete function to be overridden by child classes.
}
