#include "Rappata.h"
#include "../../../../CoreManager.h"
#include "math.h"
#include <iostream>

Rappata::Rappata()
{
}

void Rappata::LoadConfig()
{
    /// all (normal) Rappatas have the same animations, so we load them from a hardcoded file
    AnimatedObject::LoadConfig("resources/units/entity/rappata.p4a");
    AnimatedObject::setAnimationSegment("idle");

    cur_sound.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));
}

void Rappata::parseAdditionalData(nlohmann::json additional_data)
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

void Rappata::Draw()
{
    /// before we draw the object, check if we are walking and
    if (AnimatedObject::getAnimationSegment() != "death")
    {
        if (react_timer.getElapsedTime().asSeconds() > react_time)
        {
            react_time = 1.0 + ((rand() % 3000) / 1000.0);

            if (distance_to_unit <= 500)
            {
                ///we want to make Rappata more prone to escape the closest the patapon gets.
                /// 800 - minimal range, not likely to escape
                /// 500 - mid chance, might escape but might not
                /// 100 - very high change, Rappata feels threatened
                /// 0 - 100% chance, escape now

                float chance = 500 - distance_to_unit;
                int roll = rand() % 500 + 1;

                //cout << "Should Rappata react? " << chance << ": " << roll << " rolled" << endl;

                if (roll < chance)
                {
                    ///run!!!!
                    walk_timer.restart();
                    walk_time = 2.5 + (rand() % 1500) / 1000;

                    if (!run)
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

        if (local_y >= 0) ///rappata stays on ground
        {
            vspeed = 0;
            local_y = 0;
        }

        if (run)
        {
            //AnimatedObject::moveGlobalPosition(sf::Vector2f(float(160) / fps, 0));
            ///Jump occasionaly, move forward only when jumping
            ///When walk timer is over, wait for last jump to finish and then stop run

            if (walk_timer.getElapsedTime().asSeconds() >= walk_time)
            {
                if (local_y >= 0) ///finish executing last jump
                {
                    AnimatedObject::setAnimationSegment("idle");
                    run = false;
                    jump_mode = 0;
                }
            } else
            {
                ///perform jumps
                if (local_y >= 0) ///rappata stays on ground
                {
                    if (jump_mode == 0)
                    {
                        if (AnimatedObject::getAnimationSegment() != "idle")
                            AnimatedObject::setAnimationSegment("idle");

                        int a = rand() % 10000;

                        if (a <= 1000)
                        {
                            ///jump small
                            AnimatedObject::setAnimationSegment("shortjump", true);
                            jump_mode = 1;

                            //cout << "Short jump" << endl;
                        } else if (a >= 9600)
                        {
                            ///jump big
                            AnimatedObject::setAnimationSegment("longjump", true);
                            jump_mode = 2;

                            //cout << "Long jump" << endl;
                        }
                    }

                    if (AnimatedObject::getAnimationSegment() != "idle")
                    {
                        if (jump_mode == 1)
                        {
                            ///jump small
                            if (AnimatedObject::getAnimationPos() > 1.3)
                            {
                                vspeed = -300;
                                hspeed = 350;
                                jump_mode = 3;

                                //cout << "Short jump execute" << endl;
                            }
                        }

                        if (jump_mode == 2)
                        {
                            ///jump big
                            if (AnimatedObject::getAnimationPos() > 0.9)
                            {
                                vspeed = -600;
                                hspeed = 520;
                                jump_mode = 3;

                                //cout << "Long jump execute" << endl;
                            }
                        }
                    }
                } else
                {
                    jump_mode = 0;
                }
            }
        }

        global_x += hspeed / fps;
        local_y += vspeed / fps;
        vspeed += gravity / fps;
        hspeed -= (gravity / 2) / fps;
        if (hspeed <= 0)
            hspeed = 0;

        //cout << "Local_y: " << local_y << " vspeed: " << vspeed << " walk_timer: " << walk_timer.getElapsedTime().asSeconds() << " segment: " << AnimatedObject::getAnimationSegment() << " get length:" << AnimatedObject::getAnimationPos() << endl;
    } else
    {
        if (cur_pos >= anim_end)
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

                dropItem();
            }
        } else
        {
            isCollidable = false;
            isAttackable = false;
        }
    }

    /// call the parent function to draw the animations
    AnimatedObject::Draw();
}
void Rappata::OnCollide(CollidableObject* otherObject, int collidedWith, vector<string> collisionData)
{
    cout << "Rappata::OnCollide" << endl;

    if (AnimatedObject::getAnimationSegment() != "death")
    {
        walk_timer.restart();
        walk_time = 2.5 + (rand() % 1500) / 1000;

        /// don't start the animation again if Rappata is still running
        run = true;

        if (collisionData.size() > 0)
        {
            ///collisionData received from Projectile, process it
            int dmgDealt = atoi(collisionData[0].c_str());
            curHP -= dmgDealt;
        }

        if (curHP <= 0)
        {
            if (AnimatedObject::getAnimationSegment() != "death")
            {
                AnimatedObject::setAnimationSegment("death", true);
                death_timer.restart();

                cur_sound.stop();
                cur_sound.setBuffer(s_dead);
                cur_sound.play();
            }
        } else
        {
            if (!run)
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
