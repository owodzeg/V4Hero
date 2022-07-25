#include "KirajinPoweredTowerBig.h"
#include "../../../../Func.h"
#include "../../../../V4Core.h"
#include "../../../../CoreManager.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include <sstream>

KirajinPoweredTowerBig::KirajinPoweredTowerBig()
{
}

void KirajinPoweredTowerBig::parseAdditionalData(nlohmann::json additional_data)
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

void KirajinPoweredTowerBig::LoadConfig()
{
    /// all (normal) kacheeks have the same animations, so we load them from a hardcoded file
    AnimatedObject::LoadConfig("resources/units/entity/kirajin_poweredtower_big.p4a");
    AnimatedObject::setAnimationSegment("idle");

    cur_sound.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));

    s_broken.loadFromFile("resources/sfx/level/building_big_broken.ogg");
}

void KirajinPoweredTowerBig::Draw()
{
    if (dead)
    {
        shake += 2 / fps;

        if (shake >= 15)
            shake = 15;

        if (shake_timer.getElapsedTime().asMilliseconds() > shake_delay)
        {
            negative = !negative;

            if (negative)
                local_x = shake * (-1);
            else
                local_x = shake;

            shake_timer.restart();
        }

        if (death_timer.getElapsedTime().asSeconds() >= 1.5)
        {
            local_y += 50.0 / fps;
        }

        if (death_timer.getElapsedTime().asSeconds() >= 3)
        {
            sf::Color c = AnimatedObject::getColor();
            float alpha = c.a;

            alpha -= 250.0 / fps;

            if (alpha <= 0)
            {
                alpha = 0;
                ready_to_erase = true;
            }

            AnimatedObject::setColor(sf::Color(c.r, c.g, c.b, alpha));
        }
    }

    /// call the parent function to draw the animations
    AnimatedObject::Draw();
}
void KirajinPoweredTowerBig::OnCollide(CollidableObject* otherObject, int collidedWith, vector<string> collisionData)
{
    cout << "KirajinPoweredTowerBig::OnCollide" << endl;

    if (!dead)
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

        if ((curHP > 0) && (curHP <= maxHP / 4))
        {
            if (AnimatedObject::getAnimationSegment() != "idle_damaged")
                AnimatedObject::setAnimationSegment("idle_damaged", true);
        }

        if (curHP <= 0)
        {
            if (AnimatedObject::getAnimationSegment() != "idle_damaged")
                AnimatedObject::setAnimationSegment("idle_damaged", true);

            dead = true;

            isCollidable = false;
            isAttackable = false;

            dropItem();

            //AnimatedObject::setAnimationSegment("destroy", true);
            death_timer.restart();

            cur_sound.stop();
            cur_sound.setBuffer(s_broken);
            cur_sound.play();
        }
    }

    /// note we don't call the parent function. It does nothing, it just serves
    /// as an incomplete function to be overridden by child classes.
}
