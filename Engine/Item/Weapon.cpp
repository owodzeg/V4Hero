#include "Weapon.h"
#include "Item.h"


using namespace std;

Weapon Weapon::FromItem(Item item){
    Weapon wep;
    wep.item_id = item.item_id;
    wep.item_name = item.item_name;
    wep.category_id = 1;
    wep.item_description = item.item_description;
    wep.subcategory_id = item.subcategory_id;
    wep.icon_path = item.icon_path;
    return wep;
}
