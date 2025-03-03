#ifndef SAVEREADER_H
#define SAVEREADER_H
#include "Item/InventoryData.h"
#include "Item/ItemRegistry.h"
#include "Pon/PonRegistry.h"
#include "Pon/SquadRegistry.h"
#include "SaveFile.h"
#include "StringRepository.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class Config;

class SaveReader
{
public:
    ItemRegistry itemReg;
    SquadRegistry squadReg;
    PonRegistry ponReg;
    InventoryData invData;

    SaveData save_file_data;

    sf::String kami_name;
    bool hero_unlocked = false;
    int story_point = 0;

    std::vector<int> missions_unlocked = {1};
    std::vector<int> locations_unlocked = {1};
    std::map<int, int> mission_levels; // Storing the mission level

    std::string save_ver = "2.0";

    SaveReader();
    ~SaveReader();
    void LoadSave(Config& tconfig);
    void LoadSave();
    void Flush();
    void CreateBlankSave();
    bool isNewSave;
    void Save();
    bool isMissionUnlocked(int mission);
    bool isLocationUnlocked(int location);
};

#endif // SAVEREADER_H
