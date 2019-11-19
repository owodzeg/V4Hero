#ifndef NEWGAMENAMEENTRYMENU_H
#define NEWGAMENAMEENTRYMENU_H
#include <SFML/Graphics.hpp>
#include "ButtonLists/NameEntryButtonList.h"
#include "../Config.h"
#include "../Graphics/Menu.h"
#include "../Mission/MissionController.h"
#include "OptionsMenu.h"
#include "Patapolis.h"
#include "MenuButton.h"
class V4Core;
class NewGameNameEntryMenu : public Menu
{
    public:
        sf::RectangleShape mm_bigBox;
        sf::RectangleShape mm_titleBox;

        sf::Font f_font;
        sf::Text t_title;
        sf::Text t_enteredtext;
        sf::Text t_pressToContinue;

        MenuButton* confirmButton;
        NameEntryButtonList buttonList;

        OptionsMenu optionsMenu;
        PatapolisMenu patapolisMenu;
        void Initialise(Config *thisConfig, std::map<int,bool> *keymap,V4Core *parent,Menu* parentMenu);

        void Update(sf::RenderWindow &window, float fps);
        void EventFired(sf::Event event);
        void OnExit();
        void UpdateButtons();
        NewGameNameEntryMenu();
        ~NewGameNameEntryMenu();

};


#endif // NEWGAMENAMEENTRYMENU_H
