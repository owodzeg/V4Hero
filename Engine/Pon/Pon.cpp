#include "Pon.h"
#include "../Item/InventoryItem.h"
#include "../Item/Weapon.h"
#include "../SaveReader.h"
using namespace std;

Pon::Pon(SaveReader* core){
    savReader=core;
}

void Pon::GiveItem(int InvItemId,int where)
{
    InventoryItem currentItem = savReader->invdata.GetItemByInvID(InvItemId);
    switch(currentItem.item->category_id){
        case 1: /// weapons
            if(where!=0){
                weapon2_invItem_id=InvItemId;
                currentItem.PonIdRegistered = pon_id;
            } else {
                this->weapon_invItem_id=InvItemId;
                currentItem.PonIdRegistered = pon_id;
                Weapon* wep = static_cast<Weapon*>(currentItem.item);
                cout<<weapon_invItem_id<<" -> "<<wep->item_id<<" -> "<<wep->mindmg<<"|"<<wep->maxdmg<<"|"<<wep->crit<<"|"<<wep->attackspeed<<'\n';
                pon_min_dmg+=wep->mindmg;
                pon_max_dmg+=wep->maxdmg;
                pon_crit+=wep->crit;
                pon_attack_speed+=wep->attackspeed;
            }
            break;
        case 2: /// mask
            mask_invItem_id=InvItemId;
            currentItem.PonIdRegistered = pon_id;
            break;
        case 3: /// armour
            armour_invItem_id=InvItemId;
            currentItem.PonIdRegistered = pon_id;
            break;
        case 0: /// regular material item
        default:
            break;
    }
}
void Pon::RemoveItem(int InvItemId,int where)
{
    InventoryItem currentItem = savReader->invdata.GetItemByInvID(InvItemId);
    switch(currentItem.item->category_id){
        case 1: /// weapons
            if(where!=0){
                weapon2_invItem_id=InvItemId;
                currentItem.PonIdRegistered = pon_id;
            } else {
                weapon_invItem_id=InvItemId;
                currentItem.PonIdRegistered = pon_id;
            }
            break;
        case 2: /// mask
            mask_invItem_id=InvItemId;
            currentItem.PonIdRegistered = pon_id;
            break;
        case 3: /// armour
            armour_invItem_id=InvItemId;
            currentItem.PonIdRegistered = pon_id;
            break;
        case 0: /// regular material item
        default:
            break;
    }
}
