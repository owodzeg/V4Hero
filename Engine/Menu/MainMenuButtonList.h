#ifndef MAINMENUBUTTONLIST_H
#define MAINMENUBUTTONLIST_H

#include <string>
#include <SFML/Graphics.hpp>
#include "MenuButton.h"
#include "../Config.h"
#include "../Mission/MissionController.h"
#include "../Graphics/Menu.h"
#include "ButtonList.h"
class Menu;
class MainMenuButtonList : public ButtonList
{
    public:

        Menu* optionsMenu;
        void Initialise(sf::Font *font,Config &newConfig, std::map<int,bool> *keymap,MissionController *controller,Menu *parentMenu,Menu *CurOptionsMenu);
        void SelectButton(int index);

        MainMenuButtonList();
        ~MainMenuButtonList();

};


#endif // MAINMENUBUTTONLIST_H
