#include "Kirajin_Yari_1.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "../../../../Func.h"
#include "../../../../V4Core.h"

Kirajin_Yari_1::Kirajin_Yari_1()
{

}

void Kirajin_Yari_1::LoadConfig(Config *thisConfigs)
{
    /// all (normal) kacheeks have the same animations, so we load them from a hardcoded file
    AnimatedObject::LoadConfig(thisConfigs,"resources\\units\\entity\\kirajin.p4a");
    AnimatedObject::setAnimationSegment("idle_armed_focused");
}

void Kirajin_Yari_1::Draw(sf::RenderWindow& window)
{
    if(dead)
    {
        cout << "I'm dead now" << endl;

        if(getAnimationSegment() == "stagger")
        {
            cout << "Animation segment is stagger " << cur_pos << " " << anim_end << endl;

            if(cur_pos >= anim_end)
            {
                cout << "Setting death animation" << endl;

                setAnimationSegment("death", true);
            }
        }

        if(death_timer.getElapsedTime().asSeconds() > 5)
        {
            cout << "Death clock passed 3 seconds. Time to bury into the ground" << endl;

            if(getAnimationSegment() == "death")
            {
                cout << "I am despawning" << endl;
                setAnimationSegment("death_despawn", true);
            }

            if(getAnimationSegment() == "death_despawn")
            {
                if(cur_pos >= anim_end)
                {
                    ready_to_erase = true;
                }
            }
        }
    }
    else
    {
        ///Not dead
        if(getAnimationSegment() == "stagger")
        {
            if(cur_pos >= anim_end)
            {
                setAnimationSegment("idle_armed_focused", true);
            }
        }
        else
        {
            if(local_x > 5)
            {
                if(getAnimationSegment() != "walk_yari_focused")
                setAnimationSegment("walk_yari_focused");

                local_x -= float(100) / fps;
            }

            if(local_x < -5)
            {
                if(getAnimationSegment() != "walk_yari_focused")
                setAnimationSegment("walk_yari_focused");

                local_x += float(100) / fps;
            }

            if((local_x <= 5) && (local_x >= -5))
            {
                if(attack_timer.getElapsedTime().asSeconds() >= 6)
                {
                    if(getAnimationSegment() != "attack_prefever")
                    setAnimationSegment("attack_prefever", true);

                    if(getAnimationSegment() == "attack_prefever")
                    {
                        if(cur_pos >= anim_end)
                        {
                            attack_timer.restart();
                        }
                    }
                }
                else
                {
                    if(getAnimationSegment() != "idle_armed_focused")
                    setAnimationSegment("idle_armed_focused");
                }
            }
        }
    }

    vspeed += gravity / fps;

    if(hspeed > 0)
    hspeed -= 230.0 / fps;
    else
    hspeed = 0;

    if(vspeed >= 0)
    {
        if(local_y >= 0)
        {
            vspeed = 0;
            local_y = 0;
        }
    }

    local_x += hspeed / fps;
    local_y += vspeed / fps;

    /// call the parent function to draw the animations
    AnimatedObject::Draw(window);
}
void Kirajin_Yari_1::OnCollide(CollidableObject* otherObject, int collidedWith, vector<string> collisionData)
{
    cout << "Kirajin_Yari_1::OnCollide" << endl;

    if(AnimatedObject::getAnimationSegment() != "death")
    {
        if(collisionData.size() > 0)
        {
            if(isCollidable)
            {
                ///collisionData received from Projectile, process it
                int dmgDealt = atoi(collisionData[0].c_str());
                curHP -= dmgDealt;

                cout << "I received " << to_string(dmgDealt) << "damage, my HP is " << curHP << "/10" << endl;

                hspeed = 120 + (rand() % 50);
                vspeed = -170;
                AnimatedObject::setAnimationSegment("stagger", true);
            }
        }

        if(curHP <= 0)
        {
            dead = true;
            hspeed = 340;
            vspeed = -250;

            isCollidable = false;
            isAttackable = false;

            AnimatedObject::setAnimationSegment("stagger", true);
            death_timer.restart();
        }
    }

    /// note we don't call the parent function. It does nothing, it just serves
    /// as an incomplete function to be overridden by child classes.
}