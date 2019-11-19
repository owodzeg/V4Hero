#ifndef SAVEREADER_H
#define SAVEREADER_H
#include <SFML/Graphics.hpp>
#include "StringRepository.h"
#include <vector>
#include <map>
#include <string>
#include "DebugOut.h"
class SaveReader
{
    public:
    SaveReader();
    void LoadSave(Config& tconfig);
    DebugOut* debugOut;
    Config* config;
    bool isNewSave;
    void Save();
};

#endif // SAVEREADER_H
