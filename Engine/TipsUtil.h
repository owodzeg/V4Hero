#ifndef TIPSUTIL_H
#define TIPSUTIL_H
#include "DebugOut.h"
#include "Graphics/PSprite.h"
#include "StringRepository.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>
class TipsUtil
{
public:
    DebugOut* debugOut;
    Config* config;
    std::vector<std::string> backgroundFileNames;
    std::vector<std::string> iconFileNames;
    std::vector<std::string> tipTitles;
    std::vector<std::string> tipTexts;

    std::vector<PSprite> t_backgrounds;
    std::vector<PSprite> t_icons;

    int tip_amount = 7;

    TipsUtil();
    void LoadBackgrounds(Config& tconfig);
    void LoadIcons(Config& tconfig);
    void LoadStrings(Config& tconfig);
    void Save();
};

#endif // TIPSUTIL_H
