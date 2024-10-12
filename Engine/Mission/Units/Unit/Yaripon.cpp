#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "Yaripon.h"

#include <CoreManager.h>
#include <spdlog/spdlog.h>

Yaripon::Yaripon(int which, int maxpons)
{
    main.LoadConfig("resources/units/unit/yaripon.zip");
    main.setAnimation("idle_armed");

    order = which;
    maxp = maxpons;

    float max_army_spread = 660;

    double gap = 0;

    if(maxp == 1)
    {
        gap_x = 0;
    }
    else
    {
        gap = static_cast<double>(max_army_spread - 1) / (maxp - 1);
        gap_x = (max_army_spread / -2) + gap*order;
    }

    double center = max_army_spread / 2.0;  // Center of the space (325px in a 650px space)
    float unit = 120;

    if(maxp>6)
    {
        unit = max_army_spread / maxp;
    }

    // General formula for positioning units
    double pos;
    if (maxp % 2 == 1) {  // For odd number of units
        pos = center + ((order-1) - (maxp - 1) / 2.0) * unit;
    } else {  // For even number of units
        pos = center + ((order-1) - (maxp / 2.0 - 0.5)) * unit;
    }

    gap_x = pos;

    SpriteWrapper weapon;
    weapon.load("resources/graphics/item/textures/main/0014.png");
    weapon.setOrigin(weapon.getLocalBounds().width/2, weapon.getLocalBounds().height/2);
    main.addExtra(weapon, "weapon");

    SpriteWrapper helm;
    helm.load("resources/graphics/item/textures/main/0020.png");
    helm.setOrigin(helm.getLocalBounds().width/2, helm.getLocalBounds().height);
    main.addExtra(helm, "helm");
}

void Yaripon::Advance()
{
    float fps = CoreManager::getInstance().getCore()->getFPS();

    main.setAnimation("walk");
    if(enemyInSight)
        main.setAnimation("walk_focused");

    action = 2;
}

void Yaripon::Attack(int power)
{
    attackType = power;
    action = 1;
    globalRand = rand() % 10000;
    performedAttack = false;
    attackWalkTimer.restart();

    SPDLOG_DEBUG("Yaripon attack! type:{} distance:{} cur_x:{} attackspeed:{} canattackin:{}", attackType, distanceToTravel, global_x+local_x+attack_x, attackSpeed, canAttackIn);
}

