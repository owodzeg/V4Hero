#ifndef INVENTORY_DATA_H
#define INVENTORY_DATA_H
#include <SFML/Graphics.hpp>
#include "Item.h"
#include "InventoryItem.h"

class SaveReader;

class InventoryData
{
    public:
        SaveReader* savereader;

        std::vector<InventoryItem> items;
        InventoryItem GetItemByInvID(int id);
        int GetItemCountByTypeID(int id);
        int GetItemCountByName(std::string itemname);
        std::vector<InventoryItem> ItemsByType(int type);
        int CountItemsByType(int type);
        int InvItemIdFromType(int type,int itemId);
        bool CheckItemObtained(int itemID);
        InventoryData();
        ~InventoryData();
};

#endif // INVENTORY_DATA_H
