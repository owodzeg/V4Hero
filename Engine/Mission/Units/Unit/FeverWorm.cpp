#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "FeverWorm.h"

FeverWorm::FeverWorm()
{
    main.LoadConfig("resources/units/unit/feverworm.zip");
    main.setAnimation("slow");

    global_x = 300;
    global_y = 850;
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
        fever = true;
        t_done = false;
        t_timer.restart();
    }
}

void FeverWorm::Break()
{
    main.setAnimation("slow");
    fever = false;
}

void FeverWorm::Draw()
{
    auto w = CoreManager::getInstance().getWindow();

    auto v = w->getView();
    w->setView(w->getDefaultView());

    float fps = CoreManager::getInstance().getCore()->getFPS();

    if(fever && !t_done)
    {
        if(t_timer.getElapsedTime().asMilliseconds() >= 21*(1 / main.animation.animationSpeed) && t_timer.getElapsedTime().asMilliseconds() < 120*(1 / main.animation.animationSpeed))
        {
            local_x -= 300.f / fps;
        }


        if(t_timer.getElapsedTime().asMilliseconds() >= 120*(1 / main.animation.animationSpeed))
        {
            if(local_x < 120)
            {
                local_x += 500.f / fps;
            }
        }

        if(t_timer.getElapsedTime().asMilliseconds() >= 150*(1 / main.animation.animationSpeed))
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