#ifndef NAMEENTRY_BUTTONLIST_H
#define NAMEENTRY_BUTTONLIST_H

#include <string>
#include <SFML/Graphics.hpp>
#include "../MenuButton.h"
#include "../../Config.h"
#include "../../Mission/MissionController.h"
#include "../../Graphics/Menu.h"
#include "../ButtonList.h"
#include "../Patapolis.h"
class Menu;
class NewGameNameEntryMenu;
class NameEntryButtonList : public ButtonList
{
    public:
        void Initialise(sf::Font *font,Config &newConfig, std::map<int,bool> *keymap,MissionController *controller,NewGameNameEntryMenu *parentMenu);
        void SelectButton(int index);
        NewGameNameEntryMenu* savMenu;
        NameEntryButtonList();
        ~NameEntryButtonList();

};


#endif // NAMEENTRY_BUTTONLIST_H
