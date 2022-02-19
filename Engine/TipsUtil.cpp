#include "TipsUtil.h"
#include "Config.h"
#include "Func.h"
#include <algorithm>
#include <iostream>
#include <string>

TipsUtil::TipsUtil()
{
    // Load background filenames
    std::ifstream bgFile("resources/graphics/ui/tips/tip_backgrounds.txt");

    if (bgFile.good())
    {
        // thanks to https://stackoverflow.com/a/8365247
        std::copy(std::istream_iterator<std::string>(bgFile),
                  std::istream_iterator<std::string>(),
                  std::back_inserter(backgroundFileNames));

        SPDLOG_INFO("Tip backgrounds loaded: {} backgrounds detected", backgroundFileNames.size());
    } else
    {
        SPDLOG_ERROR("Could not load tips background file!");
    }

    bgFile.close();

    // Load icon filenames
    std::ifstream iconFile("resources/graphics/ui/tips/tip_icons.txt");
    
    if (iconFile.good())
    {
        std::copy(std::istream_iterator<std::string>(iconFile),
                  std::istream_iterator<std::string>(),
                  std::back_inserter(iconFileNames));

        SPDLOG_INFO("Tip icons loaded: {} icons detected", iconFileNames.size());
    } else
    {
        SPDLOG_ERROR("Could not load tips icon file!");
    }

    iconFile.close();
}

void TipsUtil::LoadBackgrounds()
{
    // dont load all icons, too much weight
    /*
    for (auto it = backgroundFileNames.begin(); it != backgroundFileNames.end(); ++it)
    {
        ResourceManager::getInstance().loadSprite("resources/graphics/ui/tips/" + *it);
    }
    */
}

void TipsUtil::LoadIcons()
{
    // dont load all icons, too much weight
    /* 
    for (auto it = iconFileNames.begin(); it != iconFileNames.end(); ++it)
    {
        ResourceManager::getInstance().loadSprite("resources/graphics/ui/tips/" + *it);
    } 
    */
}