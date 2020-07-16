#include "Kacheek.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include "../../../Func.h"
#include <sstream>
#include "../../../V4Core.h"
Kacheek::Kacheek()
{
    type = HOSTILE;
}
void Kacheek::LoadConfig(Config *thisConfigs)
{
    /// all (normal) kacheeks have the same animations, so we load them from a hardcoded file
    AnimatedObject::LoadConfig(thisConfigs,"resources\\units\\entity\\kacheek.p4a");
    AnimatedObject::setAnimationSegment("idle");
}
void Kacheek::Draw(sf::RenderWindow& window)
{
    /// before we draw the object, check if we are walking and
    if(AnimatedObject::getAnimationSegment() != "death")
    {
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

                if(!dropped_item)
                {
                    ///Drop item mechanism! Really cool!
                    cout << "Look at me! I'm a Kacheek and I'm dropping an item!" << endl;
                    int rng = rand() % 100 + 1; ///select 1 - 100;
                    int total_rng = 0;
                    int id_picked = 0;

                    cout << "Rng: " << rng << endl;

                    for(int i=0; i<loot_table.size(); i++)
                    {
                        total_rng += loot_table[i].item_chance;

                        if(rng <= total_rng)
                        {
                            id_picked = loot_table[i].item_id;
                            cout << "Picked id: " << id_picked << endl;

                            break;
                        }
                    }

                    if(id_picked != 0)
                    {
                        auto item = thisConfig->thisCore->savereader.itemreg.GetItemByID(id_picked);
                        vector<string> data = {item->spritesheet, to_string(item->spritesheet_id), to_string(id_picked)};

                        thisConfig->thisCore->currentController.spawnEntity("droppeditem",5,getGlobalPosition().x,0,getGlobalPosition().y-60,0,0,1,sf::Color::White,0,0,vector<Entity::Loot>(), data);

                        dropped_item = true;
                    }
                    else
                    {
                        cout << "No item dropped :(" << endl;
                    }
                }
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
                float raise = 40 + (-100 * (1 - percentage));

                if(raise >= 40)
                raise = 40;

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
        {
            if(AnimatedObject::getAnimationSegment() != "death")
            {
                AnimatedObject::setAnimationSegment("death", true);
                death_timer.restart();
            }
        }
    }
    //ready_to_erase = true;

    /// note we don't call the parent function. It does nothing, it just serves
    /// as an incomplete function to be overridden by child classes.
}
