#include "Patapon.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include "../../../Func.h"
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
        attackmode = -1;
    }
}

bool Patapon::doAttack()
{
    if(threw)
    {
        threw = false;
        return true;
    }
    else
    {
        return false;
    }
}

void Patapon::doRhythm(std::string current_song, std::string current_drum)
{
    if(current_song == "patapata")
    {
        action = WALK;

        if(!focus)
        setAnimationSegment("walk");
        else
        setAnimationSegment("walk_focused");

        setLoop(true);
    }
    else
    {
        if(getAnimationSegment() == "walk")
        {
            if(!getback)
            {
                if(!focus)
                setAnimationSegment("idle_armed", true);
                else
                setAnimationSegment("idle_armed_focused", true);
            }
        }
    }

    if(current_song == "ponpon")
    {
        ///use attack only once
        if(attackmode != 2)
        startAttack();
    }
    else
    {
        ///refresh attack mode
        attackmode = -1;
    }

    if((current_drum == "pata") or (current_drum == "pon") or (current_drum == "chaka") or (current_drum == "don"))
    {
        //cout << current_drum << endl;

        if(!focus)
        setAnimationSegment(current_drum, true);
        else
        setAnimationSegment(current_drum+"_focused", true);
    }
}

void Patapon::doMissionEnd()
{
    if(getAnimationSegment().find("dance") == std::string::npos)
    {
        setAnimationSegment("walk");

        if(partyClock.getElapsedTime().asMilliseconds() > 250)
        {
            if(rand() % 4 == 1)
            {
                int a = rand() % 5 + 1;
                string anim = "dance_var"+to_string(a);

                setAnimationSegment(anim, true);
            }

            partyClock.restart();
        }
    }
    else
    {
        if(cur_pos > anim_end)
        {
            setAnimationSegment("walk");
        }
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

    //cout << "gclosest: " << gclosest << " lclosest: " << lclosest << " global_x: " << global_x << " local_x: " << local_x << endl;

    if(action == ATTACK)
    {
        if(attackmode == -1) ///start the attack
        {
            attack_clock.restart();
            attackmode = 0;

            vspeed = -683;
        }

        if(attackmode == 0) ///begin the attack
        {
            canThrow = true;

            AnimatedObject::setAnimationSegment("attack_fever_jump");

            if(attack_clock.getElapsedTime().asMilliseconds() > 500)
            {
                AnimatedObject::setAnimationSegment("attack_fever_throw", true);
                attack_clock.restart();
                attackmode = 1;
            }
        }

        if(attackmode == 1) ///attack continously
        {
            float anim_speed_multiplier = float(1) / attack_speed;
            framerate = float(1) * anim_speed_multiplier;

            cout << getAnimationPos() << endl;

            if(getAnimationPos() > 0.285)
            {
                if(canThrow)
                {
                    threw = true;
                    canThrow = false;
                }
            }

            if(attack_clock.getElapsedTime().asSeconds() > attack_speed)
            {
                if(canThrow)
                {
                    if(threw == false)
                    threw = true;
                }

                AnimatedObject::setAnimationSegment("attack_fever_throw", true);
                attack_clock.restart();

                canThrow = true;
            }

            if(local_y >= 0)
            {
                AnimatedObject::setAnimationSegment("idle_armed_focused", true);
                attackmode = 2;
            }
        }

        if(attackmode == 2)
        {
            canThrow = true;
            action = IDLE;
        }
    }

    vspeed += gravity / fps;

    if(vspeed >= 0)
    {
        if(local_y >= 0)
        {
            vspeed = 0;
            local_y = 0;
        }
    }

    local_y += vspeed / fps;

    AnimatedObject::Draw(window);
}
