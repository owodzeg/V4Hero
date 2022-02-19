#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "SaveReader.h"
#include "Config.h"
#include "Func.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <string>
#include "CoreManager.h"

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
}

//TO-DO: remove the argument version
void SaveReader::LoadSave(Config& tconfig)
{
    //lol config is not even needed here
}

void SaveReader::LoadSave()
{
    ifstream conf("resources/data/sv1.p4sv", std::ios::in);
    if (conf.good())
    {
        conf >> save_data;
        save_data = save_data["save"];

        save_ver = save_data["details"]["version"];
        kami_name = sf::String(to_string(save_data["details"]["name"]));
        story_point = save_data["details"]["story_point"];
        locations_unlocked.clear(); // Clear the default from SaveReader.h
        for (int i = 0; i < save_data["details"]["locations_unlocked"].size(); i++)
        {
            locations_unlocked.push_back(save_data["details"]["locations_unlocked"][i]);
        }

        for (int i = 0; i < save_data["items"].size(); i++)
        {
            invData.addItem(itemReg.getItemByName(save_data["items"][i]["name"])->order_id, itemReg, save_data["items"][i]["count"]);
        }

        if (save_data["army"][0]["rarepon"] != -1) // Is hero unlocked?
        {
            hero_unlocked = true;
            Pon new_pon = Pon(this);
            new_pon.pon_id = save_data["army"][0]["rarepon"];
            new_pon.pon_class = save_data["army"][0]["class"];
            new_pon.pon_level = save_data["army"][0]["level"];
            new_pon.pon_exp = save_data["army"][0]["exp"];
            for (int o = 0; o < save_data["army"][0]["slots"].size(); o++)
            {
                if (save_data["army"][0]["slots"][0] != -1) // Is this necessary?
                {
                    new_pon.giveItem(invData.getInvIDByItemID(save_data["army"][0]["slots"][o]), o);
                }
            }
            ponReg.pons.push_back(new_pon);
        }

        int comparison = save_data["army"].size();
        if (comparison > 4)
            comparison = 4;

        for (int i = 1; i < comparison; i++)
        {
            for (int o = 0; o < save_data["army"][i].size(); o++)
            {
                Pon new_pon = Pon(this);
                new_pon.pon_id = save_data["army"][i][o]["rarepon"];
                new_pon.pon_class = save_data["army"][i][o]["class"];
                new_pon.pon_level = save_data["army"][i][o]["level"];
                new_pon.pon_exp = save_data["army"][i][o]["exp"];
                for (int p = 0; p < save_data["army"][i][o]["slots"].size(); p++)
                {
                    if (save_data["army"][i][o]["slots"][p][0] != -1) // Is this necessary?
                    {
                        new_pon.giveItem(invData.getInvIDByItemID(save_data["army"][i][o]["slots"][p]), p);
                    }
                }
                ponReg.pons.push_back(new_pon);
            }

            ponReg.squads_available.push_back(save_data["army"][i][0]["class"]); // Save to available squads for choosing in barracks / prep
            ponReg.army.push_back(save_data["army"][i][0]["class"]);             // The first 3 squads are in army, to change the limit of squads in army change the `i < 4` above and `i = 4` below
        }

        for (int i = 4; i < save_data["army"].size(); i++)
        {
            for (int o = 0; o < save_data["army"][i].size(); o++)
            {
                Pon new_pon = Pon(this);
                new_pon.pon_id = save_data["army"][i][o]["rarepon"];
                new_pon.pon_class = save_data["army"][i][o]["class"];
                new_pon.pon_level = save_data["army"][i][o]["level"];
                new_pon.pon_exp = save_data["army"][i][o]["exp"];
                for (int p = 0; p < save_data["army"][i][o]["slots"].size(); p++)
                {
                    if (save_data["army"][i][o]["slots"][0] != -1) // Is this necessary?
                    {
                        new_pon.giveItem(invData.getInvIDByItemID(save_data["army"][i][o]["slots"][p]), p);
                    }
                }
                ponReg.pons.push_back(new_pon);
            }

            ponReg.squads_available.push_back(save_data["army"][i][0]["class"]); // Save to available squads for choosing in barracks / prep
        }

        for (int i = 0; i < save_data["missions"].size(); i++) // Each mission is stored as a single array so it can be handled elegantly (slightly increases file size, but shouldn't be a problem)
        {
            if (save_data["missions"][i][2]) // Is unlocked?
            {
                missions_unlocked.push_back(save_data["missions"][i][0]); // Add mission id
            }
            mission_levels.insert(pair<int, int>(save_data["missions"][i][0], save_data["missions"][i][1])); // Add mission level regardless
        }
    } else
    {
        spdlog::error("Could not load save file");
    }
    conf.close();
}

void SaveReader::Flush() ///Empties the save data.
{
    missions_unlocked.clear();
    mission_levels.clear();
    locations_unlocked = {1};

    //invdata.items.clear();

    PonRegistry n_ponReg;
    InventoryData n_invData;

    ponReg = n_ponReg;
    invData = n_invData;
}

