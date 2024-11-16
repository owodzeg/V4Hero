#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "SaveReader.h"
#include "Config.h"
#include "Func.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
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

    isNewSave = !exists;
}

void SaveReader::LoadSave()
{
    ifstream conf("resources/data/sv1.p4sv", std::ios::in);
    if (conf.good())
    {
        conf >> save_data;
        SaveFile save_file = save_data.get<SaveFile>();

        save_file_data = save_file.save;
        save_data = save_data["save"];

        const SaveFileDetails& details = save_file_data.details;
        save_ver = details.version;
        kami_name = sf::String(details.name);
        story_point = details.story_point;
        locations_unlocked.clear(); // Clear the default from SaveReader.h
        for (unsigned int i = 0; i < details.locations_unlocked.size(); i++)
        {
            locations_unlocked.push_back(details.locations_unlocked[i]);
        }

        for (unsigned int i = 0; i < save_file_data.items.size(); i++)
        {
            invData.addItem(itemReg.getItemByName(save_file_data.items[i].name)->order_id, itemReg, save_file_data.items[i].count);
        }

        if (save_file_data.army.hero.rarepon != -1) // Is hero unlocked?
        {
            hero_unlocked = true;
            Pon new_pon;
            new_pon.pon_id = save_file_data.army.hero.rarepon;
            new_pon.pon_class = save_file_data.army.hero._class;
            new_pon.pon_level = save_file_data.army.hero.level;
            new_pon.pon_exp = save_file_data.army.hero.exp;
            for (unsigned int o = 0; o < save_file_data.army.hero.slots.size(); o++)
            {
                if (save_file_data.army.hero.slots[o][0] != -1) // Is this necessary?
                {
                    new_pon.giveItem(invData.getInvIDByItemID(save_file_data.army.hero.slots[o]), static_cast<int>(o));
                }
            }
            ponReg.pons.push_back(new_pon);
        }

        unsigned int comparison = std::min(save_file_data.army.squads.size(), static_cast<size_t>(4));

        for (int i = 0; i < comparison; i++)
        {
            for (unsigned int o = 0; o < save_file_data.army.squads[i].size(); o++)
            {
                Pon new_pon;
                new_pon.pon_id = save_file_data.army.squads[i][o].rarepon;
                new_pon.pon_class = save_file_data.army.squads[i][o]._class;
                new_pon.pon_level = save_file_data.army.squads[i][o].level;
                new_pon.pon_exp = save_file_data.army.squads[i][o].exp;
                for (unsigned int p = 0; p < save_file_data.army.squads[i][o].slots.size(); p++)
                {
                    if (save_file_data.army.squads[i][o].slots[p][0] != -1) // Is this necessary?
                    {
                        new_pon.giveItem(invData.getInvIDByItemID(save_file_data.army.squads[i][o].slots[p]), p);
                    }
                }
                ponReg.pons.push_back(new_pon);
            }

            ponReg.squads_available.push_back(save_file_data.army.squads[i][0]._class); // Save to available squads for choosing in barracks / prep
            ponReg.army.push_back(save_file_data.army.squads[i][0]._class);             // The first 3 squads are in army, to change the limit of squads in army change the `std::min(..., 4)` above and `i = 4` below
        }

        for (unsigned int i = 4; i < save_file_data.army.squads.size(); i++)
        {
            for (unsigned int o = 0; o < save_file_data.army.squads[i].size(); o++)
            {
                Pon new_pon;
                new_pon.pon_id = save_file_data.army.squads[i][o].rarepon;
                new_pon.pon_class = save_file_data.army.squads[i][o]._class;
                new_pon.pon_level = save_file_data.army.squads[i][o].level;
                new_pon.pon_exp = save_file_data.army.squads[i][o].exp;
                for (unsigned int p = 0; p < save_file_data.army.squads[i][o].slots.size(); p++)
                {
                    if (save_file_data.army.squads[i][o].slots[0][0] != -1) // Is this necessary?
                    {
                        new_pon.giveItem(invData.getInvIDByItemID(save_file_data.army.squads[i][o].slots[p]), p);
                    }
                }
                ponReg.pons.push_back(new_pon);
            }

            ponReg.squads_available.push_back(save_file_data.army.squads[i][0]._class); // Save to available squads for choosing in barracks / prep
        }

        for (unsigned int i = 0; i < save_file_data.missions.size(); i++) // Each mission is stored as a single array so it can be handled elegantly (slightly increases file size, but shouldn't be a problem)
        {
            if (save_file_data.missions[i].unlocked) // Is unlocked?
            {
                missions_unlocked.push_back(save_file_data.missions[i].id); // Add mission id
            }
            mission_levels.insert(pair<int, int>(save_file_data.missions[i].id, save_file_data.missions[i].level)); // Add mission level regardless
        }
    } else
    {
        SPDLOG_ERROR("Could not load save file");
    }
    conf.close();
}

