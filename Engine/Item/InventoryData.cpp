
#include "InventoryData.h"
#include "Item.h"
#include "InventoryItem.h"


using namespace std;

InventoryData::InventoryData(){

}
InventoryData::~InventoryData(){

}
InventoryItem InventoryData::GetItemByInvID(int id){
    return items[id];
}
int InventoryData::GetItemCountByTypeID(int id){
    int totalcount=0;
    for (int i=0;i<items.size()-1;i++){
        Item currentItem = *items[i].item;
        if (currentItem.item_id==id)
            totalcount++;
    }
    return totalcount;
}
std::vector<InventoryItem> InventoryData::ItemsByType(int type){
    std::vector<InventoryItem> typeItems;
    for (int i=0;i<items.size()-1;i++){
        InventoryItem currentItem = items[i];
        if (currentItem.item->category_id==type)
            typeItems.push_back(currentItem);
    }
    return typeItems;
}
int InventoryData::CountItemsByType(int type){
    int totalcount=0;
    for (int i=0;i<items.size()-1;i++){
        Item currentItem = *items[i].item;
        if (currentItem.category_id==type)
            totalcount++;
    }
    return totalcount;
}
