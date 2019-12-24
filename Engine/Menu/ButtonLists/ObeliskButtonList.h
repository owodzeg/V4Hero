#ifndef OBELISKBUTTONLIST_H
#define OBELISKBUTTONLIST_H

#include <string>
#include <SFML/Graphics.hpp>
#include "../MenuButton.h"
#include "../../Config.h"
#include "../../Mission/MissionController.h"
#include "../../Graphics/Menu.h"
#include "../ButtonList.h"
class Menu;
class ObeliskMenu;
class ObeliskButtonList : public ButtonList
{
    public:

        ObeliskMenu* obeliskMenu;
        void Initialise(sf::Font *font,Config &newConfig, std::map<int,bool> *keymap,MissionController *controller,ObeliskMenu *parentMenu);
        void SelectButton(int index);

        ObeliskButtonList();
        ~ObeliskButtonList();

};


#endif // OBELISKBUTTONLIST_H
