#ifndef SAVEFILECREATED_BUTTONLIST_H
#define SAVEFILECREATED_BUTTONLIST_H

#include <string>
#include <SFML/Graphics.hpp>
#include "../MenuButton.h"
#include "../../Config.h"
#include "../../Mission/MissionController.h"
#include "../../Graphics/Menu.h"
#include "../ButtonList.h"
#include "../Patapolis.h"

class Menu;
class SaveFileCreatedMenu;
class SaveFileCreatedMenuButtonList : public ButtonList
{
    public:
        void Initialise(sf::Font *font,Config &newConfig, std::map<int,bool> *keymap,MissionController *controller,SaveFileCreatedMenu *parentMenu);
        void SelectButton(int index);

        SaveFileCreatedMenuButtonList();
        ~SaveFileCreatedMenuButtonList();

};


#endif // SAVEFILECREATED_BUTTONLIST_H
