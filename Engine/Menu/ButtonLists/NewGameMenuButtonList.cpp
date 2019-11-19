#include "NewGameMenuButtonList.h"
#include "string"
#include "../MenuButton.h"
#include<iostream>
#include "../../Mission/MissionController.h"
#include "../MainMenu.h"
using namespace std;
NewGameMenuButtonList::NewGameMenuButtonList()
{

}
void NewGameMenuButtonList::Initialise(sf::Font *font,Config &newConfig,  std::map<int,bool> *thisKeymap,MissionController *controller,Menu *mainMenu, Menu *CurOptionsMenu, Menu *CurNameEntryMenu)
{
    ButtonList::Initialise(font,newConfig,thisKeymap,controller,mainMenu);
    MenuButton* level1Button = new MenuButton(L"menu_button_newgame",font,42,350,this,0);
    MenuButton* optionsButton = new MenuButton(L"menu_button_3",font,42,400,this,1);
    buttons.push_back(*level1Button);
    buttons.push_back(*optionsButton);
    buttons[0].SetSelected(true);
    optionsMenu = CurOptionsMenu;
    nameEntryMenu = CurNameEntryMenu;
}
void NewGameMenuButtonList::SelectButton(int index){
    currentIndex = index;
    switch (currentIndex){
            case 0:
                // start level 1
                parentMenu->Hide();
                nameEntryMenu->Show();
                break;
            case 1:
                // start options
                parentMenu->Hide();
                optionsMenu->Show();
                break;

        }
}
NewGameMenuButtonList::~NewGameMenuButtonList()
{

}
