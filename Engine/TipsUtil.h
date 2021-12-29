#ifndef TIPSUTIL_H
#define TIPSUTIL_H
#include "Config.h"
#include "Graphics/PSprite.h"
#include "StringRepository.h"
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>
class TipsUtil
{
public:
    Config* config;
    std::vector<std::string> backgroundFileNames;
    std::vector<std::string> iconFileNames;
    std::vector<std::string> tipTitles;
    std::vector<std::string> tipTexts;

    // refers to amount of tips in language file
    int tipAmount = 7;

    TipsUtil();
    void LoadBackgrounds(ResourceManager& resourceManager);
    void LoadIcons(ResourceManager& resourceManager);
};

#endif // TIPSUTIL_H
