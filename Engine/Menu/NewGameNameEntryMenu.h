#ifndef NEWGAMENAMEENTRYMENU_H
#define NEWGAMENAMEENTRYMENU_H
#include "../Config.h"
#include "../Graphics/Menu.h"
#include "../Mission/MissionController.h"
#include "ButtonLists/NameEntryButtonList.h"
#include "MenuButton.h"
#include "OptionsMenu.h"
#include "Patapolis.h"
#include "SaveFileCreated.h"
#include <SFML/Graphics.hpp>
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
    sf::Text t_promptText;

    bool showPromptText = false;

    MenuButton* confirmButton;
    NameEntryButtonList buttonList;
    SaveFileCreatedMenu savefilecreated;

    OptionsMenu optionsMenu;
    PatapolisMenu patapolisMenu;
    void Initialise(Config* thisConfig, V4Core* parent, Menu* parentMenu);
    string GetEnteredString();
    void Update(sf::RenderWindow& window, float fps);
    void EventFired(sf::Event event);
    void OnExit();
    void UpdateButtons();
    NewGameNameEntryMenu();
    ~NewGameNameEntryMenu();
};


#endif // NEWGAMENAMEENTRYMENU_H
