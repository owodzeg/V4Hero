#include "Tatepon.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include "../../../Func.h"
#include <sstream>
Tatepon::Tatepon()
{

}
void Tatepon::LoadConfig(Config *thisConfigs)
{
    /// load patapon from p4a file
    AnimatedObject::LoadConfig(thisConfigs,"resources\\units\\unit\\yaripon.p4a");
    setAnimationSegment("idle_armed");

    sword_swing.loadFromFile("resources/sfx/level/spear_throw.ogg");

    hit_1.loadFromFile("resources/sfx/level/hit_1.ogg");
    hit_2.loadFromFile("resources/sfx/level/hit_2.ogg");
    hit_3.loadFromFile("resources/sfx/level/hit_3.ogg");

    s_dead.loadFromFile("resources/sfx/level/dead_2.ogg");

    cur_sound.setVolume(float(thisConfigs->GetInt("masterVolume"))*(float(thisConfigs->GetInt("sfxVolume"))/100.f));


    can_shield = true;
    isCollidable = true;
    isAttackable = true;
    far_attack_range = 32;
    near_attack_range = 0;
}

void Tatepon::startAttack()
{
    if(action != ATTACK)
    {
        action = ATTACK;
        attackmode = -2;
        walkClock.restart();
        dest_set = false;
    }
}

