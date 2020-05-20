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
#include "TipsUtil.h"
class V4Core
{
    private:
    //MissionController currentController;
        std::vector<float> tot_fps;
        std::map<int,bool> keyMap;
        std::map<int,bool> keyMapHeld;
        sf::Thread* loadingThreadInstance;

        sf::Font f_font;
        sf::Text t_debug;
        sf::Text t_version;
        sf::Text t_pressAnyKey;
        float fps = 60;
        sf::Clock fpsclock;

    bool leftkey=false,rightkey=false,upkey=false,downkey=false;
    public:
        std::string hero_version = "v1.0.4";
        sf::RenderWindow window;

        NewGameMenu newGameMenu;
        MainMenu mainMenu;
        Config config;
        TipsUtil tipsUtil;
        SaveReader savereader;
        MissionController currentController;
        V4Core();
        std::vector<Menu*> menus;
        bool closeWindow=false;
        void Init();
        void ShowTip();
        void LoadingThread();
        void LoadingWaitForKeyPress();
        bool continueLoading;
        bool pressAnyKey;
};
#endif // V4CORE_H
