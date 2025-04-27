#include "Weather.h"

#include <cmath>
#include <iostream>
#include <string>

#include "../CoreManager.h"

using namespace std;
using std::floor;

Weather::Weather()
{
}

void Weather::loadWeather(int type)
{
    cout << "[Weather] Loading weather " << type << endl;

    switch (type)
    {
        case 1: {
            cout << "[Weather] Loading snowflakes" << endl;
            for (int i = 0; i < 3; i++)
            {
                ps_snowflakes[i].loadFromFile("resources/graphics/weather/snowflake_" + std::to_string(i + 1) + ".png", thisConfig->GetInt("textureQuality"));
                ps_snowflakes[i].setOrigin(ps_snowflakes[i].getLocalBounds().size.x / 2, ps_snowflakes[i].getLocalBounds().size.y / 2);
            }

            break;
        }
    }

    weatherType = type;
}

void Weather::draw(sf::RenderWindow& window, float fps)
{
    switch (weatherType)
    {
        case 1: ///Snow
        {
            ///snow intensivity = 60 per second
            ///fps = 240
            ///snow per frame = 0.25
            ///cant really make it a straightforward for loop
            ///need to use a second variable

            snowflakesToRender += weatherIntensivity / fps;

            for (int i = 0; i < floor(snowflakesToRender); i++)
            {
                Snowflake tmp;

                int ra = rand() % 100;
                int flake = 2;
                if (ra < 60)
                    flake = 1;
                if (ra < 15)
                    flake = 0;

                tmp.flake = ps_snowflakes[flake];
                tmp.x = rand() % 1400 - 180 - 6 * weatherIntensivity;
                tmp.y = -100;
                tmp.r = (rand() % 1000) / float(1000);
                tmp.xspeed = 6 * weatherIntensivity;
                tmp.yspeed = 30 * weatherIntensivity;
                tmp.rspeed = (rand() % 50) / float(50);

                snowflakes.push_back(tmp);

                snowflakesToRender -= 1;
            }

            break;
        }

        case 2: ///Rain
        {
            raindropsToRender += weatherIntensivity / fps;

            for (int i = 0; i < floor(raindropsToRender); i++)
            {
                Raindrop tmp;

                int ra = rand() % 100;
                int d = 1;
                if (ra < 30)
                    d = 0;

                sf::RectangleShape r_drop;
                r_drop.setSize(sf::Vector2f(3.0 * CoreManager::getInstance().getCore()->resRatio, 64.0 * CoreManager::getInstance().getCore()->resRatio));

                if (d == 1)
                    r_drop.setFillColor(sf::Color(240, 240, 240, 255));
                if (d == 0)
                    r_drop.setFillColor(sf::Color(210, 210, 210, 255));

                tmp.droplet = r_drop;
                tmp.x = rand() % 1400 - 180 - 6 * weatherIntensivity;
                tmp.y = -100;
                tmp.r = (rand() % 1000) / float(1000);
                tmp.xspeed = 0 * weatherIntensivity;
                tmp.yspeed = 60 * weatherIntensivity;

                raindrops.push_back(tmp);

                raindropsToRender -= 1;
            }

            break;
        }
    }

    auto view = window.getView();
    window.setView(window.getDefaultView());

    vector<int> s_rm;
    vector<int> r_rm;

    for (int i = 0; i < snowflakes.size(); i++)
    {
        snowflakes[i].x += snowflakes[i].xspeed / fps;
        snowflakes[i].y += snowflakes[i].yspeed / fps;
        snowflakes[i].r += snowflakes[i].rspeed / fps;

        snowflakes[i].flake.setPosition(snowflakes[i].x, snowflakes[i].y);
        snowflakes[i].flake.setRotation(snowflakes[i].r);

        if ((snowflakes[i].x > 1320) || (snowflakes[i].y > 800))
            s_rm.push_back(i);

        snowflakes[i].flake.draw();
    }

    for (int i = 0; i < raindrops.size(); i++)
    {
        raindrops[i].x += raindrops[i].xspeed / fps;
        raindrops[i].y += raindrops[i].yspeed / fps;

        raindrops[i].droplet.setPosition(sf::Vector2f(raindrops[i].x, raindrops[i].y));

        if ((raindrops[i].x > 1320) || (raindrops[i].y > 800))
            r_rm.push_back(i);

        window.draw(raindrops[i].droplet);
    }

    for (int i = 0; i < s_rm.size(); i++)
    {
        snowflakes.erase(snowflakes.begin() + s_rm[i] - i);
    }

    for (int i = 0; i < r_rm.size(); i++)
    {
        raindrops.erase(raindrops.begin() + r_rm[i] - i);
    }

    window.setView(view);
}
