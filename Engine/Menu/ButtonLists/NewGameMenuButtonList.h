#ifndef NEWGAMEMENUBUTTONLIST_H
#define NEWGAMEMENUBUTTONLIST_H

#include "../../Config.h"
#include "../../Graphics/Menu.h"
#include "../../Mission/MissionController.h"
#include "../ButtonList.h"
#include "../MenuButton.h"
#include "../Patapolis.h"
#include <SFML/Graphics.hpp>
#include <string>

class Menu;
class PatapolisMenu;
class NewGameMenuButtonList : public ButtonList
{
public:
    Menu* optionsMenu;
    Menu* nameEntryMenu;
    void Initialise(sf::Font* font, Config& newConfig, MissionController* controller, Menu* parentMenu, Menu* CurOptionsMenu, Menu* CurNameEntryMenu);
    void SelectButton(int index);

    NewGameMenuButtonList();
    ~NewGameMenuButtonList();
};


#endif // NEWGAMEMENUBUTTONLIST_H
