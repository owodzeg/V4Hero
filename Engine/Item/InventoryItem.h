#ifndef INVENTORY_ITEM_H
#define INVENTORY_ITEM_H
#include <SFML/Graphics.hpp>
#include "InventoryItem.h"
#include "Item.h"
class InventoryItem
{
    public:
    Item* item;
    int PonIdRegistered=-1;
};

#endif // INVENTORY_ITEM_H
