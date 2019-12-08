#include "NameEntryButtonList.h"
#include "string"
#include "../MenuButton.h"
#include<iostream>
#include "../../Mission/MissionController.h"
#include "../MainMenu.h"
#include "../NewGameNameEntryMenu.h"
#include "../../Func.h"
#include <SFML/Graphics.hpp>

#include "../ButtonList.h"
#include "../../V4Core.h"
#include <SFML/System/Time.hpp>
using namespace std;
NameEntryButtonList::NameEntryButtonList()
{

}
void NameEntryButtonList::Initialise(sf::Font *font,Config &newConfig,  std::map<int,bool> *thisKeymap,MissionController *controller,NewGameNameEntryMenu *parMenu)
{
    ButtonList::Initialise(font,newConfig,thisKeymap,controller,parMenu);
    MenuButton* level1Button = new MenuButton(L"menu_button_confirm",font,42,450,this,0);
    buttons.push_back(*level1Button);
    buttons[0].SetSelected(false);
    savMenu = parMenu;

    this->SelectButtons = false;
}
std::string trim(const std::string& str, const std::string& whitespace = " \t")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}
void NameEntryButtonList::SelectButton(int index){
    currentIndex = index;
    switch (currentIndex){
            case 0:
                /// start first mission
                if (trim(savMenu->GetEnteredString()," \t").length()>1 && trim(savMenu->GetEnteredString()," \t").length()<50) {
                    parentMenu->Hide();
                    savMenu->savefilecreated.Show();
                    savMenu->savefilecreated.isActive = true;
                    savMenu->savefilecreated.UpdateKamiName(savMenu->GetEnteredString());
                    savMenu->v4core->savereader.kaminame = savMenu->GetEnteredString();
                    savMenu->v4core->savereader.heroUnlocked = 0;
                    savMenu->v4core->savereader.yariponsUnlocked = 0;
                    savMenu->v4core->savereader.timeslaunched = 1;
                    InventoryItem spear1;
                    spear1.item = savMenu->v4core->savereader.itemreg.GetItemByID(1);
                    InventoryItem spear2;
                    spear2.item = savMenu->v4core->savereader.itemreg.GetItemByID(1);
                    InventoryItem spear3;
                    spear3.item = savMenu->v4core->savereader.itemreg.GetItemByID(1);
                    InventoryItem iron_spear1;
                    iron_spear1.item = savMenu->v4core->savereader.itemreg.GetItemByID(2);
                    InventoryItem mask;
                    mask.item = savMenu->v4core->savereader.itemreg.GetItemByID(4);
                    savMenu->v4core->savereader.invdata.items.push_back(spear1);
                    savMenu->v4core->savereader.invdata.items.push_back(spear2);
                    savMenu->v4core->savereader.invdata.items.push_back(spear3);
                    savMenu->v4core->savereader.invdata.items.push_back(iron_spear1);
                    savMenu->v4core->savereader.invdata.items.push_back(mask);
                    savMenu->v4core->savereader.Save();
                } else {
                    savMenu->showPromptText = true;
                }
                break;

        }
}

NameEntryButtonList::~NameEntryButtonList()
{

}
