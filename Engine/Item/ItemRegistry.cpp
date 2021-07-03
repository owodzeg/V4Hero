#include "ItemRegistry.h"
#include "../Config.h"
#include "../Func.h"
#include "../SaveReader.h"
#include "Equipment.h"
#include "Item.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json; // Convenience recommended by the library

ItemRegistry::ItemRegistry()
{
}

void ItemRegistry::readItemFiles()
{
    items.clear();

    ifstream t("resources/data/item_data.json", std::ios::in);
    t >> item_data;
    for (int i = 0; i < item_data["items"].size(); i++)
    {
        for (int j = 0; j < item_data["items"][i].size(); j++)
        {
            if (item_data["items"][i][0].is_array()) // Check whether array or something else (if something else we know the array is 2D, not 3D) (technically, we could just check if i == 0 but futureproofing)
            {
                //type_counts[i]++;
                for (int k = 0; k < item_data["items"][i][j].size(); k++)
                {
                    switch (i)
                    {
                        case 0: ///Key items
                        {
                            //cout << "[ERROR] Key Items section of item_data.json thought to be 3 levels of arrays" << endl;
                            //item_counts[0]++;

                            // Note from Owocek (27.05.2021)
                            // I made Key Items section to be 3 level array just because it would be pain in the ass to differentiate 2 level and 3 level arrays when dropping items
                            // Or maybe you got it covered already and I screwed stuff up, but imo it's better if everything is 3 layer than having "exclusives", right?

                            Item* new_item = new Item();
                            new_item->item_category = "key_items";
                            new_item->item_type = "key_item";
                            new_item->item_name = item_data["items"][i][j][k]["name"];
                            new_item->item_description = item_data["items"][i][j][k]["desc"];
                            new_item->icon_path = item_data["items"][i][j][k]["icon"];
                            new_item->spritesheet = item_data["items"][i][j][k]["item_group"];
                            new_item->spritesheet_id = item_data["items"][i][j][k]["item_id"];
                            new_item->order_id.push_back(i);
                            new_item->order_id.push_back(j);
                            new_item->order_id.push_back(k);
                            items.push_back(new_item);
                            cout << "[DEBUG] Item Registered: " << new_item->order_id[0] << ", " << new_item->order_id[1] << ", " << new_item->order_id[2] << " " << new_item->item_name << endl;

                            break;
                        }

                        case 1: /// Materials
                        {
                            Item* new_item = new Item();
                            new_item->item_category = "materials";
                            switch (j)
                            {
                                case 0: {
                                    new_item->item_type = "wood";
                                    break;
                                }

                                case 1: {
                                    new_item->item_type = "meat";
                                    break;
                                }

                                case 2: {
                                    new_item->item_type = "fangs";
                                    break;
                                }

                                case 3: {
                                    new_item->item_type = "bones";
                                    break;
                                }

                                case 4: {
                                    new_item->item_type = "ore";
                                    break;
                                }
                            }
                            new_item->item_name = item_data["items"][i][j][k]["name"];
                            new_item->item_description = item_data["items"][i][j][k]["desc"];
                            new_item->icon_path = item_data["items"][i][j][k]["icon"];
                            new_item->spritesheet = item_data["items"][i][j][k]["item_group"];
                            new_item->spritesheet_id = item_data["items"][i][j][k]["item_id"];
                            new_item->order_id.push_back(i);
                            new_item->order_id.push_back(j);
                            new_item->order_id.push_back(k);
                            cout << "[DEBUG] Item Registered: " << new_item->order_id[0] << ", " << new_item->order_id[1] << ", " << new_item->order_id[2] << " " << new_item->item_name << endl;
                            items.push_back(new_item);

                            break;
                        }

                        case 2: /// Consumables
                        {
                            Item* new_item = new Item();
                            new_item->item_category = "consumables";
                            switch (j)
                            {
                                case 0: {
                                    new_item->item_type = "potion";
                                    break;
                                }
                            }
                            new_item->item_name = item_data["items"][i][j][k]["name"];
                            new_item->item_description = item_data["items"][i][j][k]["desc"];
                            new_item->icon_path = item_data["items"][i][j][k]["icon"];
                            new_item->spritesheet = item_data["items"][i][j][k]["item_group"];
                            new_item->spritesheet_id = item_data["items"][i][j][k]["item_id"];
                            new_item->order_id.push_back(i);
                            new_item->order_id.push_back(j);
                            new_item->order_id.push_back(k);
                            cout << "[DEBUG] Item Registered: " << new_item->order_id[0] << ", " << new_item->order_id[1] << ", " << new_item->order_id[2] << " " << new_item->item_name << endl;
                            items.push_back(new_item);

                            break;
                        }

                        case 3: /// Weapons
                        {
                            Item* new_weapon = new Item();
                            new_weapon->item_category = "weapon";
                            switch (j)
                            {
                                case 0: {
                                    new_weapon->item_type = "spear";
                                    break;
                                }

                                case 1: {
                                    new_weapon->item_type = "sword";
                                    break;
                                }
                            }

                            new_weapon->item_name = item_data["items"][i][j][k]["name"];
                            new_weapon->item_description = item_data["items"][i][j][k]["desc"];
                            new_weapon->icon_path = item_data["items"][i][j][k]["icon"];
                            new_weapon->spritesheet = item_data["items"][i][j][k]["item_group"];
                            new_weapon->spritesheet_id = item_data["items"][i][j][k]["item_id"];
                            new_weapon->equip = new Equipment();
                            new_weapon->equip->hp = item_data["items"][i][j][k]["hp"];
                            new_weapon->equip->min_dmg = item_data["items"][i][j][k]["min_dmg"];
                            new_weapon->equip->max_dmg = item_data["items"][i][j][k]["max_dmg"];
                            new_weapon->equip->crit = item_data["items"][i][j][k]["crit"];
                            new_weapon->equip->attack_speed = item_data["items"][i][j][k]["attack_speed"];
                            new_weapon->order_id.push_back(i);
                            new_weapon->order_id.push_back(j);
                            new_weapon->order_id.push_back(k);
                            cout << "[DEBUG] Item Registered: " << new_weapon->order_id[0] << ", " << new_weapon->order_id[1] << ", " << new_weapon->order_id[2] << ", " << new_weapon->icon_path << ", " << new_weapon->icon_path << endl;
                            items.push_back(new_weapon);

                            break;
                        }

                        case 4: /// Armour
                        {
                            Item* new_armour = new Item();
                            new_armour->item_category = "armor";
                            switch (j)
                            {
                                case 0: {
                                    new_armour->item_type = "shield";
                                    break;
                                }

                                case 1: {
                                    new_armour->item_type = "helm";
                                    break;
                                }

                                case 2: {
                                    new_armour->item_type = "mask";
                                    break;
                                }
                            }

                            new_armour->item_name = item_data["items"][i][j][k]["name"];
                            new_armour->item_description = item_data["items"][i][j][k]["desc"];
                            new_armour->icon_path = item_data["items"][i][j][k]["icon"];
                            new_armour->spritesheet = item_data["items"][i][j][k]["item_group"];
                            new_armour->spritesheet_id = item_data["items"][i][j][k]["item_id"];
                            new_armour->equip = new Equipment();
                            new_armour->equip->hp = item_data["items"][i][j][k]["hp"];
                            new_armour->equip->min_dmg = item_data["items"][i][j][k]["min_dmg"];
                            new_armour->equip->max_dmg = item_data["items"][i][j][k]["max_dmg"];
                            new_armour->equip->crit = item_data["items"][i][j][k]["crit"];
                            new_armour->equip->attack_speed = item_data["items"][i][j][k]["attack_speed"];
                            new_armour->order_id.push_back(i);
                            new_armour->order_id.push_back(j);
                            new_armour->order_id.push_back(k);
                            cout << "[DEBUG] Item Registered: " << new_armour->order_id[0] << ", " << new_armour->order_id[1] << ", " << new_armour->order_id[2] << ", " << new_armour->icon_path << ", " << new_armour->item_name << endl;
                            items.push_back(new_armour);

                            break;
                        }
                    }
                    //if(i != 0) // Not sure
                    //{
                    //item_counts[type_counts[i] + j]++;
                    //}
                }
            } else
            {
                //type_counts[i] = 1;
                switch (i)
                {
                    case 0: ///Key items
                    {
                        Item* new_item = new Item();
                        new_item->item_category = "key_items";
                        new_item->item_type = "key_item";
                        new_item->item_name = item_data["items"][i][j]["name"];
                        new_item->item_description = item_data["items"][i][j]["desc"];
                        new_item->icon_path = item_data["items"][i][j]["icon"];
                        new_item->spritesheet = item_data["items"][i][j]["item_group"];
                        new_item->spritesheet_id = item_data["items"][i][j]["item_id"];
                        new_item->order_id.push_back(i);
                        new_item->order_id.push_back(j);
                        items.push_back(new_item);
                        cout << "[DEBUG] Item Registered: " << new_item->order_id[0] << ", " << new_item->order_id[1] << endl;
                        //item_counts[0]++;
                        break;
                    }

                    case 1: /// Materials
                    {
                        cout << "[ERROR] Materials section of item_data.json thought to be only 2 levels of arrays" << endl;
                        //item_counts[type_counts[1] + j]++; // I dunno but I might as well see if it works
                        break;
                    }

                    case 2: /// Consumables
                    {
                        cout << "[ERROR] Consumables section of item_data.json thought to be only 2 levels of arrays" << endl;
                        //item_counts[type_counts[2] + j]++;
                        break;
                    }

                    case 3: /// Weapons
                    {
                        cout << "[ERROR] Weapons section of item_data.json thought to be only 2 levels of arrays" << endl;
                        //item_counts[type_counts[4] + j]++;
                        break;
                    }

                    case 4: /// Armour
                    {
                        cout << "[ERROR] Armour section of item_data.json thought to be only 2 levels of arrays" << endl;
                        //item_counts[type_counts[5] + j]++;
                        break;
                    }
                }
            }
        }
    }

    cout << "ItemRegistry::readItemFiles(): amount of items: " << items.size() << endl;

    /*
    cout << "Loaded items:" << endl;
    for(int i = 0; i < items.size(); i++)
    {
        cout << "Item: " << items[i]->item_name << endl;
    }
    */
    // Use this to print out all loaded items
    /*
    cout << "Item counts:" << endl;
    for(int i = 0; i < item_counts.size(); i++)
    {
        for(int o = 0; o < item_counts[i].size(); o++)
        {
            cout << "Category:" << i << ", Type:" << o << ", Count:" << item_counts[i][o] << endl;
        }
    }
    */
    // Use this to print out all item counts
}
ItemRegistry::~ItemRegistry()
{
}

