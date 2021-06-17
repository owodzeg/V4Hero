#include "Json/json.hpp"
#include "SaveReader.h"
#include "Func.h"
#include "DebugOut.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <string>
#include "config.h"

using namespace std;
using json = nlohmann::json; // Convenience recommended by the library

SaveReader::SaveReader()
{
    ifstream check("resources/data/sv1.p4sv");
    bool exists = check.good();
    check.close();

    ///if config not exists -update: then dont create fresh one :)
    /*if(!exists)
    {
        ofstream conf("resources/data/sv1.p4sv");

        if(conf.is_open())
        {
            ///Safety warning
            conf << "# Take caution! The data below represents your save data! Don't edit it unless you know what you're doing, and if you must, PLEASE back it up somewhere else first <3 #";
            conf << '\n';
        }

        conf.close();
    }*/

    isNewSave = !exists;

    itemReg.saveReader = this;
    itemReg.readItemFiles(); ///load up items

    invData.saveReader = this;

    cout << invData.itemReg.items.size() << endl; ///says 35
}

void SaveReader::LoadSave(Config& tconfig)
{
    thisConfig = &tconfig;
    debugOut = thisConfig->debugOut;

    ifstream conf("resources/data/sv1.p4sv", std::ios::in);
    if(conf.good())
    {
        conf >> save_data;
        save_data = save_data["save"];

        cout << "Fetching save metadata" << endl;
        save_ver = save_data["details"]["version"];
        kami_name = sf::String(to_string(save_data["details"]["name"]));
        times_launched = save_data["details"]["times_launched"];
        times_launched++;
        locations_unlocked = save_data["details"]["locations_unlocked"];

        cout << "Fetching owned items" << endl;
        for(int i = 0; i < save_data["items"].size(); i++)
        {
            vector<int> cur_item_id;
            for(int o = 0; o < save_data["items"][i].size(); o++)
            {
                cur_item_id.push_back(save_data["items"][i][o]);
            }
            invData.addItem(cur_item_id);
        }

        cout << "Fetching hero data" << endl;
        if(save_data["army"][0]["rarepon"] != -1) // Is hero unlocked?
        {
            hero_unlocked = true;
            Pon new_pon = Pon(this);
            new_pon.pon_id = save_data["army"][0]["rarepon"];
            new_pon.pon_class = save_data["army"][0]["class"];
            new_pon.pon_level = save_data["army"][0]["level"];
            new_pon.pon_exp = save_data["army"][0]["exp"];
            for(int o = 0; o < save_data["army"][0]["slots"].size(); o++)
            {
                if(save_data["army"][0]["slots"][0] != -1) // Is this necessary?
                {
                    new_pon.giveItem(invData.getInvIDByItemID(save_data["army"][0]["slots"][o]), o);
                }
            }
            ponReg.pons.push_back(new_pon);
        }

        int comparison = save_data["army"].size();
        if(comparison>4)
        comparison = 4;

        for(int i = 1; i < comparison; i++)
        {
            for(int o = 0; o < save_data["army"][i].size(); o++)
            {
                Pon new_pon = Pon(this);
                new_pon.pon_id = save_data["army"][i][o]["rarepon"];
                new_pon.pon_class = save_data["army"][i][o]["class"];
                new_pon.pon_level = save_data["army"][i][o]["level"];
                new_pon.pon_exp = save_data["army"][i][o]["exp"];
                for(int p = 0; p < save_data["army"][i][o]["slots"].size(); p++)
                {
                    cout << "save_data[\"army\"][" << i << "][" << o << "][\"slots\"][" << p << "] = " << save_data["army"][i][o]["slots"][p] << endl;

                    if(save_data["army"][i][o]["slots"][p][0] != -1) // Is this necessary?
                    {
                        new_pon.giveItem(invData.getInvIDByItemID(save_data["army"][i][o]["slots"][p]), p);
                    }
                }
                ponReg.pons.push_back(new_pon);
            }

            ponReg.squads_available.push_back(save_data["army"][i][0]["class"]); // Save to available squads for choosing in barracks / prep
            ponReg.army.push_back(save_data["army"][i][0]["class"]); // The first 3 squads are in army, to change the limit of squads in army change the `i < 4` above and `i = 4` below
        }

        for(int i = 4; i < save_data["army"].size(); i++)
        {
            for(int o = 0; o < save_data["army"][i].size(); o++)
            {
                Pon new_pon = Pon(this);
                new_pon.pon_id = save_data["army"][i][o]["rarepon"];
                new_pon.pon_class = save_data["army"][i][o]["class"];
                new_pon.pon_level = save_data["army"][i][o]["level"];
                new_pon.pon_exp = save_data["army"][i][o]["exp"];
                for(int p = 0; p < save_data["army"][i][o]["slots"].size(); p++)
                {
                    if(save_data["army"][i][o]["slots"][0] != -1) // Is this necessary?
                    {
                        new_pon.giveItem(invData.getInvIDByItemID(save_data["army"][i][o]["slots"][p]), p);
                    }
                }
                ponReg.pons.push_back(new_pon);
            }

            ponReg.squads_available.push_back(save_data["army"][i][0]["class"]); // Save to available squads for choosing in barracks / prep
        }

        cout << "Fetching unlocked missions" << endl;
        for(int i = 0; i < save_data["missions"].size(); i++) // Each mission is stored as a single array so it can be handled elegantly (slightly increases file size, but shouldn't be a problem)
        {
            if(save_data["missions"][i][2]) // Is unlocked?
            {
                missions_unlocked.push_back(save_data["missions"][i][0]); // Add mission id
            }
            mission_levels.insert(pair<int, int>(save_data["missions"][i][0], save_data["missions"][i][0])); // Add mission level regardless
        }
    }
    else
    {
        cout << "ERROR! Could not load save file!" << endl;
    }
    conf.close();
}

