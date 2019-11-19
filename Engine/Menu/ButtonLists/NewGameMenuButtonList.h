#ifndef NEWGAMEMENUBUTTONLIST_H
#define NEWGAMEMENUBUTTONLIST_H

#include <string>
#include <SFML/Graphics.hpp>
#include "../MenuButton.h"
#include "../../Config.h"
#include "../../Mission/MissionController.h"
#include "../../Graphics/Menu.h"
#include "../ButtonList.h"
#include "../Patapolis.h"
class Menu;
class PatapolisMenu;
class NewGameMenuButtonList : public ButtonList
{
    public:

        Menu* optionsMenu;
        Menu* nameEntryMenu;
        void Initialise(sf::Font *font,Config &newConfig, std::map<int,bool> *keymap,MissionController *controller,Menu *parentMenu,Menu *CurOptionsMenu, Menu *CurNameEntryMenu);
        void SelectButton(int index);

        NewGameMenuButtonList();
        ~NewGameMenuButtonList();

};


#endif // NEWGAMEMENUBUTTONLIST_H
