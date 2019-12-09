#ifndef ITEM_H
#define ITEM_H
#include <SFML/Graphics.hpp>
class Item
{
    public:
    int item_id;
    int category_id;
    int subcategory_id;
    std::wstring item_name;
    std::wstring item_description;
    std::string icon_path;
};

#endif // ITEM_H
