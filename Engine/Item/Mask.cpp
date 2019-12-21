#include "Mask.h"
#include "Item.h"


using namespace std;

Mask* Mask::FromItem(Item* item){
    Mask* mask = new Mask();
    mask->item_id = item->item_id;
    mask->item_name = item->item_name;
    mask->category_id = 2;
    mask->item_description = item->item_description;
    mask->subcategory_id = item->subcategory_id;
    mask->icon_path = item->icon_path;
    return mask;
}
