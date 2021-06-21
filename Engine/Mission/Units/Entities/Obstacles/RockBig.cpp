#include "RockBig.h"
#include "../../../../Func.h"
#include "../../../../V4Core.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include <sstream>

RockBig::RockBig()
{
}

void RockBig::LoadConfig(Config* thisConfigs)
{
    /// all (normal) kacheeks have the same animations, so we load them from a hardcoded file
    AnimatedObject::LoadConfig(thisConfigs, "resources/units/entity/rock_big.p4a");
    AnimatedObject::setAnimationSegment("idle");

    cur_sound.setVolume(float(thisConfigs->GetInt("masterVolume")) * (float(thisConfigs->GetInt("sfxVolume")) / 100.f));

    s_broken.loadFromFile("resources/sfx/level/building_medium_broken.ogg");
}

void RockBig::parseAdditionalData(nlohmann::json additional_data)
{
    if (additional_data.contains("forceSpawnOnLvl"))
    {
        force_spawn = true;
        force_spawn_lvl = additional_data["forceSpawnOnLvl"];
    }

    if (additional_data.contains("forceDropIfNotObtained"))
    {
        force_drop = true;
        force_drop_item = additional_data["forceDropIfNotObtained"][0];

        if (additional_data["forceDropIfNotObtained"][1] != "any")
        {
            force_drop_mission_lvl = additional_data["forceDropIfNotObtained"][1];
        }
    }
}


void RockBig::Draw(sf::RenderWindow& window)
{
    if (dead)
    {
        if (death_timer.getElapsedTime().asSeconds() >= 3)
        {
            sf::Color c = AnimatedObject::getColor();
            float alpha = c.a;

            alpha -= 510.0 / fps;

            if (alpha <= 0)
            {
                alpha = 0;
                ready_to_erase = true;
            }

            AnimatedObject::setColor(sf::Color(c.r, c.g, c.b, alpha));
        }
    }

    /// call the parent function to draw the animations
    AnimatedObject::Draw(window);
}
void RockBig::OnCollide(CollidableObject* otherObject, int collidedWith, vector<string> collisionData)
{
    cout << "RockBig::OnCollide" << endl;

    if ((AnimatedObject::getAnimationSegment() != "destroy") && (AnimatedObject::getAnimationSegment() != "destroyed"))
    {
        if (collisionData.size() > 0)
        {
            if (isCollidable)
            {
                ///collisionData received from Projectile, process it
                int dmgDealt = atoi(collisionData[0].c_str());
                curHP -= dmgDealt;

                cout << "I received " << to_string(dmgDealt) << "damage, my HP is " << curHP << endl;
            }
        }

        if ((curHP > 0) && (curHP <= maxHP / 2))
        {
            if (AnimatedObject::getAnimationSegment() != "idle_damaged")
                AnimatedObject::setAnimationSegment("idle_damaged", true);
        }

        if (curHP <= 0)
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