void SaveReader::Flush() ///Empties the save data.
{
    missions_unlocked.clear();
    mission_levels.clear();
    locations_unlocked = 1;

    //invdata.items.clear();

    PonRegistry n_ponReg;
    InventoryData n_invData;

    ponReg = n_ponReg;
    invData = n_invData;
}

void SaveReader::CreateBlankSave() ///Creates a blank save data for use
{
    cout << "SaveReader::CreateBlankSave()" << endl;

    ///name of god
    kami_name = "Kamipon";

    ///times launched (unnecessary?)
    times_launched = 0;

    ///Adding starter items
    vector<string> starter_items = {"wooden_spear", "wooden_spear", "wooden_spear", "wooden_helmet", "wooden_helmet", "wooden_helmet"};

    for(int i=0; i<starter_items.size(); i++)
    {
        InventoryData::InventoryItem inv_item;
        inv_item.item = itemReg.getItemByName(starter_items[i]);
        invData.items.push_back(inv_item);

        cout << "Adding item with InvID " << invData.getInvIDByItemID(inv_item.item->order_id) << " realID: {" << inv_item.item->order_id[0] << ", " << inv_item.item->order_id[1];
        if(inv_item.item->order_id.size() > 2)
        {
            cout << ", " << inv_item.item->order_id[2];
        }
        cout << "}" << endl;
    }

    ///Defining 3 Yaripons

    for(int i=0; i<3; i++)
    {
        Pon newPon = Pon(this);

        newPon.pon_id = i;
        newPon.pon_class = 1;
        newPon.pon_squad_position = i;

        newPon.pon_exp = 0;
        newPon.pon_level = 1;

        newPon.giveItem(0+i);
        newPon.giveItem(3+i);

        ponReg.pons.push_back(newPon);
    }

    ///Worldmap data
    missions_unlocked.push_back(1);
    locations_unlocked = 1;
}

