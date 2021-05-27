#ifndef SAVEFILECREATEDMENU_H
#define SAVEFILECREATEDMENU_H
#include <SFML/Graphics.hpp>
#include "ButtonLists/SaveFileCreatedMenuButtonList.h"
#include "../Config.h"
#include "../Graphics/Menu.h"
#include "../Mission/MissionController.h"
#include "OptionsMenu.h"
#include "Patapolis.h"
#include "MenuButton.h"
class V4Core;
class NewGameNameEntryMenu;
class SaveFileCreatedMenu : public Menu
{
    public:
        sf::RectangleShape mm_bigBox;
        sf::RectangleShape mm_titleBox;

        sf::Font f_font;
        sf::Text t_title;
        sf::Text t_welcome1;
        sf::Text t_welcome2;
        sf::Text t_welcome3;

        MenuButton* confirmButton;
        SaveFileCreatedMenuButtonList buttonList;
        string kamiName = "";

        NewGameNameEntryMenu* nameEntryMenu;
        void UpdateKamiName(string newName);
        void Initialise(Config *thisConfig,V4Core *parent,NewGameNameEntryMenu* parentMenu);

        void Update(sf::RenderWindow &window, float fps);
        void EventFired(sf::Event event);
        void OnExit();
        void UpdateButtons();
        SaveFileCreatedMenu();
        ~SaveFileCreatedMenu();

};


#endif // SAVEFILECREATEDMENU_H