void Yaripon::PerformAttack()
{
    float fps = CoreManager::getInstance().getCore()->getFPS();
    performedAttack = true;

    if(attackType == 1 || attackType == 2) // fever jump attack ponpon + chakachaka
    {
        if(!inAttackSequence)
        {
            inAttackTimer.restart();
            inAttackSequence = true;
            main.setAnimation("attack_fever_jump");
            main.restartAnimation();
        }
        else
        {
            auto BPM = CoreManager::getInstance().getSongController()->getBPM();

            int jumpStart = 100 / (BPM/120);
            int jumpEnd = 400 / (BPM/120);
            int attackEnd = 1500 / (BPM/120);

            if(inAttackTimer.getElapsedTime().asMilliseconds() > jumpStart && inAttackTimer.getElapsedTime().asMilliseconds() < jumpEnd && fabs(vspeed) <= 1)
            {
                vspeed = -1000;
            }

            if(inAttackTimer.getElapsedTime().asMilliseconds() >= jumpEnd && inAttackTimer.getElapsedTime().asMilliseconds() <= attackEnd)
            {
                if(canAttackIn <= 0)
                {
                    main.setAnimation("attack_fever_throw");
                    main.restartAnimation();
                    canAttackIn = attackSpeed / (BPM/120);
                    threw = false;
                }

                canAttackIn -= 1000 * (BPM/120) / fps;
            }

            if(main.getAnimation() == "attack_fever_throw" && main.getAnimationFrame() >= 8 && !threw)
            {
                if(attackType == 1) // ponpon
                    CoreManager::getInstance().getMissionController()->SendProjectile(global_x+local_x+attack_x+gap_x, global_y+local_y-90, 1800 + (globalRand%50)*1, -1800 - (globalRand%70)*1);
                if(attackType == 2) // chakachaka
                    CoreManager::getInstance().getMissionController()->SendProjectile(global_x+local_x+attack_x+gap_x, global_y+local_y-90, 2400 + (globalRand%50)*1, 1050 - (globalRand%70)*1);
                threw = true;
            }

            if(inAttackTimer.getElapsedTime().asMilliseconds() > attackEnd)
            {
                inAttackSequence = false;
                StopAttack();
            }
        }
    }

    if(attackType == 3 || attackType == 4) // prefever nojump attack ponpon + chakachaka
    {
        if(!inAttackSequence)
        {
            inAttackTimer.restart();
            inAttackSequence = true;
            main.setAnimation("attack_prefever_focused_start");
            main.restartAnimation();
        }
        else
        {
            auto BPM = CoreManager::getInstance().getSongController()->getBPM();

            int jumpStart = 100 / (BPM/120);
            int jumpEnd = 400 / (BPM/120);
            int attackEnd = 1500 / (BPM/120);

            if(inAttackTimer.getElapsedTime().asMilliseconds() >= jumpEnd && inAttackTimer.getElapsedTime().asMilliseconds() <= attackEnd)
            {
                if(canAttackIn <= 0)
                {
                    main.setAnimation("attack_prefever_focused_throw");
                    main.restartAnimation();
                    canAttackIn = attackSpeed / (BPM/120);
                    threw = false;
                }

                canAttackIn -= 1000 / fps;
            }

            if(main.getAnimation() == "attack_prefever_focused_throw" && main.getAnimationFrame() >= 2 && !threw)
            {
                if(attackType == 3) // ponpon
                    CoreManager::getInstance().getMissionController()->SendProjectile(global_x+local_x+attack_x+gap_x, global_y+local_y-90, 1600 + (globalRand%50)*1, -1500 - (globalRand%70)*1);
                if(attackType == 4) // chakachaka
                    CoreManager::getInstance().getMissionController()->SendProjectile(global_x+local_x+attack_x+gap_x, global_y+local_y-90, 2000 + (globalRand%50)*1, -300 - (globalRand%70)*1);
                threw = true;
            }

            if(inAttackTimer.getElapsedTime().asMilliseconds() > attackEnd)
            {
                inAttackSequence = false;
                StopAttack();
            }
        }
    }
}

void Yaripon::StopAttack() //soft stop, let them finish attacks
{
    if(!inAttackSequence)
    {
        if(main.getAnimation() == "attack_fever_jump" || main.getAnimation() == "attack_fever_throw" || main.getAnimation() == "attack_prefever_focused_start" || main.getAnimation() == "attack_prefever_focused_throw")
        {
            main.setAnimation("attack_fever_fall");
            main.restartAnimation();
        }

        if(attackSpeed <= 1000) // below this attack speed we should guarantee at least a single throw
        {
            if(canAttackIn >= attackSpeed*0.6)
                canAttackIn = 0;
        }

        if(action == 1)
            action = 0;
    }
}

void Yaripon::StopAll()
{
    if(main.getAnimation() != "pata" && main.getAnimation() != "pon" && main.getAnimation() != "don" && main.getAnimation() != "chaka")
    if(main.getAnimation() != "pata_focused" && main.getAnimation() != "pon_focused" && main.getAnimation() != "don_focused" && main.getAnimation() != "chaka_focused")
    {
        main.setAnimation("idle_armed");
        if(enemyInSight)
            main.setAnimation("idle_armed_focused");
    }

    if(action == 1)
        StopAttack();
    else
        action = 0;

    walkBack = true;
}

void Yaripon::PerformMissionEnd()
{
    if(!failure)
    {
        if(main.getAnimation() != "dance_var1" && main.getAnimation() != "dance_var2" && main.getAnimation() != "dance_var3" && main.getAnimation() != "dance_var4" && main.getAnimation() != "dance_var5")
        {
            main.setAnimation("walk");
        }
    }
    else
    {
        main.setAnimation("feverbroke_hold_var1");
    }
}

