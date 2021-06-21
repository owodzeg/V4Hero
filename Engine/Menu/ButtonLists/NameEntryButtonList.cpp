#include "NameEntryButtonList.h"
#include "../../Func.h"
#include "../../Mission/MissionController.h"
#include "../MainMenu.h"
#include "../MenuButton.h"
#include "../NewGameNameEntryMenu.h"
#include "string"
#include <SFML/Graphics.hpp>
#include <iostream>

#include "../../V4Core.h"
#include "../ButtonList.h"
#include <SFML/System/Time.hpp>
using namespace std;
NameEntryButtonList::NameEntryButtonList()
{
}
void NameEntryButtonList::Initialise(sf::Font* font, Config& newConfig, MissionController* controller, NewGameNameEntryMenu* parMenu)
{
    ButtonList::Initialise(font, newConfig, controller, parMenu);
    MenuButton* level1Button = new MenuButton(L"menu_button_confirm", font, 42, 450, this, 0);
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
void NameEntryButtonList::SelectButton(int index)
{
    currentIndex = index;
    switch (currentIndex)
    {
        case 0:
            /// start first mission
            if (trim(savMenu->GetEnteredString(), " \t").length() > 1 && trim(savMenu->GetEnteredString(), " \t").length() < 50)
            {

                savMenu->savefilecreated.Show();
                savMenu->savefilecreated.is_active = true;
                savMenu->savefilecreated.UpdateKamiName(savMenu->GetEnteredString());
                savMenu->v4Core->saveReader.kami_name = savMenu->GetEnteredString();
                savMenu->v4Core->saveReader.hero_unlocked = 0;
                savMenu->v4Core->saveReader.times_launched = 1;

                for (int i = 0; i < 3; i++)
                {
                    savMenu->v4Core->saveReader.invData.addItem(savMenu->v4Core->saveReader.itemReg.getItemByName("item_wooden_spear")->order_id);
                    savMenu->v4Core->saveReader.invData.addItem(savMenu->v4Core->saveReader.itemReg.getItemByName("item_wooden_helmet")->order_id);
                }
                savMenu->v4Core->saveReader.invData.addItem(savMenu->v4Core->saveReader.itemReg.getItemByName("item_iron_helmet")->order_id);
                savMenu->v4Core->saveReader.invData.addItem(savMenu->v4Core->saveReader.itemReg.getItemByName("item_iron_spear")->order_id);


                savMenu->v4Core->saveReader.invData.addItem(savMenu->v4Core->saveReader.itemReg.getItemByName("item_shubaba_mask")->order_id);

                savMenu->v4Core->saveReader.invData.addItem(savMenu->v4Core->saveReader.itemReg.getItemByName("item_keepsake")->order_id);

                savMenu->v4Core->saveReader.invData.addItem(savMenu->v4Core->saveReader.itemReg.getItemByName("item_pon_drum")->order_id);
                savMenu->v4Core->saveReader.invData.addItem(savMenu->v4Core->saveReader.itemReg.getItemByName("item_pata_drum")->order_id);
                savMenu->v4Core->saveReader.invData.addItem(savMenu->v4Core->saveReader.itemReg.getItemByName("item_chaka_drum")->order_id);
                savMenu->v4Core->saveReader.invData.addItem(savMenu->v4Core->saveReader.itemReg.getItemByName("item_don_drum")->order_id);


                Pon starterPon = Pon(&savMenu->v4Core->saveReader);
                starterPon.pon_id = 0;
                starterPon.pon_class = 0;
                starterPon.pon_squad_position = 0;
                starterPon.pon_exp = 0;
                starterPon.pon_level = 1;
                //starterPon.weapon_invItem_id = 3;
                //starterPon.armour_invItem_id = 8;
                //starterPon.weapon2_invItem_id = -1;
                //tarterPon.mask_invItem_id = 4;
                starterPon.giveItem(3);
                starterPon.giveItem(8);
                starterPon.giveItem(4);


                Pon starterPon2 = Pon(&savMenu->v4Core->saveReader);
                starterPon2.pon_id = 1;
                starterPon2.pon_class = 1;
                starterPon2.pon_squad_position = 0;
                starterPon2.pon_exp = 0;
                starterPon2.pon_level = 1;
                //starterPon2.weapon_invItem_id = 0;
                //starterPon2.armour_invItem_id = 5;
                //starterPon2.weapon2_invItem_id = -1;
                //starterPon2.mask_invItem_id = -1;
                starterPon2.giveItem(0);
                starterPon2.giveItem(5);


                Pon starterPon3 = Pon(&savMenu->v4Core->saveReader);
                starterPon3.pon_id = 2;
                starterPon3.pon_class = 1;
                starterPon3.pon_squad_position = 1;
                starterPon3.pon_exp = 0;
                starterPon3.pon_level = 1;
                //starterPon3.weapon_invItem_id = 1;
                //starterPon3.armour_invItem_id = 6;
                //starterPon3.weapon2_invItem_id = -1;
                //starterPon3.mask_invItem_id = -1;
                starterPon3.giveItem(1);
                starterPon3.giveItem(6);


                Pon starterPon4 = Pon(&savMenu->v4Core->saveReader);
                starterPon4.pon_id = 3;
                starterPon4.pon_class = 1;
                starterPon4.pon_squad_position = 2;
                starterPon4.pon_exp = 0;
                starterPon4.pon_level = 1;
                //starterPon4.weapon_invItem_id = 2;
                //starterPon4.armour_invItem_id = 7;
                //starterPon4.weapon2_invItem_id = -1;
                //starterPon4.mask_invItem_id = -1;
                starterPon4.giveItem(2);
                starterPon4.giveItem(7);

                savMenu->v4Core->saveReader.ponReg.pons.push_back(starterPon);
                savMenu->v4Core->saveReader.ponReg.pons.push_back(starterPon2);
                savMenu->v4Core->saveReader.ponReg.pons.push_back(starterPon3);
                savMenu->v4Core->saveReader.ponReg.pons.push_back(starterPon4);


                savMenu->v4Core->saveReader.Save();

            } else
            {
                savMenu->showPromptText = true;
            }
            break;
    }
}

NameEntryButtonList::~NameEntryButtonList()
{
}
