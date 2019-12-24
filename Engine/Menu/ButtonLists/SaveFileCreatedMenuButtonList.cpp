#include "SaveFileCreatedMenuButtonList.h"
#include "string"
#include "../MenuButton.h"
#include<iostream>
#include "../../Mission/MissionController.h"
#include "../MainMenu.h"
#include "../SaveFileCreated.h"
using namespace std;
SaveFileCreatedMenuButtonList::SaveFileCreatedMenuButtonList()
{

}
void SaveFileCreatedMenuButtonList::Initialise(sf::Font *font,Config &newConfig,  std::map<int,bool> *thisKeymap,MissionController *controller,SaveFileCreatedMenu *parMenu)
{
    ButtonList::Initialise(font,newConfig,thisKeymap,controller,parMenu);
    MenuButton* level1Button = new MenuButton(L"menu_button_confirm",font,42,550,this,0);
    buttons.push_back(*level1Button);
    buttons[0].SetSelected(false);

    this->SelectButtons = false;
}
void SaveFileCreatedMenuButtonList::SelectButton(int index){
    currentIndex = index;
    switch (currentIndex){
            case 0:
                /// start first mission
                currentController->Initialise(*config,*keyMap,config->GetString("mission2Background"));
                currentController->StartMission(config->GetString("mission2Theme"),true);
                parentMenu->Hide();
                break;

        }
}
SaveFileCreatedMenuButtonList::~SaveFileCreatedMenuButtonList()
{

}
