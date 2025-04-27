#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "DroppedItem.h"

#include <CoreManager.h>
#include <format>

DroppedItem::DroppedItem(std::vector<int> item_id)
{
    order_id = item_id;

    Item* itemPtr = CoreManager::getInstance().getSaveReader()->itemReg.getItemByID(item_id);
    itemID = itemPtr->spritesheet_id;
    category = itemPtr->spritesheet;

    main.load(std::format("resources/graphics/item/textures/{}/{:04}.png", category, itemID));

    hspeed = -500 + rand() % 300;
    vspeed = -1000;
}

void DroppedItem::Collect()
{
    pickedup = true;
    anim_state = 0;
    pickupClock.restart();
}

void DroppedItem::Draw()
{
    float fps = CoreManager::getInstance().getCore()->getFPS();
    auto w = CoreManager::getInstance().getWindow();

    if (!pickedup)
    {
        if (glowTimer.getElapsedTime().asMilliseconds() > 250)
        {
            GlowCircle tmp;
            tmp.circle.setFillColor(sf::Color(255, 255, 255, tmp.alpha));
            tmp.circle.setRadius(tmp.radius * CoreManager::getInstance().getCore()->resRatio);
            tmp.x = (global_x + local_x + off_x);
            tmp.y = (global_y + local_y + off_y);
            tmp.circle.setPosition(sf::Vector2f(tmp.x * CoreManager::getInstance().getCore()->resRatio, tmp.y * CoreManager::getInstance().getCore()->resRatio));
            glow.push_back(tmp);

            glowTimer.restart();
        }

        std::vector<int> rm;

        for (int i = 0; i < glow.size(); i++)
        {
            glow[i].alpha -= 64 / fps;
            glow[i].radius += (200 * CoreManager::getInstance().getCore()->resRatio) / fps;

            if (glow[i].alpha <= 0)
                glow[i].alpha = 0;

            glow[i].circle.setFillColor(sf::Color(255, 255, 255, glow[i].alpha));
            glow[i].circle.setRadius(glow[i].radius);
            glow[i].circle.setOrigin(sf::Vector2f(glow[i].circle.getLocalBounds().size.x / 2, glow[i].circle.getLocalBounds().size.y / 2));
            glow[i].circle.setPosition(sf::Vector2f((global_x + local_x + off_x) * CoreManager::getInstance().getCore()->resRatio, (global_y + local_y + off_y) * CoreManager::getInstance().getCore()->resRatio));

            w->draw(glow[i].circle);

            if (glow[i].alpha <= 0)
                rm.push_back(i);
        }

        for (int i = 0; i < rm.size(); i++)
        {
            glow.erase(glow.begin() + (rm[i] - i));
        }

        vspeed += 1600.f / fps;
        if(hspeed > 0)
            hspeed -= 120 / fps;
        if(hspeed < 0)
            hspeed += 120 / fps;

        if(global_y >= 2160-330-100)
        {
            vspeed = 0;
        }

        global_x += hspeed / fps;
        global_y += vspeed / fps;
    }
    else
    {
        if (anim_state == 0)
        {
            destXscale = 0.6;
            destYscale = 1.5;
            local_yPosDest = -250;

            if (curXscale > destXscale)
                curXscale -= abs(curXscale - destXscale) * 12 / fps;
            if (curYscale < destYscale)
                curYscale += abs(curYscale - destYscale) * 12 / fps;
            if (local_y > local_yPosDest)
                local_y -= abs(local_y - local_yPosDest) * 12 / fps;

            if (pickupClock.getElapsedTime().asMilliseconds() >= 210)
            {
                curXscale = 0.6;
                curYscale = 1.5;
                local_yPosDest = -750;

                anim_state = 1;
            }
        }

        if (anim_state == 1)
        {
            destXscale = 1;
            destYscale = 1;
            local_yPosDest = -150;

            if (curXscale < destXscale)
                curXscale += abs(curXscale - destXscale) * 12 / fps;
            if (curYscale > destYscale)
                curYscale -= abs(curYscale - destYscale) * 12 / fps;
            if (local_y < local_yPosDest)
                local_y += abs(local_y - local_yPosDest) * 12 / fps;

            if (pickupClock.getElapsedTime().asMilliseconds() >= 330)
            {
                curXscale = 1;
                curYscale = 1;
                local_yPosDest = -150;
                hspeed2 = 0.1;

                anim_state = 2;
            }
        }

        if (anim_state == 2)
        {
            if (pickupClock.getElapsedTime().asMilliseconds() >= 380)
            {
                vspeed2 = -((global_y+local_y+300)*12) / fps;
                hspeed2 += (hspeed2 * 5) / fps;

                if(vspeed2 <= -20)
                    vspeed2 = -20;

                if (hspeed2 >= 24)
                    alpha -= 500.0 / fps;

                if (alpha <= 0)
                    alpha = 0;

                local_y += vspeed2 / fps * 60;
                local_x += hspeed2 / fps * 60;

                curXscale -= 0.1 / fps;
                curYscale -= 0.1 / fps;

                if (curXscale <= -1)
                    curXscale = -1;
                if (curYscale <= -1)
                    curYscale = -1;

                main.setColor(sf::Color(255, 255, 255, alpha));

                if ((hspeed2 >= 1000) || (alpha <= 0))
                    ready_to_erase = true;
            }
        }
    }

    main.setOrigin(main.getLocalBounds().size.x/2, main.getLocalBounds().size.y/2);
    main.setScale(curXscale, curYscale);
    main.setPosition(global_x+local_x+off_x, global_y+local_y+off_y);
    main.draw();
}