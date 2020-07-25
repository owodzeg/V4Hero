#ifndef SAVEREADER_H
#define SAVEREADER_H
#include <SFML/Graphics.hpp>
#include "StringRepository.h"
#include <vector>
#include <map>
#include <string>
#include "DebugOut.h"
#include "Item/ItemRegistry.h"
#include "Pon/PonRegistry.h"
#include "Item/InventoryData.h"
class SaveReader
{
    public:
    ItemRegistry itemreg;
    PonRegistry ponreg;
    InventoryData invdata;

    sf::String kaminame;
    int timeslaunched;
    int yariponsUnlocked;
    int heroUnlocked;

    std::vector<int> missionsUnlocked = {1};
    int locationsUnlocked=1;

    SaveReader();
    void LoadSave(Config& tconfig);
    DebugOut* debugOut;
    Config* config;
    bool isNewSave;
    void Save();
    bool isMissionUnlocked(int mission);
};

#endif // SAVEREADER_H
