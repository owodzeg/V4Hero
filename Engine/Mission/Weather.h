#ifndef WEATHER_H
#define WEATHER_H

#include "../Config.h"
#include "../Graphics/PSprite.h"
#include <SFML/Graphics.hpp>

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

    struct Snowflake {
        PSprite flake;
        float x = 0, y = 0, r = 0;
        float xspeed = 0, yspeed = 0, rspeed = 0;
    };

    vector<Snowflake> snowflakes;
    float snowflakesToRender = 0;

    /// //////////// ///
    /// Rain weather ///
    /// //////////// ///

    struct Raindrop {
        sf::RectangleShape droplet;
        float x = 0, y = 0, r = 0;
        float xspeed = 0, yspeed = 0;
    };

    vector<Raindrop> raindrops;
    float raindropsToRender = 0;

    /// ////// ///
    /// Clouds ///
    /// ////// ///
    PSprite ps_cloud[2];

    struct Cloud {
        PSprite cloud;
        int cloudType = 0; ///0 - dark in back, 1 - lighter in front
        int movementType = 0;
        float destX = 0, destY = 0;
        float localX = 0, localY = 0;
        float globalX = 0, globalY = 0;
    };


    float weatherIntensivity = 2;

    Weather();
    void loadWeather(int type);
    void setIntensivity(int intensivity);
    void draw(sf::RenderWindow& window, float fps);
};

#endif // WEATHER_H
