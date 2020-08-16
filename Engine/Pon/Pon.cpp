#include "Pon.h"
#include "../Item/InventoryItem.h"
#include "../Item/Weapon.h"
#include "../Item/Mask.h"
#include "../Item/Armour.h"
#include "../SaveReader.h"
using namespace std;

Pon::Pon(SaveReader* core)
{
    savReader=core;
}

void Pon::GiveItem(int InvItemId,int where)
{
    cout << "Pon::GiveItem(" << InvItemId << ", " << where << ")" << endl;

    InventoryItem currentItem = savReader->invdata.GetItemByInvID(InvItemId);

    cout << "Category ID: " << currentItem.item->category_id << endl;

    switch(currentItem.item->category_id)
    {
        case 1: /// weapons
        {
            cout << "Set weapon" << endl;

            if(where!=0)
            {
                if (weapon2_invItem_id!=-1)
                {
                    InventoryItem oldItem = savReader->invdata.GetItemByInvID(weapon2_invItem_id);
                    Weapon* owep = static_cast<Weapon*>(oldItem.item);
                    pon_min_dmg-=owep->mindmg;
                    pon_max_dmg-=owep->maxdmg;
                    pon_crit-=owep->crit;
                    pon_attack_speed=pon_attack_speed/(1-(owep->attackspeed/100));
                }

                weapon2_invItem_id=InvItemId;
                currentItem.PonIdRegistered = pon_id;
                Weapon* wep = static_cast<Weapon*>(currentItem.item);
                pon_min_dmg+=wep->mindmg;
                pon_max_dmg+=wep->maxdmg;
                pon_crit+=wep->crit;
                pon_attack_speed=pon_attack_speed-pon_attack_speed*wep->attackspeed/100;

                cout << "Setting weapon2_invItem_id to " << weapon2_invItem_id << endl;
            }
            else
            {
                if (weapon_invItem_id!=-1)
                {
                    InventoryItem oldItem = savReader->invdata.GetItemByInvID(weapon_invItem_id);
                    Weapon* owep = static_cast<Weapon*>(oldItem.item);
                    pon_min_dmg-=owep->mindmg;
                    pon_max_dmg-=owep->maxdmg;
                    pon_crit-=owep->crit;
                    pon_attack_speed=pon_attack_speed/(1-(owep->attackspeed/100));
                }

                this->weapon_invItem_id=InvItemId;
                currentItem.PonIdRegistered = pon_id;
                Weapon* wep = static_cast<Weapon*>(currentItem.item);
                pon_min_dmg+=wep->mindmg;
                pon_max_dmg+=wep->maxdmg;
                pon_crit+=wep->crit;
                pon_attack_speed=pon_attack_speed-pon_attack_speed*wep->attackspeed/100;

                cout << "Setting weapon_invItem_id to " << weapon_invItem_id << endl;
            }

            break;
        }

        case 2: /// mask
        {
            if (mask_invItem_id!=-1)
            {
                InventoryItem oldItem = savReader->invdata.GetItemByInvID(mask_invItem_id);
                Mask* omask = static_cast<Mask*>(oldItem.item);
                pon_min_dmg-=omask->mindmg;
                pon_max_dmg-=omask->maxdmg;
                pon_crit-=omask->crit;
                pon_attack_speed=pon_attack_speed/(1-(omask->attackspeed/100));
            }

            mask_invItem_id=InvItemId;
            currentItem.PonIdRegistered = pon_id;

            Mask* mask = static_cast<Mask*>(currentItem.item);
            pon_min_dmg+=mask->mindmg;
            pon_max_dmg+=mask->maxdmg;
            pon_crit+=mask->crit;
            pon_attack_speed=pon_attack_speed-pon_attack_speed*mask->attackspeed/100;
            break;
        }
        case 3: /// armour
        {
            if (armour_invItem_id!=-1){
                InventoryItem oldItem = savReader->invdata.GetItemByInvID(armour_invItem_id);
                Armour* oarmour = static_cast<Armour*>(oldItem.item);
                pon_min_dmg-=oarmour->mindmg;
                pon_max_dmg-=oarmour->maxdmg;
                pon_crit-=oarmour->crit;
                pon_attack_speed=pon_attack_speed/(1-(oarmour->attackspeed/100));
                pon_hp-=oarmour->hp;
            }

            armour_invItem_id=InvItemId;
            currentItem.PonIdRegistered = pon_id;
            Armour* armour = static_cast<Armour*>(currentItem.item);
            cout<<armour_invItem_id<<" -> "<<armour->item_id<<" -> "<<armour->mindmg<<"|"<<armour->maxdmg<<"|"<<armour->crit<<"|"<<armour->attackspeed<<"|"<<armour->hp<<'\n';
            pon_min_dmg+=armour->mindmg;
            pon_max_dmg+=armour->maxdmg;
            pon_crit+=armour->crit;
            pon_attack_speed=pon_attack_speed-pon_attack_speed*armour->attackspeed/100;
            pon_hp+=armour->hp;

            cout << "Setting armour_invItem_id to " << armour_invItem_id << endl;
            break;
        }

        case 0: /// regular material item
        default:
            break;
    }
}
void Pon::RemoveItem(int InvItemId,int where)
{
    InventoryItem currentItem = savReader->invdata.GetItemByInvID(InvItemId);
    switch(currentItem.item->category_id)
    {
        case 1: /// weapons
        {
            if(where!=0)
            {
                weapon2_invItem_id=InvItemId;
                currentItem.PonIdRegistered = pon_id;
            }
            else
            {
                weapon_invItem_id=InvItemId;
                currentItem.PonIdRegistered = pon_id;
            }
            break;
        }
        case 2: /// mask
        {
            mask_invItem_id=InvItemId;
            currentItem.PonIdRegistered = pon_id;
            break;
        }
        case 3: /// armour
        {
            armour_invItem_id=InvItemId;
            currentItem.PonIdRegistered = pon_id;
            break;
        }
        case 0: /// regular material item
        default:
            break;
    }
}