void Yaripon::Drum(std::string drum)
{
    if(!inAttackSequence && performedAttack)
    {
        StopAttack();

        main.setAnimation(drum);
        if(enemyInSight)
            main.setAnimation(drum+"_focused");
        main.restartAnimation();
    }
}

void Yaripon::Draw()
{
    float fps = CoreManager::getInstance().getCore()->getFPS();

    if(!missionEnd)
    {
        if(!enemyInSight)
        {
            if(inAttackSequence)
            {
                inAttackSequence = false;
                StopAttack();
            }

            if(attack_x != 0)
            {
                walkBack = true;
            }

            performedAttack = true;
        }
        else
        {
            distanceToTravel = closestEnemyX - global_x - local_x - attack_x - gap_x/2;

            if(action == 1 && (fabs(vspeed) < 1))
            {
                walkBack = false;
                bool doAttack = false;

                if(attackType == 1)
                {
                    if(distanceToTravel < 2000 - globalRand%10*5) // too close
                    {
                        main.setAnimation("walk_focused");
                        pataCurMaxSpeed = pataMaxSpeed * 2.4 / (1 + ((globalRand%5)+5) * 0.02);
                        pataSpeed -= pataCurMaxSpeed*decelerationFactor / fps;
                        if(pataSpeed < -pataCurMaxSpeed)
                            pataSpeed = -pataCurMaxSpeed;
                    }
                    else if (distanceToTravel > 2200 + globalRand%10*5) // too far
                    {
                        main.setAnimation("walk_focused");
                        pataCurMaxSpeed = pataMaxSpeed * 2.4 / (1 + ((globalRand%5)+5) * 0.02);
                        pataSpeed += pataCurMaxSpeed*accelerationFactor / fps;
                        if(pataSpeed > pataCurMaxSpeed)
                            pataSpeed = pataCurMaxSpeed;
                    }
                    else
                    {
                        if(attackWalkTimer.getElapsedTime().asMilliseconds() < 300 + 10*(globalRand%20))
                        {
                            main.setAnimation("walk_focused");
                            pataCurMaxSpeed = pataMaxSpeed / 2 / (((globalRand%5)+5) * 0.2);
                            int neg = (((globalRand%2) - 0.5) * 2);
                            pataSpeed += neg*pataCurMaxSpeed*accelerationFactor / fps;
                            if(pataSpeed > pataCurMaxSpeed)
                                pataSpeed = pataCurMaxSpeed;
                            if(pataSpeed < -pataCurMaxSpeed)
                                pataSpeed = -pataCurMaxSpeed;
                            //SPDLOG_DEBUG("Yaripon {} is moving. {} {} {}", order, pataCurMaxSpeed, pataSpeed, attackWalkTimer.getElapsedTime().asMilliseconds());
                        }
                        else
                        {
                            //SPDLOG_DEBUG("ATTACK!!!!");
                            doAttack = true;
                        }
                    }
                }
                else if(attackType == 2)
                {
                    if(distanceToTravel < 800 - globalRand%10*5)
                    {
                        main.setAnimation("walk_focused");
                        pataCurMaxSpeed = pataMaxSpeed * 2.4 / (1 + ((globalRand%5)+5) * 0.02);
                        pataSpeed -= pataCurMaxSpeed*decelerationFactor / fps;
                        if(pataSpeed < -pataCurMaxSpeed)
                            pataSpeed = -pataCurMaxSpeed;
                    }
                    else if (distanceToTravel > 1000 + globalRand%10*5)
                    {
                        main.setAnimation("walk_focused");
                        pataCurMaxSpeed = pataMaxSpeed * 2.4 / (1 + ((globalRand%5)+5) * 0.02);
                        pataSpeed += pataCurMaxSpeed*accelerationFactor / fps;
                        if(pataSpeed > pataCurMaxSpeed)
                            pataSpeed = pataCurMaxSpeed;
                    }
                    else
                    {
                        if(attackWalkTimer.getElapsedTime().asMilliseconds() < 300 + 10*(globalRand%20))
                        {
                            main.setAnimation("walk_focused");
                            pataCurMaxSpeed = pataMaxSpeed / 2 / (((globalRand%5)+5) * 0.2);
                            int neg = (((globalRand%2) - 0.5) * 2);
                            pataSpeed += neg*pataCurMaxSpeed*accelerationFactor / fps;
                            if(pataSpeed > pataCurMaxSpeed)
                                pataSpeed = pataCurMaxSpeed;
                            if(pataSpeed < -pataCurMaxSpeed)
                                pataSpeed = -pataCurMaxSpeed;
                            //SPDLOG_DEBUG("Yaripon {} is moving. {} {} {}", order, pataCurMaxSpeed, pataSpeed, attackWalkTimer.getElapsedTime().asMilliseconds());
                        }
                        else
                        {
                            //SPDLOG_DEBUG("ATTACK!!!!");
                            doAttack = true;
                        }
                    }
                }
                else if(attackType == 3)
                {
                    if(distanceToTravel < 1400 - globalRand%10*5) // too close
                    {
                        main.setAnimation("walk_focused");
                        pataCurMaxSpeed = pataMaxSpeed * 2.4 / (1 + ((globalRand%5)+5) * 0.02);
                        pataSpeed -= pataCurMaxSpeed*decelerationFactor / fps;
                        if(pataSpeed < -pataCurMaxSpeed)
                            pataSpeed = -pataCurMaxSpeed;
                    }
                    else if (distanceToTravel > 1600 + globalRand%10*5) // too far
                    {
                        main.setAnimation("walk_focused");
                        pataCurMaxSpeed = pataMaxSpeed * 2.4 / (1 + ((globalRand%5)+5) * 0.02);
                        pataSpeed += pataCurMaxSpeed*accelerationFactor / fps;
                        if(pataSpeed > pataCurMaxSpeed)
                            pataSpeed = pataCurMaxSpeed;
                    }
                    else
                    {
                        if(attackWalkTimer.getElapsedTime().asMilliseconds() < 300 + 10*(globalRand%20))
                        {
                            main.setAnimation("walk_focused");
                            pataCurMaxSpeed = pataMaxSpeed / 2 / (((globalRand%5)+5) * 0.2);
                            int neg = (((globalRand%2) - 0.5) * 2);
                            pataSpeed += neg*pataCurMaxSpeed*accelerationFactor / fps;
                            if(pataSpeed > pataCurMaxSpeed)
                                pataSpeed = pataCurMaxSpeed;
                            if(pataSpeed < -pataCurMaxSpeed)
                                pataSpeed = -pataCurMaxSpeed;
                            //SPDLOG_DEBUG("Yaripon {} is moving. {} {} {}", order, pataCurMaxSpeed, pataSpeed, attackWalkTimer.getElapsedTime().asMilliseconds());
                        }
                        else
                        {
                            //SPDLOG_DEBUG("ATTACK!!!!");
                            doAttack = true;
                        }
                    }
                }
                else if(attackType == 4)
                {
                    if(distanceToTravel < 800 - globalRand%10*5)
                    {
                        main.setAnimation("walk_focused");
                        pataCurMaxSpeed = pataMaxSpeed * 2.4 / (1 + ((globalRand%5)+5) * 0.02);
                        pataSpeed -= pataCurMaxSpeed*decelerationFactor / fps;
                        if(pataSpeed < -pataCurMaxSpeed)
                            pataSpeed = -pataCurMaxSpeed;
                    }
                    else if (distanceToTravel > 1000 + globalRand%10*5)
                    {
                        main.setAnimation("walk_focused");
                        pataCurMaxSpeed = pataMaxSpeed * 2.4 / (1 + ((globalRand%5)+5) * 0.02);
                        pataSpeed += pataCurMaxSpeed*accelerationFactor / fps;
                        if(pataSpeed > pataCurMaxSpeed)
                            pataSpeed = pataCurMaxSpeed;
                    }
                    else
                    {
                        if(attackWalkTimer.getElapsedTime().asMilliseconds() < 300 + 10*(globalRand%20))
                        {
                            main.setAnimation("walk_focused");
                            pataCurMaxSpeed = pataMaxSpeed / 2 / (((globalRand%5)+5) * 0.2);
                            int neg = (((globalRand%2) - 0.5) * 2);
                            pataSpeed += neg*pataCurMaxSpeed*accelerationFactor / fps;
                            if(pataSpeed > pataCurMaxSpeed)
                                pataSpeed = pataCurMaxSpeed;
                            if(pataSpeed < -pataCurMaxSpeed)
                                pataSpeed = -pataCurMaxSpeed;
                            //SPDLOG_DEBUG("Yaripon {} is moving. {} {} {}", order, pataCurMaxSpeed, pataSpeed, attackWalkTimer.getElapsedTime().asMilliseconds());
                        }
                        else
                        {
                            //SPDLOG_DEBUG("ATTACK!!!!");
                            doAttack = true;
                        }
                    }
                }

                if(doAttack)
                {
                    pataSpeed = 0;
                    PerformAttack();
                }

                if(fabs(vspeed) < 1)
                    attack_x += pataSpeed / fps;
            }
            else if(action == 1 && fabs(vspeed) >= 1)
            {
                pataSpeed = 0;
                PerformAttack();
            }
        }

        if(action == 0 && !walkBack)
        {
            pataSpeed *= 0.9;
        }

        if(action == 2)
        {
            walkBack = true;
        }

        if(floor(fabs(attack_x)) > 0)
        {
            if(walkBack && (fabs(vspeed) < 1))
            {
                performedAttack = true;

                pataSpeed += pataMaxSpeed*accelerationFactor / fps;

                if(pataSpeed < 0)
                    pataSpeed = 0;

                if(pataSpeed > pataMaxSpeed)
                    pataSpeed = pataMaxSpeed;

                if(attack_x > 0)
                {
                    attack_x -= pataSpeed / fps;
                }
                if(attack_x < 0)
                {
                    attack_x += pataSpeed / fps;
                }

                if(main.getAnimation() != "pata" && main.getAnimation() != "pon" && main.getAnimation() != "don" && main.getAnimation() != "chaka")
                    if(main.getAnimation() != "pata_focused" && main.getAnimation() != "pon_focused" && main.getAnimation() != "don_focused" && main.getAnimation() != "chaka_focused")
                    {
                        main.setAnimation("walk");
                        if(enemyInSight)
                            main.setAnimation("walk_focused");
                    }
            }
        } else
        {
            walkBack = false;
        }
    }
    else
    {
        PerformMissionEnd();
    }

    vspeed += gravity / fps;
    local_y += vspeed / fps;

    if(local_y >= 0)
    {
        local_y = 0;
        vspeed = 0;
    }

    main.setGlobalPosition(sf::Vector2f(global_x, global_y));
    main.setLocalPosition(sf::Vector2f(local_x+attack_x+gap_x, local_y));
    main.Draw();

    if(toggleDebug)
    {
        auto strRepo = CoreManager::getInstance().getStrRepo();
        debugText.setFont(strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage()));
        debugText.setCharacterSize(12);
        debugText.setString(std::format("o\n{}\nact\n{}\natk_x\n{}\npS\n{}\naf\n{}\nias\n{}\npA\n{}\niat\n{}\ncai\n{:2.2f}",order, action, attack_x, pataSpeed, main.getAnimationFrame(), inAttackSequence, performedAttack, inAttackTimer.getElapsedTime().asMilliseconds(), canAttackIn));
        debugText.disable_processing = true;
        debugText.setOrigin(debugText.getLocalBounds().width/2, debugText.getLocalBounds().height);
        debugText.setPosition(global_x+local_x+attack_x+gap_x-20, global_y+local_y-100);
        debugText.draw();
    }
}