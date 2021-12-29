#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "ResourceManager.h"
#include "V4Core.h"

ResourceManager::ResourceManager()
{
    
}

void ResourceManager::getQuality(V4Core* core)
{
    quality = static_cast<Quality>(core->config.GetInt("textureQuality"));
}

void ResourceManager::loadSprite(std::string path)
{
    loadedSprites[path].loadFromFile(path, quality);
    SPDLOG_INFO("Loaded sprite with path {}", path);
}

PSprite& ResourceManager::getSprite(const std::string& path)
{
    if (loadedSprites.find(path) != loadedSprites.end())
    {
        return loadedSprites[path];
    } else
    {
        SPDLOG_ERROR("ResourceManager failed to get sprite of key: {}", path);
        PSprite toReturn;
        return toReturn; // Return empty sprite to safely perform useless operations on
        // later replace it with a dedicated "error" texture for more clearance on where the texture broke
    }
}

void ResourceManager::unloadSprite(const std::string& path)
{

}

std::string ResourceManager::qualityToString()
{
    switch (quality)
    {
        case 0: {
            return "L";
            break;
        }
        case 1: {
            return "M";
            break;
        }
        case 2: {
            return "H";
            break;
        }
        case 3: {
            return "U";
            break;
        }
    }
}