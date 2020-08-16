#include "Armour.h"
#include "Item.h"
#include <iostream>

using namespace std;

Armour* Armour::FromItem(Item* item)
{
    cout << "Armour::FromItem:" << endl;

    Armour* wep = new Armour();
    wep->item_id = item->item_id;
    wep->item_name = item->item_name;
    wep->category_id = 3;
    wep->item_description = item->item_description;
    wep->subcategory_id = item->subcategory_id;
    wep->icon_path = item->icon_path;

    cout << "category_id: " << wep->category_id << endl;

    return wep;
}
