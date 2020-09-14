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
    AnimatedObject::LoadConfig(thisConfigs,"resources\\units\\unit\\patapon.p4a");
    setAnimationSegment("idle_armed");

    isCollidable = true;
    isAttackable = true;
}

void Patapon::startAttack()
{
    if(action != ATTACK)
    {
        action = ATTACK;
        attackmode = -2;
        walkClock.restart();
        dest_set = false;
    }
}

bool Patapon::doAttack()
{
    if(action == ATTACK)
    {
        if(enemy_in_range)
        {
            if(attackmode == -2) ///prepare the attack (move around)
            {
                ///Calculate the local x
                if(!dest_set)
                {
                    if(entity_distance < 360) ///unit is too close, go back (distance = 250)
                    {
                        dest_local_x = -(360-entity_distance) - (rand() % 50); ///take a random position farther from the enemy
                    }
                    else if((entity_distance >= 360) && (entity_distance <= 410)) ///perfect spot, make some tiny adjustments and start attacking
                    {
                        ///320-370
                        dest_local_x += rand() % 40 - 20;
                    }
                    else if(entity_distance > 410) ///unit is too far, get closer (distance = 400)
                    {
                        dest_local_x = (entity_distance - 410) + (rand() % 50); ///take a random position closer the enemy
                    }

                    dest_set = true;

                    cout << "Setting dest_local_x to " << dest_local_x << " (prev: " << prev_dest_local_x << ") (entity_distance: " << entity_distance << ")" << endl;
                }

                //dest_local_x = entity_distance;

                if((local_x >= dest_local_x-5) && (local_x <= dest_local_x+5)) ///When you are set at your position, start the attack
                attackmode = -1;

                if(walkClock.getElapsedTime().asMilliseconds() >= 750) ///If walking takes you too long, stop and start the attack
                {
                    attackmode = -1;
                    dest_local_x = local_x;
                }
            }

            if(attackmode == -1) ///start the attack
            {
                prev_dest_local_x = dest_local_x;

                attack_clock.restart();
                attackmode = 0;

                if(isFever)
                vspeed = -683;
            }

            if(attackmode == 0) ///begin the attack
            {
                canThrow = true;

                if(isFever)
                {
                    if(AnimatedObject::getAnimationSegment() != "attack_fever_jump")
                    AnimatedObject::setAnimationSegment("attack_fever_jump", true);

                    if(attack_clock.getElapsedTime().asMilliseconds() > 500)
                    {
                        AnimatedObject::setAnimationSegment("attack_fever_throw", true);
                        attack_clock.restart();
                        attackmode = 1;
                    }
                }
                else
                {
                    if(AnimatedObject::getAnimationSegment() != "attack_prefever_focused_start")
                    AnimatedObject::setAnimationSegment("attack_prefever_focused_start", true);

                    if(attack_clock.getElapsedTime().asMilliseconds() > 400)
                    {
                        AnimatedObject::setAnimationSegment("attack_prefever_focused_throw", true);
                        attack_clock.restart();
                        attackmode = 1;
                    }
                }
            }

            if(attackmode == 1) ///attack continously
            {
                if(isFever)
                {
                    float anim_speed_multiplier = float(1) / attack_speed;
                    framerate = float(1) * anim_speed_multiplier;

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
                else
                {
                    framerate = 1;

                    if(getAnimationPos() > 0.066)
                    {
                        if(canThrow)
                        {
                            threw = true;
                            canThrow = false;
                        }
                    }

                    if(AnimatedObject::getAnimationSegment() == "attack_prefever_focused_end")
                    {
                        attackmode = 2;
                    }
                }
            }

            if(attackmode == 2)
            {
                canThrow = true;
                action = IDLE;
            }
        }
    }

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

void Patapon::doRhythm(std::string current_song, std::string current_drum, int combo)
{
    if(!dead)
    {
        if(current_song == "patapata")
        {
            action = WALK;

            dest_local_x = 0;

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
            attackmode = -1;
            action = IDLE;

            if(!focus)
            setAnimationSegment(current_drum, true);
            else
            setAnimationSegment(current_drum+"_focused", true);
        }

        if(combo >= 11)
        isFever = true;
        else
        isFever = false;
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

    if(getUnitHP() <= 0)
    {
        if(!dead)
        {
            dead = true;
            deathClock.restart();
            hspeed = -400;
            vspeed = -250;
            action = IDLE;
            attackmode = -1;

            setAnimationSegment("stagger_var5", true);
        }
    }

    if(dead)
    {
        cout << "I'm dead now" << endl;

        if(getAnimationSegment() == "stagger_var5")
        {
            cout << "Animation segment is stagger_var5 " << cur_pos << " " << anim_end << endl;

            if(cur_pos >= anim_end)
            {
                cout << "Setting death animation" << endl;

                setAnimationSegment("death", true);
            }
        }

        if(deathClock.getElapsedTime().asSeconds() > 5)
        {
            cout << "Death clock passed 3 seconds. Time to bury into the ground" << endl;

            if(getAnimationSegment() == "death_corpse")
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

    //cout << "gclosest: " << gclosest << " lclosest: " << lclosest << " global_x: " << global_x << " local_x: " << local_x << endl;

    vspeed += gravity / fps;

    if(hspeed < 0)
    hspeed += 230.0 / fps;
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

    AnimatedObject::Draw(window);
}

void Patapon::OnCollide(CollidableObject* otherObject, int collidedWith, vector<string> collisionData)
{
    cout << "Patapon::OnCollide" << endl;

    if(collisionData.size() > 0)
    {
        if(isCollidable)
        {
            ///collisionData received from Projectile, process it
            int dmgDealt = atoi(collisionData[0].c_str());
            current_hp -= dmgDealt;

            cout << "I received " << to_string(dmgDealt) << "damage, my HP is " << current_hp << endl;
        }
    }

    /// note we don't call the parent function. It does nothing, it just serves
    /// as an incomplete function to be overridden by child classes.
}

