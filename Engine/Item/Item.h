#ifndef ITEM_H
#define ITEM_H
#include <SFML/Graphics.hpp>
class Item
{
    public:
    int item_id;
    int category_id;
    int subcategory_id;
    int equip_id;
    std::wstring item_name;
    std::wstring item_description;
    std::string icon_path;
    std::string spritesheet="none";
    int spritesheet_id=-1;
};

#endif // ITEM_H
