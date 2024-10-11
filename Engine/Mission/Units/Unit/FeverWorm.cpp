#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "FeverWorm.h"
#include <CoreManager.h>
#include <Func.h>

FeverWorm::FeverWorm()
{
    main.LoadConfig("resources/units/unit/feverworm.zip");
    main.setAnimation("slow");

    global_x = 300;
    global_y = 720;

    local_x = -1600;
}

void FeverWorm::PrefeverSpeedUp()
{
    main.setAnimation("fast");
}

void FeverWorm::PrefeverSlowDown()
{
    main.setAnimation("slow");
}

void FeverWorm::Transform()
{
    if(!fever)
    {
        main.setAnimation("transform");
        main.restartAnimation();
        fever = true;
        t_done = false;
        t_timer.restart();
        local_x = 0;
    }
}

void FeverWorm::Break()
{
    main.setAnimation("slow");
    fever = false;
    local_x = -1600;
}

void FeverWorm::Draw()
{
    auto w = CoreManager::getInstance().getWindow();

    auto v = w->getView();
    w->setView(w->getDefaultView());

    float fps = CoreManager::getInstance().getCore()->getFPS();

    auto r = CoreManager::getInstance().getRhythm();

    float destPrefeverOffset = (r->satisfaction / r->getAccRequirement(r->combo));

    if(r->combo >= 2 && !fever)
    {
        if(r->advanced_prefever)
            main.setAnimation("fast");
        else
            main.setAnimation("slow");

        local_x_dest = -500 + (destPrefeverOffset * 500);

        float delta = 1/fps * (4 * (main.animation.animationSpeed / 60.6));
        Func::smoothTransition(local_x, local_x_dest, delta);
    }

    if(fever && !t_done)
    {
        if(t_timer.getElapsedTime().asMilliseconds() >= 24*(1000 / main.animation.animationSpeed) && t_timer.getElapsedTime().asMilliseconds() < 35*(1000 / main.animation.animationSpeed))
        {
            if(local_x > -1600)
            {
                local_x -= 6500.f * (main.animation.animationSpeed/60.6) / fps;
            } else
            {
                local_x = -1600;
            }
        }


        if(t_timer.getElapsedTime().asMilliseconds() >= 50*(1000 / main.animation.animationSpeed))
        {
            main.setAnimation("fever");

            if(local_x < 120)
            {
                local_x += 1600.f * (main.animation.animationSpeed/60.6) / fps;
            }
        }

        if(t_timer.getElapsedTime().asMilliseconds() >= 80*(1000 / main.animation.animationSpeed))
        {
            if(local_x >= 120)
            t_done = true;
        }
    }

    main.setGlobalPosition(sf::Vector2f(global_x, global_y));
    main.setLocalPosition(sf::Vector2f(local_x, local_y));
    main.Draw();

    w->setView(v);
}