#ifndef MAINMENU_H
#define MAINMENU_H
#include <SFML/Graphics.hpp>
#include "ButtonList.h"
#include "../Config.h"
#include "../Mission/MissionController.h"

class MainMenu
{
    public:
        sf::RectangleShape mm_bigBox;
        sf::RectangleShape mm_smallerBox;
        sf::RectangleShape mm_titleBox;


        sf::Font f_font;
        sf::Text t_title;
        sf::Text t_pressToContinue;

        Config *thisConfig;
        std::map<int,bool> *keyMap;
        MissionController currentController;

        bool inMission;

        ButtonList buttonList;
        void Initialise(Config &thisConfig, std::map<int,bool> *keymap);
        void Update(sf::RenderWindow &window, float fps);
        void KeyPressedEvent(sf::Event event);
        MainMenu();
        ~MainMenu();

};


#endif // CAMERA_H
