#include "ObeliskButtonList.h"
#include "string"
#include "../MenuButton.h"
#include<iostream>
#include "../../Mission/MissionController.h"
#include "../../V4Core.h"
using namespace std;
class ObeliskMenu;
ObeliskButtonList::ObeliskButtonList()
{

}
void ObeliskButtonList::Initialise(sf::Font *font,Config &newConfig,  std::map<int,bool> *thisKeymap,MissionController *controller,ObeliskMenu *mainMenu)
{
    ButtonList::Initialise(font,newConfig,thisKeymap,controller,mainMenu);
    //MenuButton* level1Button = new MenuButton(L"menu_button_1",font,42,300,this,0);
    //MenuButton* level2Button = new MenuButton(L"menu_button_2",font,42,350,this,1);
    MenuButton* patapolisButton = new MenuButton(L"obelisk_mission_1_name",font,72,200,this,0,100);
    MenuButton* optionsButton = new MenuButton(L"obelisk_mission_2_name",font,72,300,this,1,100);
    MenuButton* quitButton = new MenuButton(L"obelisk_quit",font,72,config->GetInt("resY")-200,this,2,100);
    buttons.push_back(*patapolisButton);
    //buttons.push_back(*level1Button);
    //buttons.push_back(*level2Button);
    buttons.push_back(*optionsButton);
    buttons.push_back(*quitButton);
    obeliskMenu = mainMenu;
    buttons[0].SetSelected(true);
    this->SelectButtons = true;
}
void ObeliskButtonList::SelectButton(int index){
    currentIndex = index;
    switch (currentIndex){
            /*case 1:
                // start level 1

                parentMenu->Hide();
                break;
            case 2:
                // start level 2
                currentController->Initialise(*config,*keyMap,config->GetString("mission2Background"));
                currentController->StartMission(config->GetString("mission2Theme"));
                parentMenu->Hide();
                break;*/

            case 0:
                // start options
                currentController->Initialise(*config,*keyMap,config->GetString("mission1Background"));
                currentController->StartMission(config->GetString("mission1Theme"));
                obeliskMenu->parentMenu->Hide();
                obeliskMenu->parentMenu->isActive=false;
                parentMenu->Hide();
                parentMenu->isActive = false;
                break;
            case 1:
                // start options
                currentController->Initialise(*config,*keyMap,config->GetString("mission2Background"));
                currentController->StartMission(config->GetString("mission2Theme"));
                obeliskMenu->parentMenu->Hide();
                obeliskMenu->parentMenu->isActive=false;
                parentMenu->Hide();
                parentMenu->isActive = false;
                break;
            case 2:
                // start options
                parentMenu->Hide();
                parentMenu->isActive = false;
                break;
        }
}
ObeliskButtonList::~ObeliskButtonList()
{

}
