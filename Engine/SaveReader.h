#ifndef SAVEREADER_H
#define SAVEREADER_H
#include <SFML/Graphics.hpp>
#include "StringRepository.h"
#include <vector>
#include <map>
#include <string>
#include "DebugOut.h"
#include "Item/ItemRegistry.h"
#include "Item/InventoryData.h"
class SaveReader
{
    public:
    ItemRegistry itemreg;
    InventoryData invdata;

    sf::String kaminame;
    int timeslaunched;
    int yariponsUnlocked;
    int heroUnlocked;


    SaveReader();
    void LoadSave(Config& tconfig);
    DebugOut* debugOut;
    Config* config;
    bool isNewSave;
    void Save();
};

#endif // SAVEREADER_H