void SaveReader::CreateBlankSave()
{
    spdlog::info("Creating blank save...");

    kami_name = "Kamipon";

    vector<string> starter_items = {"item_wooden_spear", "item_wooden_spear", "item_wooden_spear", "item_wooden_helmet", "item_wooden_helmet", "item_wooden_helmet"};
    for (int i = 0; i < starter_items.size(); i++)
    {
        invData.addItem(itemReg.getItemByName(starter_items[i])->order_id, itemReg);
    }

    // Defining 3 Yaripons

    for (int i = 0; i < 3; i++)
    {
        Pon newPon = Pon(this);

        newPon.pon_id = i;
        newPon.pon_class = 0;
        newPon.pon_squad_position = i;

        newPon.pon_exp = 0;
        newPon.pon_level = 1;

        newPon.giveItem(invData.getInvIDByItemID(itemReg.getItemByName("item_wooden_spear")->order_id), 0);
        newPon.giveItem(invData.getInvIDByItemID(itemReg.getItemByName("item_wooden_helmet")->order_id), 1);

        ponReg.pons.push_back(newPon);
    }

    ponReg.squads_available.push_back(0);

    // Worldmap data
    missions_unlocked.push_back(1);
    locations_unlocked = {1};

    story_point = 1;

    spdlog::info("Finished creating blank save");
}

void SaveReader::Save()
{
    ofstream save_file("resources/data/sv1.p4sv", ios::ate);
    save_file.seekp(0); // Honestly no idea what this does but it was in the previous version

    json save_json;

    save_json["save"]["details"]["version"] = "2.0";
    save_json["save"]["details"]["name"] = kami_name;
    save_json["save"]["details"]["locations_unlocked"] = locations_unlocked;
    save_json["save"]["details"]["story_point"] = story_point;

    for (int i = 0; i < invData.items.size(); i++)
    {
        json item;

        item["name"] = invData.items[i].item->item_name;
        item["count"] = invData.items[i].item_count;

        save_json["save"]["items"] += item;
    }

    if (hero_unlocked)
    {
        save_json["save"]["army"][0]["rarepon"] = ponReg.pons[0].pon_id;
        save_json["save"]["army"][0]["class"] = ponReg.pons[0].pon_class;
        save_json["save"]["army"][0]["level"] = ponReg.pons[0].pon_id;
        save_json["save"]["army"][0]["exp"] = ponReg.pons[0].pon_id;
        std::vector<std::vector<int>> slots;
        for (int p = 0; p < 5; p++) // Potentially dangerous magic number. Use ponReg.pons[o].slots.size() in case it causes trouble
        {
            if (ponReg.pons[0].slots[p] != -1)
            {
                std::vector<int> curItem = invData.items[ponReg.pons[0].slots[p]].item->order_id;
                slots.push_back(curItem);
            } else
            {
                slots.push_back(std::vector<int>(1, -1)); // empty or doesn't exist
            }
        }
        save_json["save"]["army"][0]["slots"] = slots;
    } else
    {
        save_json["save"]["army"][0]["rarepon"] = -1; // This is the laziest way to fix this crash that I could possibly think of
    }

    int squad_pos = 0;
    for (int i = 1; i <= ponReg.squads_available.size(); i++) // Skip hero
    {
        for (int o = 0; o < ponReg.pons.size(); o++)
        {
            if (ponReg.pons[o].pon_class == ponReg.squads_available[i - 1])
            {
                save_json["save"]["army"][i][squad_pos]["rarepon"] = ponReg.pons[o].pon_id;
                save_json["save"]["army"][i][squad_pos]["class"] = ponReg.pons[o].pon_class;
                save_json["save"]["army"][i][squad_pos]["level"] = ponReg.pons[o].pon_level;
                save_json["save"]["army"][i][squad_pos]["exp"] = ponReg.pons[o].pon_exp;

                std::vector<std::vector<int>> slots;
                for (int p = 0; p < 5; p++) // Potentially dangerous magic number. Use ponReg.pons[o].slots.size() in case it causes trouble
                {
                    if (ponReg.pons[o].slots[p] != -1)
                    {
                        std::vector<int> curItem = invData.items[ponReg.pons[o].slots[p]].item->order_id;
                        slots.push_back(curItem);
                    } else
                    {
                        slots.push_back(std::vector<int>(1, -1)); // empty
                    }
                }
                save_json["save"]["army"][i][squad_pos]["slots"] = slots;

                squad_pos++;
            }
        }
        squad_pos = 0;
    }

    std::vector<json> missions; // Doing this like so skips saving an accidental null value in the array
    map<int, int>::iterator it;
    for (it = mission_levels.begin(); it != mission_levels.end(); it++)
    {
        json tmp;
        tmp[0] = it->first;
        tmp[1] = it->second;
        tmp[2] = isMissionUnlocked(it->first);
        missions.push_back(tmp);
    }
    save_json["save"]["missions"] = missions;

    /// make sure to prettify the output json because otherwise it's pain in the ass
    save_file << std::setw(4) << save_json;
    save_file.close();

    spdlog::info("Done saving.");
}

bool SaveReader::isMissionUnlocked(int mission)
{
    if (std::find(missions_unlocked.begin(), missions_unlocked.end(), mission) != missions_unlocked.end())
    {
        return true;
    } else
    {
        return false;
    }
}

bool SaveReader::isLocationUnlocked(int location)
{
    return std::find(locations_unlocked.begin(), locations_unlocked.end(), location) != locations_unlocked.end();
}
