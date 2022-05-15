#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Graphics/PSprite.h"
#include <filesystem>
#include <unordered_map>
#include <spdlog/spdlog.h>

class V4Core;

// Thanks to https://github.com/ArtainR for implementation
class ResourceManager
{
    public:
        static ResourceManager& getInstance();
        void getQuality();
        int getCurrentQuality();
        void loadSprite(std::string path);
        PSprite& getSprite(const std::string& path);
        void unloadSprite(const std::string& path);
        void unloadState(int state);
        void reloadPSprites();

	private:
        ResourceManager();
        ~ResourceManager();

        std::unordered_map<std::string, PSprite> loadedSprites; //holds PSprites
        std::unordered_map<std::string, int> loadedSources; //holds source state

        std::unordered_map<int, std::vector<std::string>> loadedPaths; //holds list of paths for each source

        enum Quality
        {
            Low = 0,
            Medium = 1,
            High = 2,
            Ultra = 3
        };

        Quality quality;
};

#endif