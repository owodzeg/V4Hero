#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "ResourceManager.h"
#include "CoreManager.h"
#include "StateManager.h"
#include "V4Core.h"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

ResourceManager& ResourceManager::getInstance()
{
    static ResourceManager instance;
    return instance;
}

void ResourceManager::getQuality()
{
    quality = static_cast<Quality>(CoreManager::getInstance().getConfig()->GetInt("textureQuality"));
}

int ResourceManager::getCurrentQuality()
{
    return quality;
}

void ResourceManager::loadSprite(std::string path)
{
    loadedSprites[path].loadFromFile(path, CoreManager::getInstance().getConfig()->GetInt("textureQuality"));
    loadedSources[path] = StateManager::getInstance().getState();

    loadedPaths[StateManager::getInstance().getState()].push_back(path);

    SPDLOG_INFO("Loaded sprite with path {}", path);
    ///have to add handling for when texture doesn't exist
}

PSprite& ResourceManager::getSprite(const std::string& path)
{
    if (loadedSprites.find(path) != loadedSprites.end())
    {
        SPDLOG_TRACE("Provided sprite with path {}", path);
        return loadedSprites[path];
    } else
    {
        /* SPDLOG_ERROR("ResourceManager failed to get sprite of key: {}", path);
        PSprite toReturn;
        return toReturn; // Return empty sprite to safely perform useless operations on
        // later replace it with a dedicated "error" texture for more clearance on where the texture broke*/
        
        // if the texture is not loaded yet, load it
        SPDLOG_TRACE("Sprite with path {} not loaded yet.", path);
        loadSprite(path);
        // and then try running the function again
        return getSprite(path);

        // it's especially helpful when you want to automatically assign the loaded sprite without separately using loadSprite and getSprite.
        // this mechanism is not for use in loops. always preload the assets
    }
}

void ResourceManager::unloadSprite(const std::string& path)
{
    loadedSprites.erase(path);

    int source = loadedSources[path];
    loadedSources.erase(path);
}

void ResourceManager::unloadState(int state)
{
    for(auto a : loadedPaths[state])
    {
        unloadSprite(a);
        TextureManager::getInstance().unloadImage(a);
        TextureManager::getInstance().unloadTexture(a);
    }

    loadedPaths[state].clear();
}

// this action reloads all currently loaded PSprites with actual quality setting.
void ResourceManager::reloadPSprites()
{
    TextureManager::getInstance().reloadTextures(quality);

    for (const auto& sprite : loadedSprites)
    {
        std::string path = sprite.first;
        //loadSprite(path);
        loadedSprites[path].loadFromFile(path, quality);
    }
}