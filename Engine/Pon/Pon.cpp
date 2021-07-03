#include "Pon.h"
#include "../SaveReader.h"

using namespace std;

Pon::Pon()
{
}

Pon::Pon(SaveReader* core)
{
    saveReader = core;
}

void Pon::recalculateStats()
{
    switch (pon_id)
    {
        case 0: {
            pon_base_hp = 100;
            pon_base_min_dmg = 0;
            pon_base_max_dmg = 0;
            pon_base_crit = 0;
            pon_base_attack_speed = 2.00;
            break;
        }
    }

    pon_hp = pon_base_hp;
    pon_min_dmg = pon_base_min_dmg;
    pon_max_dmg = pon_base_max_dmg;
    pon_crit = pon_base_crit;
    pon_attack_speed = pon_base_attack_speed;

    for (int i = 0; i < slots.size(); i++)
    {
        cout << "i: " << i << " slots[" << i << "]: " << slots[i] << endl;

        if (slots[i] > -1)
        {
            pon_hp += saveReader->invData.items[slots[i]].item->equip->hp;
            pon_min_dmg += saveReader->invData.items[slots[i]].item->equip->min_dmg;
            pon_max_dmg += saveReader->invData.items[slots[i]].item->equip->max_dmg;
            pon_crit += saveReader->invData.items[slots[i]].item->equip->crit;
            pon_attack_speed += saveReader->invData.items[slots[i]].item->equip->attack_speed;
        }
    }
}

void Pon::giveItem(int inv_item_id, int where)
{

    cout << "Pon::GiveItem(" << inv_item_id << ", " << where << ")" << endl;
    if (canEquip(saveReader->invData.items[inv_item_id].item->order_id, where))
    {
        slots[where] = inv_item_id;
        recalculateStats();
    } else
    {
        cout << "Couldn't equip item " << saveReader->invData.items[inv_item_id].item->item_name << " in slot " << where << endl;
    }
}
void Pon::removeItem(int where)
{
    switch (where)
    {
        case 0: {
            slot_1_invItem_id = -1;
            break;
        }
        case 1: {
            slot_2_invItem_id = -1;
            break;
        }
        case 2: {
            slot_3_invItem_id = -1;
            break;
        }
        case 4: {
            slot_5_invItem_id = -1;
            break;
        }
    }

    recalculateStats();
}

int Pon::getSlotCount()
{
    switch (pon_class)
    {
        case 0: // Yaripon
        {
            return 2;
        }
        case 1: // Tatepon
        {
            return 3;
        }
        case 2: // Yumipon
        {
            return 2;
        }
    }
}

bool Pon::canEquip(vector<int> eq_id, int where)
{
    if (eq_id.size() < 2 || eq_id[0] == 0)
    {
        cout << "[DEBUG] Attempted to equip a key item?" << endl;
        return false; // Why would you attempt to equip a key item
    }
    cout << "Asking if equipping {" << eq_id[0] << ", " << eq_id[1] << ", " << eq_id[2] << "} is viable (where=" << where << ")" << endl;
    switch (pon_class)
    {
        case 0: // Yaripon
        {
            if (eq_id[0] == 3 && eq_id[1] == 0 && where == 0)
                return true; // Spears
            if (eq_id[0] == 4 && eq_id[1] == 1 && where == 1)
                return true; // Helmets
            break;
        }

        case 1: // Tatepon
        {
            if (eq_id[0] == 3 && eq_id[1] == 1 && where == 0)
                return true; // Swords
            if (eq_id[0] == 4 && (eq_id[1] == 1 && where == 1))
                return true; // Helmets
            if (eq_id[0] == 4 && (eq_id[1] == 0 && where == 2))
                return true; // Shields
            break;
        }
    }
    return false;
}

//void Pon::()