Item* ItemRegistry::getItemByID(std::vector<int> id)
{
    for (int i = 0; i < items.size(); i++) // Just to test other stuff
    {
        if (items[i]->order_id == id)
        {
            return items[i];
        }
    }
    /*
    int total_id = 0;
    for(int i = type_counts[id[0]]; i >= 0; i--)
    {
        if(i == type_counts[id[0]])
        {
            o = id[1];
        }
        else
        {
            o = item_counts[i][type_counts[]]
        }
        for(; o)
    }
    cerr  << "Returning: " << items[total_id + id[id.size() - 1]]->item_name << endl << endl;
    cerr << "total_id is " << total_id << endl;
    if(items[total_id + id[id.size() - 1]]->order_id != id)
    {
        cout << "[ERROR] Item \"found\" but is wrong" << endl;
    }
    else
    {
        return items[total_id + id[id.size() - 1]];
    }
    */
    if (id.size() > 2)
    {
        cout << "[ERROR] Item registry failed to found item of id{" << id[0] << " " << id[1] << " " << id[2] << endl;
    } else
    {
        cout << "[ERROR] Item registry failed to found item of id{" << id[0] << " " << id[1] << endl;
    }
}

int ItemRegistry::getCategoryIDByString(std::string item_category)
{
    if (item_category == "key_items")
        return Categories::KEY_ITEMS;
    if (item_category == "materials")
        return Categories::MATERIALS;
    if (item_category == "consumables")
        return Categories::CONSUMABLES;
    if (item_category == "weapon")
        return Categories::WEAPONS;
    if (item_category == "armor")
        return Categories::ARMOR;
}

Item* ItemRegistry::getItemByName(std::string name, bool lang_specific)
{
    if (lang_specific) // By e.g. Wooden Spear (won't return if comparing between languages)
    {
        string converted_name = Func::ConvertToUtf8String(saveReader->thisConfig->strRepo.GetUnicodeString(name));
        for (int i = 0; i < items.size(); i++)
        {
            if (Func::ConvertToUtf8String(saveReader->thisConfig->strRepo.GetUnicodeString(items[i]->item_name)) == converted_name)
            {
                return items[i];
            }
        }
    } else // By e.g. item_wooden_spear
    {
        for (int i = 0; i < items.size(); i++)
        {
            if (items[i]->item_name == name)
            {
                return items[i];
            }
        }
    }
}