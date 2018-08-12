#ifndef V4CORE_H
#define V4CORE_H

#include <SFML/Graphics.hpp>

#include "Rhythm/Rhythm.h"
#include "Config.h"

class V4Core
{
    private:
    Rhythm rhythm;
    Config config;

    std::vector<float> tot_fps;
    std::map<int,bool> keyMap;

    float fps = 60;
    sf::Clock fpsclock;

    bool leftkey=false,rightkey=false,upkey=false,downkey=false;

    public:
    V4Core();
    void Init();
};

#endif // V4CORE_H
