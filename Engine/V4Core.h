#ifndef V4CORE_H
#define V4CORE_H

#include <SFML/Graphics.hpp>

#include "Rhythm/Rhythm.h"
#include "Mission/Background.h"
#include "Mission/Camera.h"
#include "Config.h"
#include "Mission/Patapon.h"

class V4Core
{
    private:
    Rhythm rhythm;
    Config config;
    Background test_bg;
    Camera camera;
    Patapon patapon;

    std::vector<float> tot_fps;
    std::map<int,bool> keyMap;

    sf::Font f_font;
    sf::Text t_debug;

    float fps = 60;
    sf::Clock fpsclock;

    bool leftkey=false,rightkey=false,upkey=false,downkey=false;

    public:
    V4Core();
    void Init();
};

#endif // V4CORE_H
