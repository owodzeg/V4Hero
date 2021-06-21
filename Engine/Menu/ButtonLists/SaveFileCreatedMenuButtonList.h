#ifndef SAVEFILECREATED_BUTTONLIST_H
#define SAVEFILECREATED_BUTTONLIST_H

#include "../../Config.h"
#include "../../Graphics/Menu.h"
#include "../../Mission/MissionController.h"
#include "../ButtonList.h"
#include "../MenuButton.h"
#include "../Patapolis.h"
#include <SFML/Graphics.hpp>
#include <string>

class Menu;
class SaveFileCreatedMenu;
class SaveFileCreatedMenuButtonList : public ButtonList
{
public:
    void Initialise(sf::Font* font, Config& newConfig, MissionController* controller, SaveFileCreatedMenu* parentMenu);
    void SelectButton(int index);
    SaveFileCreatedMenu* svCrtMnu;
    SaveFileCreatedMenuButtonList();
    ~SaveFileCreatedMenuButtonList();
};


#endif // SAVEFILECREATED_BUTTONLIST_H
