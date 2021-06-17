#include "SaveFileCreatedMenuButtonList.h"
#include "string"
#include "../MenuButton.h"
#include<iostream>
#include "../../Mission/MissionController.h"
#include "../MainMenu.h"
#include "../SaveFileCreated.h"
#include "../../V4Core.h"
using namespace std;
SaveFileCreatedMenuButtonList::SaveFileCreatedMenuButtonList()
{

}
void SaveFileCreatedMenuButtonList::Initialise(sf::Font *font,Config &newConfig,MissionController *controller,SaveFileCreatedMenu *parMenu)
{
    ButtonList::Initialise(font,newConfig,controller,parMenu);
    MenuButton* level1Button = new MenuButton(L"menu_button_confirm",font,42,550,this,0);
    buttons.push_back(*level1Button);
    buttons[0].SetSelected(false);

    this->SelectButtons = false;
    svCrtMnu = parMenu;
}
void SaveFileCreatedMenuButtonList::SelectButton(int index){
    currentIndex = index;
    switch (currentIndex){
            case 0:
                sf::Thread loadingThreadInstance(&V4Core::loadingThread,parentMenu->v4Core);
                parentMenu->v4Core->continue_loading=true;
                parentMenu->v4Core->window.setActive(false);
                loadingThreadInstance.launch();

                parentMenu->Hide();
                parentMenu->is_active = false;
                svCrtMnu->nameEntryMenu->Hide();

                currentController->Initialise(*config,config->GetString("mission1Background"),*parentMenu->v4Core);
                currentController->StartMission(config->GetString("mission1Theme"),1,1);

                break;

        }
}
SaveFileCreatedMenuButtonList::~SaveFileCreatedMenuButtonList()
{

}
