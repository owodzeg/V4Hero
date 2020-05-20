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

                    savMenu->savefilecreated.Show();
                    savMenu->savefilecreated.isActive = true;
                    savMenu->savefilecreated.UpdateKamiName(savMenu->GetEnteredString());
                    savMenu->v4core->savereader.kaminame = savMenu->GetEnteredString();
                    savMenu->v4core->savereader.heroUnlocked = 0;
                    savMenu->v4core->savereader.yariponsUnlocked = 0;
                    savMenu->v4core->savereader.timeslaunched = 1;
                    InventoryItem spear1;
                    spear1.item = savMenu->v4core->savereader.itemreg.GetItemByID(1);
                    spear1.inventoryId=0;
                    InventoryItem spear2;
                    spear2.item = savMenu->v4core->savereader.itemreg.GetItemByID(1);
                    spear2.inventoryId=1;
                    InventoryItem spear3;
                    spear3.item = savMenu->v4core->savereader.itemreg.GetItemByID(1);
                    spear3.inventoryId=2;
                    InventoryItem iron_spear1;
                    iron_spear1.item = savMenu->v4core->savereader.itemreg.GetItemByID(2);
                    iron_spear1.inventoryId=3;
                    InventoryItem mask;
                    mask.item = savMenu->v4core->savereader.itemreg.GetItemByID(4);
                    mask.inventoryId=4;
                    InventoryItem woodhelmet_1;
                    woodhelmet_1.item = savMenu->v4core->savereader.itemreg.GetItemByID(16);
                    woodhelmet_1.inventoryId=5;
                    InventoryItem woodhelmet_2;
                    woodhelmet_2.item = savMenu->v4core->savereader.itemreg.GetItemByID(16);
                    woodhelmet_2.inventoryId=6;
                    InventoryItem woodhelmet_3;
                    woodhelmet_3.item = savMenu->v4core->savereader.itemreg.GetItemByID(16);
                    woodhelmet_3.inventoryId=7;
                    InventoryItem ironhelmet_1;
                    ironhelmet_1.item = savMenu->v4core->savereader.itemreg.GetItemByID(17);
                    ironhelmet_1.inventoryId=8;
                    InventoryItem keepsake;
                    keepsake.item = savMenu->v4core->savereader.itemreg.GetItemByID(15);
                    keepsake.inventoryId=9;

                    InventoryItem pondrum;
                    pondrum.item = savMenu->v4core->savereader.itemreg.GetItemByID(11);
                    pondrum.inventoryId=10;
                    InventoryItem patadrum;
                    patadrum.item = savMenu->v4core->savereader.itemreg.GetItemByID(12);
                    patadrum.inventoryId=11;
                    InventoryItem chakadrum;
                    chakadrum.item = savMenu->v4core->savereader.itemreg.GetItemByID(13);
                    chakadrum.inventoryId=12;
                    InventoryItem dondrum;
                    dondrum.item = savMenu->v4core->savereader.itemreg.GetItemByID(14);
                    dondrum.inventoryId=13;


                    savMenu->v4core->savereader.invdata.items.push_back(spear1);
                    savMenu->v4core->savereader.invdata.items.push_back(spear2);
                    savMenu->v4core->savereader.invdata.items.push_back(spear3);
                    savMenu->v4core->savereader.invdata.items.push_back(iron_spear1);
                    savMenu->v4core->savereader.invdata.items.push_back(mask);
                    savMenu->v4core->savereader.invdata.items.push_back(woodhelmet_1);
                    savMenu->v4core->savereader.invdata.items.push_back(woodhelmet_2);
                    savMenu->v4core->savereader.invdata.items.push_back(woodhelmet_3);
                    savMenu->v4core->savereader.invdata.items.push_back(ironhelmet_1);
                    savMenu->v4core->savereader.invdata.items.push_back(keepsake);
                    savMenu->v4core->savereader.invdata.items.push_back(pondrum);
                    savMenu->v4core->savereader.invdata.items.push_back(patadrum);
                    savMenu->v4core->savereader.invdata.items.push_back(chakadrum);
                    savMenu->v4core->savereader.invdata.items.push_back(dondrum);


                    Pon starterPon = Pon(&savMenu->v4core->savereader);
                    starterPon.pon_id = 0;
                    starterPon.pon_class = 0;
                    starterPon.pon_squad_position = 0;
                    starterPon.pon_exp = 0;
                    starterPon.pon_level = 1;
                    //starterPon.weapon_invItem_id = 3;
                    //starterPon.armour_invItem_id = 8;
                    //starterPon.weapon2_invItem_id = -1;
                    //tarterPon.mask_invItem_id = 4;
                    starterPon.GiveItem(3);
                    starterPon.GiveItem(8);
                    starterPon.GiveItem(4);


                    Pon starterPon2 = Pon(&savMenu->v4core->savereader);
                    starterPon2.pon_id = 1;
                    starterPon2.pon_class = 1;
                    starterPon2.pon_squad_position = 0;
                    starterPon2.pon_exp = 0;
                    starterPon2.pon_level = 1;
                    //starterPon2.weapon_invItem_id = 0;
                    //starterPon2.armour_invItem_id = 5;
                    //starterPon2.weapon2_invItem_id = -1;
                    //starterPon2.mask_invItem_id = -1;
                    starterPon2.GiveItem(0);
                    starterPon2.GiveItem(5);


                    Pon starterPon3 = Pon(&savMenu->v4core->savereader);
                    starterPon3.pon_id = 2;
                    starterPon3.pon_class = 1;
                    starterPon3.pon_squad_position = 1;
                    starterPon3.pon_exp = 0;
                    starterPon3.pon_level = 1;
                    //starterPon3.weapon_invItem_id = 1;
                    //starterPon3.armour_invItem_id = 6;
                    //starterPon3.weapon2_invItem_id = -1;
                    //starterPon3.mask_invItem_id = -1;
                    starterPon3.GiveItem(1);
                    starterPon3.GiveItem(6);


                    Pon starterPon4 = Pon(&savMenu->v4core->savereader);
                    starterPon4.pon_id = 3;
                    starterPon4.pon_class = 1;
                    starterPon4.pon_squad_position = 2;
                    starterPon4.pon_exp = 0;
                    starterPon4.pon_level = 1;
                    //starterPon4.weapon_invItem_id = 2;
                    //starterPon4.armour_invItem_id = 7;
                    //starterPon4.weapon2_invItem_id = -1;
                    //starterPon4.mask_invItem_id = -1;
                    starterPon4.GiveItem(2);
                    starterPon4.GiveItem(7);

                    savMenu->v4core->savereader.ponreg.pons.push_back(starterPon);
                    savMenu->v4core->savereader.ponreg.pons.push_back(starterPon2);
                    savMenu->v4core->savereader.ponreg.pons.push_back(starterPon3);
                    savMenu->v4core->savereader.ponreg.pons.push_back(starterPon4);


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
