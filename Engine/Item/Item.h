#ifndef ITEM_H
#define ITEM_H
#include <SFML/Graphics.hpp>
#include "Equipment.h"
class Item
{
    public:
    std::string item_category;
    std::string item_type;
    std::vector<int> order_id;
    std::string item_name;
    std::string item_description;
    std::string icon_path;
    std::string spritesheet="none";
    int spritesheet_id=-1;
    Equipment* equip; // This isn't good but it's the best I've got rn
};

#endif // ITEM_H
