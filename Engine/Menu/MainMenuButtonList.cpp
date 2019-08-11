#include "MainMenuButtonList.h"
#include "string"
#include "MenuButton.h"
#include<iostream>
#include "../Mission/MissionController.h"
#include "MainMenu.h"
using namespace std;
MainMenuButtonList::MainMenuButtonList()
{

}
void MainMenuButtonList::Initialise(sf::Font *font,Config &newConfig,  std::map<int,bool> *thisKeymap,MissionController *controller,Menu *mainMenu, Menu *CurOptionsMenu)
{
    ButtonList::Initialise(font,newConfig,thisKeymap,controller,mainMenu);
    MenuButton* level1Button = new MenuButton(L"menu_button_1",font,42,300,this,0);
    MenuButton* level2Button = new MenuButton(L"menu_button_2",font,42,350,this,1);
    MenuButton* optionsButton = new MenuButton(L"menu_button_3",font,42,400,this,2);
    buttons.push_back(*level1Button);
    buttons.push_back(*level2Button);
    buttons.push_back(*optionsButton);
    buttons[0].SetSelected(true);
    optionsMenu = CurOptionsMenu;
}
void MainMenuButtonList::SelectButton(int index){
    currentIndex = index;
    switch (currentIndex){
            case 0:
                // start level 1
                currentController->Initialise(*config,*keyMap,config->GetString("mission1Background"));
                currentController->StartMission(config->GetString("mission1Theme"));
                parentMenu->Hide();
                break;
            case 1:
                // start level 2
                currentController->Initialise(*config,*keyMap,config->GetString("mission2Background"));
                currentController->StartMission(config->GetString("mission2Theme"));
                parentMenu->Hide();
                break;
            case 2:
                // start options
                parentMenu->Hide();
                optionsMenu->Show();
                break;
        }
}
MainMenuButtonList::~MainMenuButtonList()
{

}
