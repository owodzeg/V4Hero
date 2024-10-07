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

    float max_army_spread = 650;

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
    attackWalkTimer.restart();

    SPDLOG_DEBUG("Yaripon attack! type:{} distance:{} cur_x:{} attackspeed:{} canattackin:{}", attackType, distanceToTravel, global_x+local_x+attack_x, attackSpeed, canAttackIn);
}

void Yaripon::PerformAttack()
{
    float fps = CoreManager::getInstance().getCore()->getFPS();

    if(!inAttackSequence)
    {
        inAttackTimer.restart();
        inAttackSequence = true;
        main.setAnimation("attack_fever_jump");
        main.restartAnimation();
    }
    else
    {
        int jumpStart = 100;
        int jumpEnd = 400;
        int attackEnd = 1500;

        if(inAttackTimer.getElapsedTime().asMilliseconds() > jumpStart && inAttackTimer.getElapsedTime().asMilliseconds() < jumpEnd)
            vspeed = -1000;

        if(inAttackTimer.getElapsedTime().asMilliseconds() >= jumpEnd && inAttackTimer.getElapsedTime().asMilliseconds() <= attackEnd)
        {
            if(canAttackIn <= 0)
            {
                main.setAnimation("attack_fever_throw");
                main.restartAnimation();
                canAttackIn = attackSpeed;
            }

            canAttackIn -= 1000 / fps;
        }

        if(inAttackTimer.getElapsedTime().asMilliseconds() > attackEnd)
        {
            StopAttack();
        }
    }
}

void Yaripon::StopAttack()
{
    if(main.getAnimation() == "attack_fever_jump" || main.getAnimation() == "attack_fever_throw")
    {
        main.setAnimation("attack_fever_fall");
        main.restartAnimation();
    }

    inAttackSequence = false;

    if(attackSpeed <= 1000) // below this attack speed we should guarantee at least a single throw
    {
        if(canAttackIn >= attackSpeed*0.6)
            canAttackIn = 0;
    }

    if(action == 1)
        action = 0;
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

    StopAttack();
    action = 0;
}

void Yaripon::Drum(std::string drum)
{
    StopAttack();

    main.setAnimation(drum);
    if(enemyInSight)
        main.setAnimation(drum+"_focused");
    main.restartAnimation();
}

void Yaripon::Draw()
{
    float fps = CoreManager::getInstance().getCore()->getFPS();

    if(!enemyInSight)
    {
        pataSpeed -= pataMaxSpeed*decelerationFactor / fps;
        if(pataSpeed < 0)
            pataSpeed = 0;

        if(attack_x-pataSpeed < 0)
            attack_x = 0;
        else if(attack_x+pataSpeed > 0)
            attack_x = 0;

        if(attack_x < 0)
            attack_x -= pataSpeed / fps;
        else if(attack_x > 0)
            attack_x += pataSpeed / fps;
        else
            attack_x = 0;
    }
    else
    {
        distanceToTravel = closestEnemyX - global_x - local_x - attack_x;

        if(action == 1)
        {
            if(distanceToTravel < 1200)
            {
                main.setAnimation("walk_focused");
                pataSpeed -= pataMaxSpeed*decelerationFactor / fps;
                if(pataSpeed < -pataMaxSpeed)
                    pataSpeed = -pataMaxSpeed;
            }
            else if (distanceToTravel > 1800)
            {
                main.setAnimation("walk_focused");
                pataSpeed += pataMaxSpeed*accelerationFactor / fps;
                if(pataSpeed > pataMaxSpeed)
                    pataSpeed = pataMaxSpeed;
            }
            else
            {
                bool doAttack = false;

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

                if(doAttack)
                {
                    pataSpeed = 0;
                    PerformAttack();
                }
            }

            attack_x += pataSpeed / fps;
        }
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
        debugText.setString(std::format("o{{n}}{}{{n}}af{{n}}{}{{n}}ias{{n}}{}{{n}}iat{{n}}{}{{n}}cai{{n}}{:2.2f}",order, main.getAnimationFrame(), inAttackSequence, inAttackTimer.getElapsedTime().asMilliseconds(), canAttackIn));
        debugText.setOrigin(debugText.getLocalBounds().width/2, debugText.getLocalBounds().height);
        debugText.setPosition(global_x+local_x+attack_x+gap_x-20, global_y+local_y-100);
        debugText.draw();
    }
}