#include "WoodenSpikes.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "../../../../Func.h"
#include "../../../../V4Core.h"

WoodenSpikes::WoodenSpikes()
{

}

void WoodenSpikes::LoadConfig(Config *thisConfigs)
{
    /// all (normal) kacheeks have the same animations, so we load them from a hardcoded file
    AnimatedObject::LoadConfig(thisConfigs,"resources\\units\\entity\\wooden_spikes.p4a");
    AnimatedObject::setAnimationSegment("idle");

    cur_sound.setVolume(float(thisConfigs->GetInt("masterVolume"))*(float(thisConfigs->GetInt("sfxVolume"))/100.f));

    s_broken.loadFromFile("resources/sfx/level/boulder_broken.ogg");
}

void WoodenSpikes::parseAdditionalData(std::vector<std::string> additional_data)
{
    for(int i=0; i<additional_data.size(); i++)
    {
        if(additional_data[i].find("forceSpawnOnLvl") != std::string::npos)
        {
            vector<string> eq = Func::Split(additional_data[i], ':');

            force_spawn = true;
            force_spawn_lvl = stoi(eq[1]);
        }
        else if(additional_data[i].find("forceDropIfNotObtained") != std::string::npos)
        {
            vector<string> eq = Func::Split(additional_data[i], ':');

            force_drop = true;
            force_drop_item = stoi(eq[1]);

            if(eq[2] != "any")
            force_drop_mission_lvl = 0;
        }
    }
}

void WoodenSpikes::Draw(sf::RenderWindow& window)
{
    if(dead)
    {
        if(death_timer.getElapsedTime().asSeconds() >= 3)
        {
            sf::Color c = AnimatedObject::getColor();
            float alpha = c.a;

            alpha -= 510.0 / fps;

            if(alpha <= 0)
            {
                alpha = 0;
                ready_to_erase = true;
            }

            AnimatedObject::setColor(sf::Color(c.r,c.g,c.b,alpha));
        }
    }

    /// call the parent function to draw the animations
    AnimatedObject::Draw(window);
}
void WoodenSpikes::OnCollide(CollidableObject* otherObject, int collidedWith, vector<string> collisionData)
{
    cout << "WoodenSpikes::OnCollide" << endl;

    if((AnimatedObject::getAnimationSegment() != "destroy") && (AnimatedObject::getAnimationSegment() != "destroyed"))
    {
        if(collisionData.size() > 0)
        {
            if(isCollidable)
            {
                ///collisionData received from Projectile, process it
                int dmgDealt = atoi(collisionData[0].c_str());
                curHP -= dmgDealt;

                cout << "I received " << to_string(dmgDealt) << "damage, my HP is " << curHP << endl;
            }
        }

        if(curHP <= 0)
        {
            dead = true;

            isCollidable = false;
            isAttackable = false;

            dropItem();

            AnimatedObject::setAnimationSegment("destroy", true);
            death_timer.restart();

            cur_sound.stop();
            cur_sound.setBuffer(s_broken);
            cur_sound.play();
        }
    }

    /// note we don't call the parent function. It does nothing, it just serves
    /// as an incomplete function to be overridden by child classes.
}
