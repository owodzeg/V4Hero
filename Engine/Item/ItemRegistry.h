#ifndef ITEM_REGISTRY_H
#define ITEM_REGISTRY_H
#include <SFML/Graphics.hpp>
#include "Item.h"
class ItemRegistry
{
    public:
        std::vector<Item*> items;
        ItemRegistry();
        ~ItemRegistry();
        Item* GetItemByID(int id);
        void ReadItemFiles();
};

#endif // ITEM_REGISTRY_H
