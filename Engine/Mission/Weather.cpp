#include "Weather.h"

#include <string>

Weather::Weather()
{

}

void Weather::loadWeather(int type)
{
    switch(type)
    {
        case 1:
        {
            ps_snowflakes[0].loadFromFile("resources/graphics/weather/snowflake_1.png", thisConfig->GetInt("textureQuality"), 1);
            ps_snowflakes[1].loadFromFile("resources/graphics/weather/snowflake_2.png", thisConfig->GetInt("textureQuality"), 1);
            ps_snowflakes[2].loadFromFile("resources/graphics/weather/snowflake_3.png", thisConfig->GetInt("textureQuality"), 1);

            for(int i=0; i<3; i++)
            {
                ps_snowflakes[i].loadFromFile("resources/graphics/weather/snowflake_"+std::to_string(i+1)+".png", thisConfig->GetInt("textureQuality"), 1);
                ps_snowflakes[i].setOrigin(ps_snowflakes[i].getLocalBounds().width/2, ps_snowflakes[i].getLocalBounds().height/2);
            }

            break;
        }
    }

    weatherType = type;
}

void Weather::draw(sf::RenderWindow& window, float fps)
{
    switch(weatherType)
    {
        case 1: ///Snow
        {
            auto view = window.getView();
            window.setView(window.getDefaultView());

            ///snow intensivity = 60 per second
            ///fps = 240
            ///snow per frame = 0.25
            ///cant really make it a straightforward for loop
            ///need to use a second variable

            snowflakesToRender += weatherIntensivity / fps;

            for(int i=0; i<floor(snowflakesToRender); i++)
            {
                Snowflake tmp;

                int ra = rand() % 100;
                int flake = 2;
                if(ra < 60)
                flake = 1;
                if(ra < 15)
                flake = 0;

                tmp.flake = ps_snowflakes[flake];
                tmp.x = rand() % 1400 - 200;
                tmp.y = -100;
                tmp.r = (rand() % 1000) / float(1000);
                tmp.xspeed = 12;
                tmp.yspeed = 60;
                tmp.rspeed = (rand() % 50) / float(50);

                snowflakes.push_back(tmp);

                snowflakesToRender -= 1;
            }

            vector<int> s_rm;

            for(int i=0; i<snowflakes.size(); i++)
            {
                snowflakes[i].x += snowflakes[i].xspeed / fps;
                snowflakes[i].y += snowflakes[i].yspeed / fps;
                snowflakes[i].r += snowflakes[i].rspeed / fps;

                snowflakes[i].flake.setPosition(snowflakes[i].x, snowflakes[i].y);
                snowflakes[i].flake.setRotation(snowflakes[i].r);

                if((snowflakes[i].x > 1320) || (snowflakes[i].y > 800))
                s_rm.push_back(i);

                snowflakes[i].flake.draw(window);
            }

            for(int i=0; i<s_rm.size(); i++)
            {
                snowflakes.erase(snowflakes.begin() + s_rm[i] - i);
            }

            window.setView(view);

            break;
        }
    }
}
