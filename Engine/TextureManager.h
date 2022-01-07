#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <filesystem>
#include <spdlog/spdlog.h>
#include <unordered_map>
#include <SFML/Graphics.hpp>

// Class for holding the texture pointers so we can make sure textures are not being copied around (as this causes all sorts of issues)
// Thanks to https://codereview.stackexchange.com/questions/191793/resource-manager-with-sfml for the code reference

class TextureManager
{
public:
    static TextureManager& getInstance();
    void loadTexture(const std::string& path, int quality);
    sf::Texture& getTexture(const std::string& path);
    sf::Texture& getTexture(const std::string& path, int quality);
    sf::Texture& scaleTexture(const std::string& path, int ratio);
    bool checkImageExists(const std::string& key);
    void loadImageFromFile(const std::string& path);
    void loadImageFromMemory(const std::string& key, sf::Image image);
    sf::Image& getImage(const std::string& key);
    void loadTextureFromImage(const std::string& img_key);
    void unloadImage(const std::string& key);
    void applyForceLoad(bool force);
    void reloadTextures(int quality);

private:
    TextureManager();
    ~TextureManager();
    
    std::unordered_map<std::string, sf::Texture> loadedTextures;
    std::unordered_map<std::string, sf::Image> loadedImages;

    bool forceLoad = false;
};

#endif