#ifndef MAINMENU_H
#define MAINMENU_H
#include <SFML/Graphics.hpp>
#include "ButtonList.h"
#include "../Config.h"
#include "../Mission/MissionController.h"
class V4Core;
class MainMenu
{
    public:
        sf::RectangleShape mm_bigBox;
        sf::RectangleShape mm_smallerBox;
        sf::RectangleShape mm_titleBox;
        V4Core *v4core;

        sf::Font f_font;
        sf::Text t_title;
        sf::Text t_pressToContinue;

        Config *thisConfig;
        std::map<int,bool> *keyMap;

        bool inMission;

        ButtonList buttonList;
        void Initialise(Config &thisConfig, std::map<int,bool> *keymap,V4Core *parent);
        void Update(sf::RenderWindow &window, float fps);
        void EventFired(sf::Event event);
        MainMenu();
        ~MainMenu();

};


#endif // CAMERA_H