bool Tatepon::doAttack()
{
    cout << "[DEBUG] Tatepon tried to attack!" << endl;
    if(action == ATTACK)
    {
        cout << "[DEBUG] Tatepon tried to attack (action)!" << endl;
        if(enemy_in_range)
        {
            if(attackmode == -2) ///prepare the attack (move around)
            {
                ///Calculate the local x
                if(!dest_set)
                {
                    if((entity_distance >= near_attack_range) && (entity_distance <= far_attack_range)) ///perfect spot, make some tiny adjustments and start attacking
                    {
                        cout << "[DEBUG] Tatepon is positioned well, setting dest_local_x to " << entity_distance - far_attack_range << endl;
                        dest_local_x = entity_distance - far_attack_range; ///Get as close to enemy as possible
                    }
                    else if(entity_distance > far_attack_range) ///unit is too far, get closer
                    {
                        cout << "[DEBUG] Tatepon has to get closer, setting dest_local_x to " << entity_distance + (far_attack_range + near_attack_range) / 2 << endl;
                        dest_local_x = entity_distance + (far_attack_range + near_attack_range) / 2; ///take a position closer the enemy
                    }

                    dest_set = true;

                    cout << "Setting dest_local_x to " << dest_local_x << " (prev: " << prev_dest_local_x << ") (entity_distance: " << entity_distance << ")" << endl;
                }

                //dest_local_x = entity_distance;

                if((local_x >= dest_local_x-5) && (local_x <= dest_local_x+5)) ///When you are set at your position, start the attack
                attackmode = -1;

                if(walkClock.getElapsedTime().asMilliseconds() >= 1000) ///If walking takes you too long, stop and defend (slowly walking back)
                {
                    action = DEFEND;
                    dest_local_x = local_x;
                }
            }

            if(attackmode == -1) ///start the attack
            {
                prev_dest_local_x = dest_local_x;

                attack_clock.restart();
                attackmode = 0;

                if(isFever) ///Weaken knockback if fever?
                vspeed = -683;
            }

            if(attackmode == 0) ///begin the attack
            {
                canAttack = true;

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

                    if(attack_clock.getElapsedTime().asMilliseconds() > 600)
                    {
                        AnimatedObject::setAnimationSegment("attack_prefever_focused_throw", true);
                        attack_clock.restart();
                        attackmode = 1;
                    }
                }
            }

            if(attackmode == 1) ///attack continuously
            {
                if(isFever)
                {
                    float anim_speed_multiplier = float(1) / attack_speed;
                    framerate = float(1) * anim_speed_multiplier;

                    if(getAnimationPos() > 0.285)
                    {
                        if(canAttack)
                        {
                            cur_sound.stop();
                            cur_sound.setBuffer(sword_swing);
                            cur_sound.play();

                            attacked = true;
                            canAttack = false;
                        }
                    }

                    if(attack_clock.getElapsedTime().asSeconds() > attack_speed)
                    {
                        if(canAttack)
                        {
                            if(attacked == false)
                            attacked = true;
                        }

                        AnimatedObject::setAnimationSegment("attack_fever_throw", true);
                        attack_clock.restart();

                        canAttack = true;
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
                        if(canAttack)
                        {
                            cur_sound.stop();
                            cur_sound.setBuffer(sword_swing);
                            cur_sound.play();

                            attacked = true;
                            canAttack = false;
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
                canAttack = true;
                action = IDLE;
            }
        }
    }

    if(attacked)
    {
        attacked = false;
        return true;
    }
    else
    {
        return false;
    }
}

void Tatepon::doRhythm(std::string current_song, std::string current_drum, int combo)
{
    if(!dead)
    {
        if(enemy_in_range)
        focus = true;
        else
        focus = false;

        if((current_song == "patapata") || (current_song == "chakapata"))
        {
            action = WALK;

            dest_local_x = 0;

            if(!focus)
            {
                if(getAnimationSegment() != "walk")
                setAnimationSegment("walk", true);
            }
            else
            {
                if(getAnimationSegment() != "walk_focused")
                setAnimationSegment("walk_focused", true);
            }
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
        else if(current_song == "chakachaka")
        {
            defend = true;
        }
        else
        {
            ///refresh attack mode
            attackmode = -1;
        }

        if(current_song != "")
        {
            if(current_song == "ponchaka")
            {
                if((getAnimationSegment() != "charge_start") && (getAnimationSegment() != "charge_intact"))
                setAnimationSegment("charge_start", true);

                charge_m1 = true;
                charged = false;
            }
            else
            {
                if(charge_m1)
                {
                    if(charged == false)
                    charged = true;
                    else
                    charge_m1 = false;
                }
                else if(current_song != old_current_song)
                {
                    charged = false;
                }
            }

            if((current_song != "ponpon") && (current_song != "chakachaka"))
            {
                dest_local_x = 0;
            }

            if(current_song != "chakachaka")
            defend = false;
        }

        old_current_song = current_song;

        //cout << "new comms: " << charged << " " << charge_m1 << " " << defend << endl;

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

        if((getAnimationSegment().find("pata") != std::string::npos) || (getAnimationSegment().find("pon") != std::string::npos) || (getAnimationSegment().find("don") != std::string::npos) || (getAnimationSegment().find("chaka") != std::string::npos))
        {
            if(cur_pos >= anim_end-0.0333)
            {
                if(!focus)
                {
                    setAnimationSegment("idle_armed", true);
                }
                else
                {
                    setAnimationSegment("idle_armed_focused", true);
                }
            }
        }

        if(combo < 2)
        {
            if((getAnimationSegment() == "charge_intact") || (getAnimationSegment() == "walk") || (getAnimationSegment() == "walk_focused"))
            {
                if(!focus)
                {
                    setAnimationSegment("idle_armed", true);
                }
                else
                {
                    setAnimationSegment("idle_armed_focused", true);
                }
            }

            if(focus)
            {
                if(getAnimationSegment() == "idle_armed")
                setAnimationSegment("idle_armed_focused", true);
            }
            else
            {
                if(getAnimationSegment() == "idle_armed_focused")
                setAnimationSegment("idle_armed", true);
            }

            defend = false;
            charged = false;
            charge_m1 = false;
        }

        if(combo >= 11)
        isFever = true;
        else
        isFever = false;
    }
}

void Tatepon::doMissionEnd()
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

void Tatepon::Draw(sf::RenderWindow& window)
{
    //cout << "Patapon: " << getAnimationSegment() << " " << getAnimationPos() << "/" << getAnimationLength(getAnimationSegment()) << " " << curFrame << " " << index << " " << floor(getAnimationLength(getAnimationSegment()) * animation_framerate) - 1 << " " << animation_framerate << endl;

    if(AnimatedObject::getAnimationSegment() == "walk")
    {
        framerate = 0.8;
    }
    else
    {
        framerate = 1;
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

                cur_sound.stop();
                cur_sound.setBuffer(s_dead);
                cur_sound.play();
            }
        }

        if(deathClock.getElapsedTime().asSeconds() > 3)
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
    else
    {
        if(old_current_song == "chakachaka")
        {
            cout << "[DEBUG] ChakaChaka in draw function" << endl;
            //thisConfig->thisCore->saveReader.ponreg.GetPonByID(0);
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

void Tatepon::OnCollide(CollidableObject* otherObject, int collidedWith, vector<string> collisionData)
{
    cout << "Patapon::OnCollide" << endl;

    if(collisionData.size() > 0)
    {
        if(isCollidable)
        {
            ///collisionData received from Projectile, process it
            int dmgDealt = atoi(collisionData[0].c_str());

            if(defend)
            {
                if(charged)
                dmgDealt = round(dmgDealt / 4);
                else
                dmgDealt = round(dmgDealt / 2);
            }

            current_hp -= dmgDealt;

            cout << "I received " << to_string(dmgDealt) << "damage, my HP is " << current_hp << endl;

            cur_sound.stop();

            int a = rand() % 3;

            switch(a)
            {
                case 0:
                cur_sound.setBuffer(hit_1);
                break;

                case 1:
                cur_sound.setBuffer(hit_2);
                break;

                case 2:
                cur_sound.setBuffer(hit_3);
                break;
            }

            cur_sound.play();
        }
    }

    /// note we don't call the parent function. It does nothing, it just serves
    /// as an incomplete function to be overridden by child classes.
}

