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
#include "SaveReader.h"
#include "Menu/NewGameMenu.h"
class V4Core
{
    private:

    //MissionController currentController;



    std::vector<float> tot_fps;
    std::map<int,bool> keyMap;

    sf::Font f_font;
    sf::Text t_debug;

    float fps = 60;
    sf::Clock fpsclock;

    bool leftkey=false,rightkey=false,upkey=false,downkey=false;
    public:
    NewGameMenu newGameMenu;
    MainMenu mainMenu;
    Config config;
    SaveReader savereader;
        MissionController currentController;
    V4Core();
    std::vector<Menu*> menus;
    bool closeWindow=false;
    void Init();
};

#endif // V4CORE_H
