#include "Kacheek.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include "../../../../Func.h"
#include <sstream>
#include "../../../../V4Core.h"

Kacheek::Kacheek()
{

}

void Kacheek::LoadConfig(Config *thisConfigs)
{
    /// all (normal) kacheeks have the same animations, so we load them from a hardcoded file
    AnimatedObject::LoadConfig(thisConfigs,"resources\\units\\entity\\kacheek.p4a");
    AnimatedObject::setAnimationSegment("idle");

    s_startle.loadFromFile("resources/sfx/level/kacheek_startled.ogg");
    s_dead.loadFromFile("resources/sfx/level/kacheek_dead.ogg");

    cur_sound.setVolume(float(thisConfigs->GetInt("masterVolume"))*(float(thisConfigs->GetInt("sfxVolume"))/100.f));
}

void Kacheek::Draw(sf::RenderWindow& window)
{
    /// before we draw the object, check if we are walking and
    if(AnimatedObject::getAnimationSegment() != "death")
    {
        if(react_timer.getElapsedTime().asSeconds() > react_time)
        {
            react_time = 1.0 + ((rand() % 3000) / 1000.0);

            if(distance_to_unit <= 600)
            {
                ///we want to make kacheek more prone to escape the closest the patapon gets.
                /// 800 - minimal range, not likely to escape
                /// 500 - mid chance, might escape but might not
                /// 100 - very high change, kacheek feels threatened
                /// 0 - 100% chance, escape now

                float chance = 600 - distance_to_unit;
                int roll = rand() % 600 + 1;

                if(roll < chance)
                {
                    ///run!!!!
                    walk_timer.restart();
                    walk_time = 2.5 + (rand() % 1500) / 1000;

                    /// don't start the animation again if kacheek is still running
                    if(AnimatedObject::getAnimationSegment() != "walk")
                    {
                        AnimatedObject::setAnimationSegment("walk");
                        current_frame = 0;
                    }

                    if(!run)
                    {
                        cur_sound.stop();
                        cur_sound.setBuffer(s_startle);
                        cur_sound.play();
                    }

                    run = true;
                }
            }

            react_timer.restart();
        }

        if(run)
        {
            AnimatedObject::moveGlobalPosition(sf::Vector2f(float(160) / fps, 0));

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
    }
    else
    {
        if(cur_pos >= anim_end)
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

                dropItem();
            }
        }
        else
        {
            isCollidable = false;
            isAttackable = false;

            ///Animate the tumble here
            if((getAnimationPos() >= 0.2) && (getAnimationPos() <= 1))
            {
                ///Make it do a 180 spin based on frames 12 - 60
                float percentage = (getAnimationPos() - 0.2) / (1 - 0.2);
                rotation = 190 * percentage;

                if(rotation >= 190)
                rotation = 190;

                rotation = rotation * 3.1415928 / 180;

                local_x = 280 * percentage;
            }

            if((getAnimationPos() >= 0.2) && (getAnimationPos() <= 0.4))
            {
                ///Make it do a 180 spin based on frames 12 - 60
                float percentage = (getAnimationPos() - 0.2) / (0.4 - 0.2);
                float raise = -60 * percentage;

                if(raise <= -60)
                raise = -60;

                local_y = raise;
            }

            if((getAnimationPos() >= 0.4) && (getAnimationPos() <= 0.6))
            {
                ///Make it do a 180 spin based on frames 12 - 60
                float percentage = (getAnimationPos() - 0.4) / (0.6 - 0.4);
                float raise = -60 * (1 - percentage);

                if(raise >= 0)
                raise = 0;

                local_y = raise;
            }

            if((getAnimationPos() >= 0.6) && (getAnimationPos() <= 0.8))
            {
                ///Make it do a 180 spin based on frames 12 - 60
                float percentage = (getAnimationPos() - 0.6) / (0.8 - 0.6);
                float raise = -60 * percentage;

                if(raise <= -60)
                raise = -60;

                local_y = raise;
            }

            if((getAnimationPos() >= 0.8) && (getAnimationPos() <= 1))
            {
                ///Make it do a 180 spin based on frames 12 - 60
                float percentage = (getAnimationPos() - 0.8) / (1 - 0.8);
                float raise = 28 + (-100 * (1 - percentage));

                if(raise >= 28)
                raise = 28;

                local_y = raise;
            }
        }
    }

    /// call the parent function to draw the animations
    AnimatedObject::Draw(window);
}
void Kacheek::OnCollide(CollidableObject* otherObject, int collidedWith, vector<string> collisionData)
{
    cout << "Kacheek::OnCollide" << endl;

    if(AnimatedObject::getAnimationSegment() != "death")
    {
        walk_timer.restart();
        walk_time = 2.5 + (rand() % 1500) / 1000;

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
        {
            if(AnimatedObject::getAnimationSegment() != "death")
            {
                AnimatedObject::setAnimationSegment("death", true);
                death_timer.restart();

                cur_sound.stop();
                cur_sound.setBuffer(s_dead);
                cur_sound.play();
            }
        }
        else
        {
            if(!run)
            {
                cur_sound.stop();
                cur_sound.setBuffer(s_startle);
                cur_sound.play();
            }
        }

        run = true;
    }
    //ready_to_erase = true;

    /// note we don't call the parent function. It does nothing, it just serves
    /// as an incomplete function to be overridden by child classes.
}
