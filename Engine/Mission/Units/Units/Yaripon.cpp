#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "Yaripon.h"
#include "../../../Func.h"
#include "../../../V4Core.h"
#include "../../../CoreManager.h"
#include <cmath>
#include <iostream>
#include <spdlog/spdlog.h>


Yaripon::Yaripon()
{
}
void Yaripon::LoadConfig()
{
    /// load patapon from p4a file
    AnimatedObject::LoadConfig("resources/units/unit/yaripon.p4a");
    setAnimationSegment("idle_armed");

    spear_throw.loadFromFile("resources/sfx/level/spear_throw.ogg");

    hit_1.loadFromFile("resources/sfx/level/hit_1.ogg");
    hit_2.loadFromFile("resources/sfx/level/hit_2.ogg");
    hit_3.loadFromFile("resources/sfx/level/hit_3.ogg");

    s_dead.loadFromFile("resources/sfx/level/dead_2.ogg");

    cur_sound.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));

    isCollidable = true;
    isAttackable = true;
}

void Yaripon::startAttack()
{
    if (action != ATTACK)
    {
        action = ATTACK;
        attackMode = AttackMode::Preparing;
        walkClock.restart();
        dest_set = false;
    }
}

bool Yaripon::doAttack()
{
    if (action == ATTACK)
    {
        if (enemy_in_range)
        {
            if (attackMode == AttackMode::Preparing) ///prepare the attack (move around)
            {
                ///Calculate the local x
                if (!dest_set)
                {
                    if (entity_distance < low_throw_range) ///unit is too close, go back (distance = 250)
                    {
                        dest_local_x = -(low_throw_range - entity_distance) - (rand() % 50);                  ///take a random position farther from the enemy
                    } else if ((entity_distance >= low_throw_range) && (entity_distance <= high_throw_range)) ///perfect spot, make some tiny adjustments and start attacking
                    {
                        ///320-370
                        dest_local_x += rand() % 40 - 20;
                    } else if (entity_distance > high_throw_range) ///unit is too far, get closer (distance = 400)
                    {
                        dest_local_x = (entity_distance - high_throw_range) + (rand() % 50); ///take a random position closer the enemy
                    }

                    dest_set = true;

                    cout << "Setting dest_local_x to " << dest_local_x << " (prev: " << prev_dest_local_x << ") (entity_distance: " << entity_distance << ")" << endl;
                }

                //dest_local_x = entity_distance;

                if ((local_x >= dest_local_x - 5) && (local_x <= dest_local_x + 5)) ///When you are set at your position, start the attack
                    attackMode = AttackMode::Ready;

                if (walkClock.getElapsedTime().asMilliseconds() >= 750) ///If walking takes you too long, stop and start the attack
                {
                    attackMode = AttackMode::Ready;
                    dest_local_x = local_x;
                }
            }

            if (attackMode == AttackMode::Ready) ///start the attack
            {
                prev_dest_local_x = dest_local_x;

                attack_clock.restart();
                attackMode = AttackMode::Start;

                if (isFever)
                    vspeed = -683;
            }

            if (attackMode == AttackMode::Start) ///begin the attack
            {
                canAttack = true;

                if (isFever)
                {
                    if (AnimatedObject::getAnimationSegment() != "attack_fever_jump")
                    {
                        spdlog::trace("Attacking jump in fever mode");
                        AnimatedObject::setAnimationSegment("attack_fever_jump", true);
                    }

                    if (attack_clock.getElapsedTime().asMilliseconds() > 500)
                    {
                        AnimatedObject::setAnimationSegment("attack_fever_throw", true);
                        attack_clock.restart();
                        attackMode = AttackMode::Attacking;
                    }
                } else
                {
                    if (AnimatedObject::getAnimationSegment() != "attack_prefever_focused_start")
                        AnimatedObject::setAnimationSegment("attack_prefever_focused_start", true);

                    if (attack_clock.getElapsedTime().asMilliseconds() > 400)
                    {
                        AnimatedObject::setAnimationSegment("attack_prefever_focused_throw", true);
                        attack_clock.restart();
                        attackMode = AttackMode::Attacking;
                    }
                }
            }

            if (attackMode == AttackMode::Attacking) ///attack continously
            {
                if (isFever)
                {
                    float anim_speed_multiplier = float(1) / attack_speed;
                    framerate = float(1) * anim_speed_multiplier;

                    if (getAnimationPos() > 0.285)
                    {
                        if (canAttack)
                        {
                            cur_sound.stop();
                            cur_sound.setBuffer(spear_throw);
                            cur_sound.play();

                            attacked = true;
                            canAttack = false;
                        }
                    }

                    if (attack_clock.getElapsedTime().asSeconds() > attack_speed)
                    {
                        if (canAttack)
                        {
                            if (!attacked)
                                attacked = true;
                        }

                        AnimatedObject::setAnimationSegment("attack_fever_throw", true);
                        attack_clock.restart();

                        canAttack = true;
                    }

                    if (local_y >= 0)
                    {
                        AnimatedObject::setAnimationSegment("idle_armed_focused", true);
                        attackMode = AttackMode::AfterAttack;
                    }
                } else
                {
                    framerate = 1;

                    if (getAnimationPos() > 0.066)
                    {
                        if (canAttack)
                        {
                            cur_sound.stop();
                            cur_sound.setBuffer(spear_throw);
                            cur_sound.play();

                            attacked = true;
                            canAttack = false;
                        }
                    }

                    if (AnimatedObject::getAnimationSegment() == "attack_prefever_focused_end")
                    {
                        attackMode = AttackMode::AfterAttack;
                    }
                }
            }

            if (attackMode == AttackMode::AfterAttack)
            {
                canAttack = true;
                action = IDLE;
            }
        }
    }

    if (attacked)
    {
        attacked = false;
        return true;
    } else
    {
        return false;
    }
}

