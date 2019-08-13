#ifndef MAINMENUBUTTONLIST_H
#define MAINMENUBUTTONLIST_H

#include <string>
#include <SFML/Graphics.hpp>
#include "MenuButton.h"
#include "../Config.h"
#include "../Mission/MissionController.h"
#include "../Graphics/Menu.h"
#include "ButtonList.h"
#include "Patapolis.h"
class Menu;
class PatapolisMenu;
class MainMenuButtonList : public ButtonList
{
    public:

        Menu* optionsMenu;
        PatapolisMenu* patapolisMenu;
        void Initialise(sf::Font *font,Config &newConfig, std::map<int,bool> *keymap,MissionController *controller,Menu *parentMenu,Menu *CurOptionsMenu, PatapolisMenu *patapolis);
        void SelectButton(int index);

        MainMenuButtonList();
        ~MainMenuButtonList();

};


#endif // MAINMENUBUTTONLIST_H
