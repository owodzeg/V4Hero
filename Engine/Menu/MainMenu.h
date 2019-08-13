#ifndef MAINMENU_H
#define MAINMENU_H
#include <SFML/Graphics.hpp>
#include "MainMenuButtonList.h"
#include "../Config.h"
#include "../Graphics/Menu.h"
#include "../Mission/MissionController.h"
#include "OptionsMenu.h"
#include "Patapolis.h"
class V4Core;
class MainMenu : public Menu
{
    public:
        sf::RectangleShape mm_bigBox;
        sf::RectangleShape mm_titleBox;

        sf::Font f_font;
        sf::Text t_title;
        sf::Text t_pressToContinue;

        MainMenuButtonList buttonList;

        OptionsMenu optionsMenu;
        PatapolisMenu patapolisMenu;
        void Initialise(Config *thisConfig, std::map<int,bool> *keymap,V4Core *parent);

        void Update(sf::RenderWindow &window, float fps);
        void EventFired(sf::Event event);
        void OnExit();
        void UpdateButtons();
        MainMenu();
        ~MainMenu();

};


#endif // CAMERA_H