void Yaripon::Update()
{
    if (current_hp <= 0)
    {
        if (!dead)
        {
            dying = true;
            hspeed = -400;
            vspeed = -250;
            action = IDLE;
            attackMode = AttackMode::Ready;
        }
    }

    vspeed += gravity / fps;

    if (hspeed < 0)
        hspeed += 230.0f / fps;
    else
        hspeed = 0;

    if (vspeed >= 0)
    {
        if (local_y >= 0)
        {
            vspeed = 0;
            local_y = 0;
        }
    }

    local_x += hspeed / fps;
    local_y += vspeed / fps;
}

// State update -> Animation -> Draw
void Yaripon::Draw(sf::RenderWindow& window)
{
    if (AnimatedObject::getAnimationSegment() == "walk")
    {
        framerate = 0.8;
    } else
    {
        framerate = 1;
    }

    if (dying)
    {
        deathClock.restart();
        setAnimationSegment("stagger_var5", true);
        dying = false;
        dead = true;
    } else if (dead)
    {
        spdlog::trace("I'm dead now");

        if (getAnimationSegment() == "stagger_var5")
        {
            spdlog::trace("Animation segment is stagger_var5 {} {}", cur_pos, anim_end);

            if (cur_pos >= anim_end)
            {
                spdlog::trace("Setting death animation");

                setAnimationSegment("death", true);

                cur_sound.stop();
                cur_sound.setBuffer(s_dead);
                cur_sound.play();
            }
        }

        if (deathClock.getElapsedTime().asSeconds() > 5)
        {
            spdlog::trace("Death clock passed 3 seconds. Time to bury into the ground");

            if (getAnimationSegment() == "death_corpse")
            {
                spdlog::trace("I am despawning");
                setAnimationSegment("death_despawn", true);
            }

            if (getAnimationSegment() == "death_despawn")
            {
                if (cur_pos >= anim_end)
                {
                    ready_to_erase = true;
                }
            }
        }
    }

    AnimatedObject::Draw(window);
}

void Yaripon::UpdateRhythm(std::string current_song, std::string current_drum, int combo)
{
    if (!dead)
    {
        if (!dead)
        {
            focus = enemy_in_range;
        }

        if ((current_song == "patapata") || (current_song == "chakapata"))
        {
            action = WALK;

            dest_local_x = 0;
        }

        if (current_song == "ponpon")
        {
            ///use attack only once
            if (attackMode != AttackMode::AfterAttack)
                startAttack();

            if (isFever)
            {
                high_throw_range = 410;
                low_throw_range = 360;
            } else
            {
                high_throw_range = 320;
                low_throw_range = 270;
            }
        } else if (current_song == "chakachaka")
        {
            defend = true;

            ///use attack only once
            if (attackMode != AttackMode::AfterAttack)
                startAttack();

            if (isFever)
            {
                high_throw_range = 250;
                low_throw_range = 200;
            } else
            {
                high_throw_range = 320;
                low_throw_range = 270;
            }
        } else
        {
            ///refresh attack mode
            attackMode = AttackMode::Ready;
        }

        if ((current_song != "ponpon") && (current_song != "chakachaka"))
        {
            dest_local_x = 0;
        }

        if (current_song != "chakachaka")
        {
            defend = false;
        }

        if ((current_drum == "pata") || (current_drum == "pon") || (current_drum == "chaka") || (current_drum == "don"))
        {
            attackMode = AttackMode::Ready;
            action = IDLE;
        }

        if (!current_song.empty())
        {
            if (current_song == "ponchaka")
            {
                charge_m1 = true;
                charged = false;
            } else
            {
                if (charge_m1)
                {
                    if (!charged)
                        charged = true;
                    else
                        charge_m1 = false;
                } else if (current_song != old_current_song)
                {
                    charged = false;
                }
            }
        }

        old_current_song = current_song;

        if (combo < 2)
        {
            defend = false;
            charged = false;
            charge_m1 = false;
        }

        isFever = combo >= 11;
    }
}

