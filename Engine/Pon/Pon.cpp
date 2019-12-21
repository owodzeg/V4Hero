#include "Pon.h"
#include "../Item/InventoryItem.h"
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
