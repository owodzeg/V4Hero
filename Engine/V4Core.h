#ifndef V4CORE_H
#define V4CORE_H

#include <SFML/Graphics.hpp>

#include "Rhythm/Rhythm.h"
#include "Mission/Background.h"
#include "Mission/Camera.h"
#include "Config.h"
#include "StringRepository.h"
#include "Mission/Units/Patapon.h"
#include "Mission/MissionController.h"
#include "Menu/MainMenu.h"
#include "Graphics/Menu.h"
class V4Core
{
    private:

    Config config;
    //MissionController currentController;
    MainMenu mainMenu;



    std::vector<float> tot_fps;
    std::map<int,bool> keyMap;

    sf::Font f_font;
    sf::Text t_debug;

    float fps = 60;
    sf::Clock fpsclock;

    bool leftkey=false,rightkey=false,upkey=false,downkey=false;

    public:
        MissionController currentController;
    V4Core();
    std::vector<Menu*> menus;
    void Init();
};

#endif // V4CORE_H
