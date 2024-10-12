#ifndef ITEM_REGISTRY_H
#define ITEM_REGISTRY_H
#include "Item.h"
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

class SaveReader;

class ItemRegistry
{
public:
    std::vector<Item*> items;
    //std::vector<int> item_counts = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    //std::vector<int> type_counts = {0, 0, 0, 0, 0};
    nlohmann::json item_data;
    ItemRegistry();
    ~ItemRegistry();
    Item* getItemByName(std::string name, bool lang_specific = false);
    enum Categories
    {
        KEY_ITEMS = 0,
        MATERIALS = 1,
        CONSUMABLES = 2,
        WEAPONS = 3,
        ARMOR = 4
    };
    void readItemFiles();
    Item* getItemByID(std::vector<int> id);
    std::vector<int> findItemByCatID(std::string cat, int id);
    int getCategoryIDByString(std::string item_category);
};

#endif // ITEM_REGISTRY_H
