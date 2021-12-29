#include "TipsUtil.h"
#include "Config.h"
#include "Func.h"
#include <algorithm>
#include <iostream>
#include <string>

TipsUtil::TipsUtil()
{
    std::ifstream check("resources/graphics/ui/tips/tip_backgrounds.txt");
    bool exists = check.good();
    check.close();
}

void TipsUtil::LoadBackgrounds(ResourceManager& resourceManager)
{
    std::ifstream conf("resources/graphics/ui/tips/tip_backgrounds.txt");
    if (conf.good())
    {
        std::string line;
        while (getline(conf, line))
        {
            ///ignore comments and empty lines
            if (line.find("#") == std::string::npos && line.find("//") == std::string::npos && line.length() > 0)
            {
                ///Split the Key and Value
                if (line.back() == '\r')
                {
                    line.pop_back();
                }
                backgroundFileNames.push_back(line);
            }
        }
    } else
    {
        SPDLOG_ERROR("Could not load tips background file!");
    }
    conf.close();

    for (auto it = backgroundFileNames.begin(); it != backgroundFileNames.end(); ++it)
    {
        resourceManager.loadSprite("resources/graphics/ui/tips/" + *it);
    }
}

void TipsUtil::LoadIcons(ResourceManager& resourceManager)
{
    std::ifstream conf("resources/graphics/ui/tips/tip_icons.txt");
    if (conf.good())
    {
        std::string line;
        while (getline(conf, line))
        {
            ///ignore comments and empty lines
            if (line.find("#") == std::string::npos && line.find("//") == std::string::npos && line.length() > 0)
            {
                if (line.back() == '\r')
                {
                    line.pop_back();
                }
                ///Split the Key and Value
                iconFileNames.push_back(line);
            }
        }
    } else
    {
        SPDLOG_ERROR("Could not load tips icon file!");
    }

    conf.close();

    for (auto it = iconFileNames.begin(); it != iconFileNames.end(); ++it)
    {
        resourceManager.loadSprite("resources/graphics/ui/tips/" + *it);
    }
}