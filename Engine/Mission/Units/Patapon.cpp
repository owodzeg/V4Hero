#include "Patapon.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include "../../Func.h"
#include <sstream>
Patapon::Patapon()
{

}
void Patapon::LoadConfig(Config *thisConfigs)
{
    /// load patapon from p4a file
    AnimatedObject::LoadConfig(thisConfigs,"resources\\units\\patapon.p4a");
}

void Patapon::startAttack()
{
    if(action != ATTACK)
    {
        action = ATTACK;
        //AnimatedObject::setAnimationSegment("attack_yari");
        //attack_clock.restart();
    }
}

void Patapon::Draw(sf::RenderWindow& window)
{
    if(AnimatedObject::getAnimationSegment() == "walk")
    {
        framerate = 0.8;
    }
    else
    {
        framerate = 1;
    }

    if(gclosest <= 850)
    {
        focus = true;
    }
    else
    {
        focus = false;
    }

    if((!focus) || (action == WALK))
    {
        if(lclosest <= gclosest-5)
        {
            local_x -= float(200) / fps;
            AnimatedObject::setAnimationSegment("walk");
            AnimatedObject::setLoop(true);
            getback = true;
        }
        else if(lclosest >= gclosest+5)
        {
            local_x += float(200) / fps;
            AnimatedObject::setAnimationSegment("walk");
            AnimatedObject::setLoop(true);
            getback = true;
        }
        else
        {
            //AnimatedObject::setAnimationSegment("idle");
            getback = false;
        }
    }

    cout << "gclosest: " << gclosest << " lclosest: " << lclosest << " global_x: " << global_x << " local_x: " << local_x << endl;

    if(action == ATTACK)
    {
        if(focus)
        {
            if((lclosest >= 375) && (lclosest <= 475))
            {
                getback = false;

                if(stopthrew)
                {
                    if(attack_clock.getElapsedTime().asSeconds() > attack_speed)
                    {
                        AnimatedObject::setAnimationSegment("attack_yari");
                        attack_clock.restart();

                        stopthrew = false;
                    }
                }
                else
                {
                    if(AnimatedObject::getAnimationSegment() != "attack_yari")
                    {
                        AnimatedObject::setAnimationSegment("attack_yari");
                        attack_clock.restart();
                    }
                }
            }
            else if(lclosest > 475)
            {
                AnimatedObject::setAnimationSegment("walk");
                getback = true;

                local_x += float(200) / fps;
            }
            else if(lclosest < 375)
            {
                AnimatedObject::setAnimationSegment("walk");
                getback = true;

                local_x -= float(200) / fps;
            }
        }
    }
    else
    {
        stopthrew = false;
    }

    //cout << current_animation << " " << AnimatedObject::getAnimationPos() << endl;

    if(AnimatedObject::getAnimationSegment() == "attack_yari")
    {
        if((AnimatedObject::getAnimationPos() >= 0.066) && (AnimatedObject::getAnimationPos() <= 0.5))
        {
            vspeed = -433;
        }

        if(!stopthrew)
        {
            if(AnimatedObject::getAnimationPos() >= 0.733)
            {
                threw = true;
                stopthrew = true;
            }
        }
    }

    //cout << "1 local_y: " << local_y << " vspeed: " << vspeed << endl;

    vspeed += gravity / fps;

    //cout << "2 local_y: " << local_y << " vspeed: " << vspeed << endl;

    if(vspeed >= 0)
    {
        if(local_y >= 0)
        {
            vspeed = 0;
            local_y = 0;
        }
    }

    //cout << "3 local_y: " << local_y << " vspeed: " << vspeed << endl;

    local_y += vspeed / fps;

    //cout << "4 local_y: " << local_y << " vspeed: " << vspeed << endl;

    AnimatedObject::Draw(window);
}