void SaveReader::Flush() ///Empties the save data.
{
    missions_unlocked.clear();
    mission_levels.clear();
    locations_unlocked = {1};
    missions_unlocked = {1};

    //invdata.items.clear();

    PonRegistry n_ponReg;
    InventoryData n_invData;

    ponReg = n_ponReg;
    invData = n_invData;
}

void SaveReader::CreateBlankSave()
{
    SPDLOG_INFO("Creating blank save...");

    kami_name = "Kamipon";

    vector<string> starter_items = {"item_wooden_spear", "item_wooden_spear", "item_wooden_spear", "item_wooden_spear", "item_wooden_spear", "item_wooden_spear", "item_wooden_helmet", "item_wooden_helmet", "item_wooden_helmet", "item_wooden_helmet", "item_wooden_helmet", "item_wooden_helmet"};
    for (unsigned int i = 0; i < starter_items.size(); i++)
    {
        invData.addItem(itemReg.getItemByName(starter_items[i])->order_id, itemReg);
    }

    // Defining 6 Yaripons

    for (int i = 0; i < 6; i++)
    {
        Pon newPon;

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

    SPDLOG_INFO("Finished creating blank save");
}

void SaveReader::Save()
{
    SaveFileDetails details{
        .version = "2.0",
        .name = kami_name,
        .locations_unlocked = locations_unlocked,
        .story_point = story_point
    };

    std::vector<SavedItem> items;
    for (unsigned int i = 0; i < invData.items.size(); i++)
    {
        SavedItem new_item{
            .name = invData.items[i].item->item_name,
            .count = invData.items[i].item_count
        };

        items.push_back(new_item);
    }

    SquadSlot hero{
        .rarepon = -1
    };
    if (hero_unlocked)
    {
        hero.rarepon = ponReg.pons[0].pon_id;
        hero._class = ponReg.pons[0].pon_class;
        hero.level = ponReg.pons[0].pon_id;
        hero.exp = ponReg.pons[0].pon_id;
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
        hero.slots = slots;
    }

    std::vector<std::vector<SquadSlot>> squads(ponReg.squads_available.size());
    for (unsigned int i = 0; i <= ponReg.squads_available.size(); i++)
    {
        for (unsigned int o = 0; o < ponReg.pons.size(); o++)
        {
            if (ponReg.pons[o].pon_class == ponReg.squads_available[i])
            {
                SquadSlot squad_slot{
                    .rarepon = ponReg.pons[o].pon_id,
                    ._class = ponReg.pons[o].pon_class,
                    .level = ponReg.pons[o].pon_level,
                    .exp = ponReg.pons[o].pon_exp,
                    .slots = {}
                };

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
                squad_slot.slots = slots;

                squads[i].push_back(squad_slot);
            }
        }
    }

    Army army{
        .hero = hero,
        .squads = squads,
    };

    std::vector<SavedMission> missions;

    // previous approach with mission levels wasn't going to cut it because we often sit at level 0 and therefore it is not added to the mission_levels map
    // this causes missions to become unavailable after saving because game looks for mission_levels which don't exist
    // thats why we take the furthest unlocked mission and fill in the blanks
    // it might be more weighty on the savedata but it guarantees that all missions id's are properly initialized after saving and loading back
    // and we'll keep an incremental order of these missions anyway...
    // TODO: what to do with potential DLC ids? probably handle separately in some "DLC" savedata clusters (to-be-invented in the future)

    auto it = max_element(std::begin(missions_unlocked), std::end(missions_unlocked));

    for (int i=1; i<=*it; i++)
    {
        SavedMission saved_mission{
            .id = i,
            .level = mission_levels[i],
            .unlocked = isMissionUnlocked(i)
        };
        missions.push_back(saved_mission);
    }

    // old solution

    /*map<int, int>::iterator it;
    for (it = mission_levels.begin(); it != mission_levels.end(); it++)
    {
        json tmp;
        tmp[0] = it->first;
        tmp[1] = it->second;
        tmp[2] = isMissionUnlocked(it->first);
        missions.push_back(tmp);
    }
    */

    ofstream save_file("resources/data/sv1.p4sv", ios::ate);
    save_file.seekp(0); // Honestly no idea what this does but it was in the previous version

    json save_json = SaveFile{
        .save = SaveData{
            .army = army,
            .details = details,
            .items = items,
            .missions = missions,
        }
    };

    /// make sure to prettify the output json because otherwise it's pain in the ass
    save_file << std::setw(4) << save_json;
    save_file.close();

    SPDLOG_INFO("Done saving.");
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
