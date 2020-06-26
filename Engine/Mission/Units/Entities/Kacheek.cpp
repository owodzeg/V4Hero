#include "Kacheek.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include "../../../Func.h"
#include <sstream>
Kacheek::Kacheek()
{
    type = HOSTILE;
}
void Kacheek::LoadConfig(Config *thisConfigs)
{
    /// all (normal) kacheeks have the same animations, so we load them from a hardcoded file
    AnimatedObject::LoadConfig(thisConfigs,"resources\\units\\kacheek.p4a");
}
void Kacheek::Draw(sf::RenderWindow& window)
{
    /// before we draw the object, check if we are walking and
    if(run)
    {
        AnimatedObject::moveGlobalPosition(sf::Vector2f(float(140) / fps, 0));

        if(walk_timer.getElapsedTime().asSeconds() >= walk_time)
        {
            AnimatedObject::setAnimationSegment("idle");
            run = false;
        }
        else
        {
            AnimatedObject::setAnimationSegment("walk");
        }
    }

    /// call the parent function to draw the animations
    AnimatedObject::Draw(window);
}
void Kacheek::OnCollide(CollidableObject* otherObject, int collidedWith, vector<string> collisionData)
{
    cout << "Kacheek::OnCollide" << endl;

    run = true;
    walk_timer.restart();
    walk_time = 1.5 + (rand() % 2500) / 1000;

    /// don't start the animation again if kacheek is still running
    if(AnimatedObject::getAnimationSegment() != "walk")
    {
        AnimatedObject::setAnimationSegment("walk");
        current_frame = 0;
    }

    if(collisionData.size() > 0)
    {
        ///collisionData received from Projectile, process it
        int dmgDealt = atoi(collisionData[0].c_str());
        curHP -= dmgDealt;
    }

    if(curHP <= 0)
    ready_to_erase = true;

    /// note we don't call the parent function. It does nothing, it just serves
    /// as an incomplete function to be overridden by child classes.
}