void SaveReader::Save()
{
    ofstream save_file("resources/data/sv1.p4sv", ios::ate);
    save_file.seekp(0); // Honestly no idea what this does but it was in the previous version

    json save_json;

    save_json["save"]["details"]["version"] = 2.0;
    save_json["save"]["details"]["name"] = kami_name;
    save_json["save"]["details"]["times_launched"] = times_launched;
    save_json["save"]["details"]["locations_unlocked"] = locations_unlocked;

    for(int i = 0; i < invData.items.size(); i++)
    {
        for(int o = 0; o < invData.items[i].item_count; o++)
        {
            save_json["save"]["items"] += invData.items[i].item->order_id;
        }
    }

    if(hero_unlocked)
    {
        save_json["save"]["army"][0]["rarepon"] = ponReg.pons[0].pon_id;
        save_json["save"]["army"][0]["class"] = ponReg.pons[0].pon_class;
        save_json["save"]["army"][0]["level"] = ponReg.pons[0].pon_id;
        save_json["save"]["army"][0]["exp"] = ponReg.pons[0].pon_id;
        save_json["save"]["army"][0]["slot_1"] = invData.items[ponReg.pons[0].slot_1_invItem_id].item->order_id;
        save_json["save"]["army"][0]["slot_2"] = invData.items[ponReg.pons[0].slot_2_invItem_id].item->order_id;
        save_json["save"]["army"][0]["slot_3"] = invData.items[ponReg.pons[0].slot_3_invItem_id].item->order_id;
        save_json["save"]["army"][0]["slot_4"] = invData.items[ponReg.pons[0].slot_4_invItem_id].item->order_id;
        save_json["save"]["army"][0]["slot_5"] = invData.items[ponReg.pons[0].slot_5_invItem_id].item->order_id;
    }

    int squad_pos = 0;
    for(int i = 0; i < ponReg.squads_available.size(); i++)
    {
        for(int o = 0; o < ponReg.pons.size(); o++)
        {
            if(ponReg.pons[o].pon_class == ponReg.squads_available[i])
            {
                int temp_i = i;
                if(hero_unlocked)
                {
                    temp_i++;
                }
                save_json["save"]["army"][temp_i][squad_pos]["rarepon"] = ponReg.pons[o].pon_id;
                save_json["save"]["army"][temp_i][squad_pos]["class"] = ponReg.pons[o].pon_class;
                save_json["save"]["army"][temp_i][squad_pos]["level"] = ponReg.pons[o].pon_id;
                save_json["save"]["army"][temp_i][squad_pos]["exp"] = ponReg.pons[o].pon_id;

                if (ponReg.pons[o].slot_1_invItem_id != -1)
                    save_json["save"]["army"][temp_i][squad_pos]["slot_1"] = invData.items[ponReg.pons[o].slot_1_invItem_id].item->order_id;
                else
                    save_json["save"]["army"][temp_i][squad_pos]["slot_1"] = { -1 };

                if (ponReg.pons[o].slot_2_invItem_id != -1)
                    save_json["save"]["army"][temp_i][squad_pos]["slot_2"] = invData.items[ponReg.pons[o].slot_2_invItem_id].item->order_id;
                else
                    save_json["save"]["army"][temp_i][squad_pos]["slot_2"] = { -1 };

                if (ponReg.pons[o].slot_3_invItem_id != -1)
                    save_json["save"]["army"][temp_i][squad_pos]["slot_3"] = invData.items[ponReg.pons[o].slot_3_invItem_id].item->order_id;
                else
                    save_json["save"]["army"][temp_i][squad_pos]["slot_3"] = { -1 };

                if (ponReg.pons[o].slot_4_invItem_id != -1)
                    save_json["save"]["army"][temp_i][squad_pos]["slot_4"] = invData.items[ponReg.pons[o].slot_4_invItem_id].item->order_id;
                else
                    save_json["save"]["army"][temp_i][squad_pos]["slot_4"] = { -1 };

                if (ponReg.pons[o].slot_5_invItem_id != -1)
                    save_json["save"]["army"][temp_i][squad_pos]["slot_5"] = invData.items[ponReg.pons[o].slot_5_invItem_id].item->order_id;
                else
                    save_json["save"]["army"][temp_i][squad_pos]["slot_5"] = { -1 };

                squad_pos++;
            }
        }
        squad_pos = 0;
    }

    map<int, int>::iterator it;
    for(it = mission_levels.begin(); it != mission_levels.end(); it++)
    {
        save_json["save"]["missions"][it->first][0] = it->first;
        save_json["save"]["missions"][it->first][1] = it->second;
        save_json["save"]["missions"][it->first][2] = isMissionUnlocked(it->first);
    }

    save_file << save_json;
    save_file.close();
}

bool SaveReader::isMissionUnlocked(int mission)
{
    if(std::find(missions_unlocked.begin(), missions_unlocked.end(), mission) != missions_unlocked.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}
