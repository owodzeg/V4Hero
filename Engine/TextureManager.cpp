#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "TextureManager.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

TextureManager& TextureManager::getInstance()
{
    static TextureManager instance;
    return instance;
}

sf::Texture& TextureManager::getTexture(const std::string& path)
{
    if (loadedTextures.find(path) != loadedTextures.end())
    {
        SPDLOG_TRACE("Providing texture with path {}", path);
        return loadedTextures[path];
    } else
    {
        SPDLOG_INFO("Loading texture with path {}", path);
        loadedTextures[path].loadFromFile(path);
        return loadedTextures[path];
    }
}

bool TextureManager::checkImageExists(const std::string& key)
{
    if (loadedImages.find(key) == loadedImages.end())
    {
        return false;
    } else
    {
        return true;
    }
}

void TextureManager::loadImage(const std::string& key, sf::Image image)
{
    if (loadedImages.find(key) == loadedImages.end())
    {
        loadedImages[key] = image;
    } else
    {
        //SPDLOG_ERROR("Couldn't load image {}: image already loaded", key);
        //in theory this shouldnt be an error 
    }
}

sf::Image& TextureManager::getImage(const std::string& key)
{
    if (loadedImages.find(key) != loadedImages.end())
    {
        SPDLOG_TRACE("Providing image with key {}", key);
        return loadedImages[key];
    } else
    {
        SPDLOG_ERROR("Couldn't load image {}: image doesn't exist", key);
        sf::Image image; //create empty image to prevent crashes
        return image;
    }
}

void TextureManager::loadTextureFromImage(const std::string& img_key)
{
    if (loadedImages.find(img_key) == loadedImages.end())
    {
        SPDLOG_WARN("Couldn't find image {}, texture may be missing", img_key);
    }

    if (loadedTextures.find(img_key) == loadedTextures.end())
    {
        loadedTextures[img_key].loadFromImage(loadedImages[img_key]);
    }
}