#include "NameEntryButtonList.h"
#include "string"
#include "../MenuButton.h"
#include<iostream>
#include "../../Mission/MissionController.h"
#include "../MainMenu.h"
using namespace std;
NameEntryButtonList::NameEntryButtonList()
{

}
void NameEntryButtonList::Initialise(sf::Font *font,Config &newConfig,  std::map<int,bool> *thisKeymap,MissionController *controller,Menu *mainMenu)
{
    ButtonList::Initialise(font,newConfig,thisKeymap,controller,mainMenu);
    MenuButton* level1Button = new MenuButton(L"menu_button_confirm",font,42,450,this,0);
    buttons.push_back(*level1Button);
    buttons[0].SetSelected(false);

    this->SelectButtons = false;
}
void NameEntryButtonList::SelectButton(int index){
    currentIndex = index;
    switch (currentIndex){
            case 0:
                /// start first mission
                currentController->Initialise(*config,*keyMap,config->GetString("mission2Background"));
                currentController->StartMission(config->GetString("mission2Theme"));
                parentMenu->Hide();
                break;

        }
}
NameEntryButtonList::~NameEntryButtonList()
{

}
