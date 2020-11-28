#ifndef WEATHER_H
#define WEATHER_H

#include <SFML/Graphics.hpp>
#include "../Config.h"
#include "../Graphics/PSprite.h"

using namespace std;

class Weather
{
    public:
    Config* thisConfig;

    int weatherType = 0; ///0 - clear, 1 - snow

    /// //////////// ///
    /// Snow weather ///
    /// //////////// ///

    PSprite ps_snowflakes[3];

    struct Snowflake
    {
        PSprite flake;
        float x=0,y=0,r=0;
        float xspeed=0,yspeed=0,rspeed=0;
    };

    vector<Snowflake> snowflakes;
    float snowflakesToRender = 0;

    float weatherIntensivity = 2;

    Weather();
    void loadWeather(int type);
    void setIntensivity(int intensivity);
    void draw(sf::RenderWindow& window, float fps);
};

#endif // WEATHER_H
