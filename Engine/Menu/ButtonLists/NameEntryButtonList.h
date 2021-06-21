#ifndef NAMEENTRY_BUTTONLIST_H
#define NAMEENTRY_BUTTONLIST_H

#include "../../Config.h"
#include "../../Graphics/Menu.h"
#include "../../Mission/MissionController.h"
#include "../ButtonList.h"
#include "../MenuButton.h"
#include "../Patapolis.h"
#include <SFML/Graphics.hpp>
#include <string>

class Menu;
class NewGameNameEntryMenu;
class NameEntryButtonList : public ButtonList
{
public:
    void Initialise(sf::Font* font, Config& newConfig, MissionController* controller, NewGameNameEntryMenu* parentMenu);
    void SelectButton(int index);
    NewGameNameEntryMenu* savMenu;
    NameEntryButtonList();
    ~NameEntryButtonList();
};


#endif // NAMEENTRY_BUTTONLIST_H
