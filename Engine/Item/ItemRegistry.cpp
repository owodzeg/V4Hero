
#include "ItemRegistry.h"
#include "Item.h"
#include "../Func.h"
#include <fstream>
#include<iostream>
#include "Weapon.h"
#include "mask.h"
#include "Armour.h"
using namespace std;

ItemRegistry::ItemRegistry(){

}
void ItemRegistry::ReadItemFiles()
{
    /** REWORK THIS!!!!! The code can be more efficient and nice looking. **/

    vector<std::string> itemPaths;
    ifstream conf("resources/data/itemdata/aaa_item_registry.txt");
    if(conf.good())
    {
        string line;
        while(getline(conf, line))
        {
            ///ignore comments
            if(line.find("#") == std::string::npos && line.find("//") == std::string::npos)
            {
                ///register the item path
                itemPaths.push_back(line);
            }
        }
    }
    else
    {
        cout << "ERROR! Could not find item registry file!" << endl;
        return;
    }
    conf.close();
    bool quit=false;
    for (int i=0;i<itemPaths.size();i++){
        Item* newItem=new Item();
        ifstream itemStream("resources/data/itemdata/"+itemPaths[i]+".txt");

        cout << "itemStream(" << "resources/data/itemdata/"+itemPaths[i]+".txt)" << endl;

        if(itemStream.good())
        {
            string line;
            while(getline(itemStream, line))
            {
                ///ignore comments
                if(line.find("#") == std::string::npos && line.find("//") == std::string::npos)
                {
                    cout << "[ItemRegistry] " << line << endl;

                    ///read into the item
                    vector<string> key = Func::Split(line,':');

                    if(key[0]=="id")
                    {
                        newItem->item_id = stoi(key[1]);
                    }
                    else if(key[0]=="name")
                    {
                        std::wstring resws;
                        std::string str = key[1];
                        resws.assign(str.begin(), str.end());
                        newItem->item_name = resws;
                    }
                    else if(key[0]=="type")
                    {
                        newItem->category_id = stoi(key[1]);

                        cout << "category_id: " << key[1] << endl;

                        switch (stoi(key[1])){
                            case 1: /// weapon
                                {

                                quit=true;
                                ///Weapon* wep = new Weapon();
                                Weapon* wep = Weapon::FromItem(newItem);
                                //wep->item_id = newItem->item_id;
                                while(getline(itemStream, line))
                                {
                                    ///ignore comments
                                    if(line.find("#") == std::string::npos && line.find("//") == std::string::npos)
                                    {
                                        ///read into the item
                                        vector<string> key = Func::Split(line,':');
                                        if(key[0]=="id"){
                                            wep->item_id = stoi(key[1]);
                                        } else if(key[0]=="subtype"){
                                            wep->subcategory_id = stoi(key[1]);
                                        } else if(key[0]=="desc"){
                                            std::wstring resws;
                                            std::string str = key[1];
                                            resws.assign(str.begin(), str.end());
                                            wep->item_description = resws;
                                        } else if(key[0]=="icon"){
                                            wep->icon_path = key[1];
                                        } else if(key[0]=="mindmg"){
                                            wep->mindmg = stoi(key[1]);
                                        } else if(key[0]=="maxdmg"){
                                            wep->maxdmg = stoi(key[1]);
                                        } else if(key[0]=="crit"){
                                            wep->crit = stoi(key[1]);
                                        } else if(key[0]=="attackspeed"){
                                            wep->attackspeed = stoi(key[1]);
                                        } else if(key[0]=="equip_id"){
                                            wep->equip_id = stoi(key[1]);
                                        }
                                        else if(key[0]=="spritesheet")
                                        {
                                            wep->spritesheet = key[1];
                                        }
                                        else if(key[0]=="spritesheet_id")
                                        {
                                            wep->spritesheet_id = stoi(key[1]);
                                        }
                                    }
                                }
                                delete newItem;
                                newItem = wep;
                                cout<< newItem->item_id << wep->item_id<<'\n';
                                break;
                                }
                            case 2: /// mask
                                {


                                quit=true;
                                Mask* mask = Mask::FromItem(newItem);
                                while(getline(itemStream, line))
                                {
                                    ///ignore comments
                                    if(line.find("#") == std::string::npos && line.find("//") == std::string::npos)
                                    {
                                        ///read into the item
                                        vector<string> key = Func::Split(line,':');
                                        if(key[0]=="id"){
                                            mask->item_id = stoi(key[1]);
                                        } else if(key[0]=="subtype"){
                                            mask->subcategory_id = stoi(key[1]);
                                        } else if(key[0]=="desc"){
                                            std::wstring resws;
                                            std::string str = key[1];
                                            resws.assign(str.begin(), str.end());
                                            mask->item_description = resws;
                                        } else if(key[0]=="icon"){
                                            mask->icon_path = key[1];
                                        } else if(key[0]=="mindmg"){
                                            mask->mindmg = stoi(key[1]);
                                        } else if(key[0]=="maxdmg"){
                                            mask->maxdmg = stoi(key[1]);
                                        } else if(key[0]=="crit"){
                                            mask->crit = stoi(key[1]);
                                        } else if(key[0]=="attackspeed"){
                                            mask->attackspeed = stoi(key[1]);
                                        } else if(key[0]=="equip_id"){
                                            mask->equip_id = stoi(key[1]);
                                        }
                                        else if(key[0]=="spritesheet")
                                        {
                                            mask->spritesheet = key[1];
                                        }
                                        else if(key[0]=="spritesheet_id")
                                        {
                                            mask->spritesheet_id = stoi(key[1]);
                                        }
                                    }
                                }
                                delete newItem;
                                newItem = mask;
                                break;
                                }
                            case 3: /// armour
                                {
                                cout << "loading a new armor" << endl;

                                quit=true;
                                Armour* masks = Armour::FromItem(newItem);
                                while(getline(itemStream, line))
                                {
                                    ///ignore comments
                                    if(line.find("#") == std::string::npos && line.find("//") == std::string::npos)
                                    {
                                        ///read into the item
                                        vector<string> key = Func::Split(line,':');
                                        if(key[0]=="id"){
                                            masks->item_id = stoi(key[1]);
                                        } else if(key[0]=="subtype"){
                                            masks->subcategory_id = stoi(key[1]);
                                        } else if(key[0]=="desc"){
                                            std::wstring resws;
                                            std::string str = key[1];
                                            resws.assign(str.begin(), str.end());
                                            masks->item_description = resws;
                                        } else if(key[0]=="icon"){
                                            masks->icon_path = key[1];
                                        } else if(key[0]=="mindmg"){
                                            masks->mindmg = stoi(key[1]);
                                        } else if(key[0]=="maxdmg"){
                                            masks->maxdmg = stoi(key[1]);
                                        } else if(key[0]=="crit"){
                                            masks->crit = stoi(key[1]);
                                        } else if(key[0]=="attackspeed"){
                                            masks->attackspeed = stoi(key[1]);
                                        } else if(key[0]=="hp"){
                                            masks->hp = stoi(key[1]);
                                        } else if(key[0]=="equip_id"){
                                            masks->equip_id = stoi(key[1]);
                                        }
                                        else if(key[0]=="spritesheet")
                                        {
                                            masks->spritesheet = key[1];
                                        }
                                        else if(key[0]=="spritesheet_id")
                                        {
                                            masks->spritesheet_id = stoi(key[1]);
                                        }
                                    }
                                }
                                delete newItem;
                                newItem = masks;
                                break;
                                }
                            case 0: /// regular item/material
                            default:
                                quit=false;
                                break;
                        }
                        if (quit){
                            break;
                        }
                    }
                    else if(key[0]=="subtype")
                    {
                        newItem->subcategory_id = stoi(key[1]);
                    }
                    else if(key[0]=="desc")
                    {
                        std::wstring resws;
                        std::string str = key[1];
                        resws.assign(str.begin(), str.end());
                        newItem->item_description = resws;
                    }
                    else if(key[0]=="icon")
                    {
                        newItem->icon_path = key[1];
                    }
                    else if(key[0]=="spritesheet")
                    {
                        newItem->spritesheet = key[1];
                    }
                    else if(key[0]=="spritesheet_id")
                    {
                        newItem->spritesheet_id = stoi(key[1]);
                    }
                    else if(key[0]=="equip_id")
                    {
                        cout << "Setting equip_id to" << key[1] << endl;
                        newItem->equip_id = stoi(key[1]);
                    }
                }
            }
            cout << "Registered "<< itemPaths[i] <<" as an item ! (" << newItem->category_id << ")" << endl;
        }
        else
        {
            cout << "ERROR! Could not find "<< itemPaths[i] <<"item !" << endl;
            return;
        }
        itemStream.close();
        items.push_back(newItem);

    }
}
ItemRegistry::~ItemRegistry(){

}
Item* ItemRegistry::GetItemByID(int id)
{
    for (int i=0; i<items.size(); i++)
    {
        Item* currentItem = items[i];

        if (currentItem->item_id==id)
            return items[i];
    }

    cout << "A stupid happened: " << id <<'\n';
}