void Yaripon::doRhythm(std::string current_song, std::string current_drum, int combo)
{
    if (!dead)
    {
        if ((current_song == "patapata") || (current_song == "chakapata"))
        {
            if (!focus)
            {
                if (getAnimationSegment() != "walk")
                    setAnimationSegment("walk", true);
            } else
            {
                if (getAnimationSegment() != "walk_focused")
                    setAnimationSegment("walk_focused", true);
            }
        } else
        {
            if (getAnimationSegment() == "walk")
            {
                if (!getback)
                {
                    if (!focus)
                        setAnimationSegment("idle_armed", true);
                    else
                        setAnimationSegment("idle_armed_focused", true);
                }
            }
        }

        if (!current_song.empty())
        {
            if (current_song == "ponchaka")
            {
                if ((getAnimationSegment() != "charge_start") && (getAnimationSegment() != "charge_intact"))
                    setAnimationSegment("charge_start", true);
            }
        }

        if ((current_drum == "pata") || (current_drum == "pon") || (current_drum == "chaka") || (current_drum == "don"))
        {
            if (!focus)
                setAnimationSegment(current_drum, true);
            else
                setAnimationSegment(current_drum + "_focused", true);
        }

        if ((getAnimationSegment().find("pata") != std::string::npos) || (getAnimationSegment().find("pon") != std::string::npos) || (getAnimationSegment().find("don") != std::string::npos) || (getAnimationSegment().find("chaka") != std::string::npos))
        {
            if (cur_pos >= anim_end - 0.0333)
            {
                if (!focus)
                {
                    setAnimationSegment("idle_armed", true);
                } else
                {
                    setAnimationSegment("idle_armed_focused", true);
                }
            }
        }

        if (combo < 2)
        {
            if ((getAnimationSegment() == "charge_intact") || (getAnimationSegment() == "walk") || (getAnimationSegment() == "walk_focused"))
            {
                if (!focus)
                {
                    setAnimationSegment("idle_armed", true);
                } else
                {
                    setAnimationSegment("idle_armed_focused", true);
                }
            }

            if (focus)
            {
                if (getAnimationSegment() == "idle_armed")
                    setAnimationSegment("idle_armed_focused", true);
            } else
            {
                if (getAnimationSegment() == "idle_armed_focused")
                    setAnimationSegment("idle_armed", true);
            }
        }
    }
}

void Yaripon::doMissionEnd()
{
    if (getAnimationSegment().find("dance") == std::string::npos)
    {
        setAnimationSegment("walk");

        if (partyClock.getElapsedTime().asMilliseconds() > 250)
        {
            if (rand() % 4 == 1)
            {
                int a = rand() % 5 + 1;
                string anim = "dance_var" + to_string(a);

                setAnimationSegment(anim, true);
            }

            partyClock.restart();
        }
    } else
    {
        if (cur_pos > anim_end)
        {
            setAnimationSegment("walk");
        }
    }
}

void Yaripon::OnCollide(CollidableObject* otherObject, int collidedWith, vector<string> collisionData)
{
    cout << "Patapon::OnCollide" << endl;

    if (collisionData.size() > 0)
    {
        if (isCollidable)
        {
            ///collisionData received from Projectile, process it
            int dmgDealt = atoi(collisionData[0].c_str());

            if (defend)
            {
                if (charged)
                    dmgDealt = round(dmgDealt / 4);
                else
                    dmgDealt = round(dmgDealt / 2);
            }

            current_hp -= dmgDealt;

            cout << "I received " << to_string(dmgDealt) << "damage, my HP is " << current_hp << endl;

            cur_sound.stop();

            int a = rand() % 3;

            switch (a)
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
