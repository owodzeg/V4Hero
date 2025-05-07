#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "TextureManager.h"
#include "CoreManager.h"
#include <vector>
#include <thread>
#include <future>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "External/stb_image_resize2.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
    SPDLOG_DEBUG("TextureManager Destructor.");
}

TextureManager& TextureManager::getInstance()
{
    static TextureManager instance;
    return instance;
}

void TextureManager::loadTexture(const std::string& path, int quality)
{
    if(!loadedTextures[path].loadFromFile(path))
    {
        SPDLOG_ERROR("Couldn't load texture {}", path);
    }

    if (quality < 3)
    {
        SPDLOG_TRACE("Quality is {}, texture needs to be downscaled");
        loadImageFromFile(path);

        int ratio = 1;

        switch (quality)
        {
            case 0: {
                ratio = 6;
                break;
            }

            case 1: {
                ratio = 3;
                break;
            }

            case 2: {
                ratio = 2;
                break;
            }
        }

        SPDLOG_DEBUG("Loading downscaled texture with path {} and ratio {}", path, ratio);

        applyForceLoad(true);
        scaleTexture(path, ratio);
        applyForceLoad(false);
    }
}

sf::Texture& TextureManager::getTexture(const std::string& path)
{
    std::lock_guard<std::mutex> guard(resource_mutex);
    // first texture initialization should be used with quality setting.
    // if you are using this function, that means the texture is already loaded
    // if it's not, this code will crash

    SPDLOG_TRACE("Providing existing texture with path {} [no quality]", path);
    return loadedTextures[path];
}

sf::Texture& TextureManager::getTexture(const std::string& path, int quality, bool downscale)
{
    if (loadedTextures.find(path) != loadedTextures.end() && !forceLoad)
    {
        SPDLOG_TRACE("Providing existing texture with path {}", path);
        return loadedTextures[path];
    } else
    {
        if(downscale)
        {
            if (quality < 3)
            {
                SPDLOG_TRACE("Quality is {}, texture needs to be downscaled");
                loadImageFromFile(path);

                int ratio = 1;

                switch (quality)
                {
                    case 0: {
                        ratio = 6;
                        break;
                    }

                    case 1: {
                        ratio = 3;
                        break;
                    }

                    case 2: {
                        ratio = 2;
                        break;
                    }
                }

                SPDLOG_DEBUG("Loading downscaled texture with path {} and ratio {}", path, ratio);
                return scaleTexture(path, ratio);
            } else
            {
                SPDLOG_INFO("Loading texture with path {}", path);
                loadTexture(path, 3);
                return loadedTextures[path];
            }
        }
        else
        {
            SPDLOG_INFO("Loading texture (no downscale) with path {}", path);
            loadTexture(path, 3);
            return loadedTextures[path];
        }
    }
}

sf::Texture& TextureManager::scaleTexture(const std::string& path, int ratio, bool unload)
{
    if (loadedImages.find(path) != loadedImages.end() && ratio == 1)
    {
        loadTextureFromImage(path);

        if (unload)
            unloadImage(path);

        SPDLOG_DEBUG("Providing downscaled texture with path {}", path);
        return loadedTextures[path];
    }

    if (loadedImages.find(path) != loadedImages.end())
    {
        SPDLOG_DEBUG("Loading source image from {}", path);
        sf::Image& source = loadedImages[path];

        int originalWidth = source.getSize().x;
        int originalHeight = source.getSize().y;

        int nwidth = static_cast<int>(ceil(static_cast<float>(originalWidth) / float(ratio)));
        int nheight = static_cast<int>(ceil(static_cast<float>(originalHeight) / float(ratio)));

        // Get pixel data from source image 
        const uint8_t* sourcePixels = source.getPixelsPtr();

        // Prepare a destination buffer
        std::vector<uint8_t> destPixels(nwidth * nheight * 4); // RGBA = 4 channels

        // Use stb_image_resize to scale the texture
        stbir_resize_uint8_srgb(sourcePixels, originalWidth, originalHeight, 0,
                           destPixels.data(), nwidth, nheight, 0, STBIR_RGBA);

        // Create a new SFML Image from the resized pixels
        sf::Image dest;
        dest.resize(sf::Vector2u(nwidth, nheight), destPixels.data());

        loadedImages[path] = dest;
        loadTextureFromImage(path);

        if (unload)
            unloadImage(path);

        SPDLOG_DEBUG("Providing downscaled texture with path {}", path);
        return loadedTextures[path];
    }

    SPDLOG_ERROR("Could not find appropriate texture to scale.");
    throw TextureManagerException("Could not find appropriate texture to scale.");
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

void TextureManager::loadImageFromFile(const std::string& path)
{
    std::lock_guard<std::mutex> guard(resource_mutex);

    if (loadedImages.find(path) == loadedImages.end() || forceLoad)
    {
        SPDLOG_INFO("Loading image from file {}", path);
        if(!loadedImages[path].loadFromFile(path))
        {
            SPDLOG_ERROR("Couldn't load image {}", path);
        }
    } else
    {
        //SPDLOG_ERROR("Couldn't load image {}: image already loaded", key);
        //in theory this shouldnt be an error
    }
}

void TextureManager::loadImageFromFileWithScale(const std::string& path, int quality)
{
    std::lock_guard<std::mutex> guard(resource_mutex);

    if (loadedImages.find(path) == loadedImages.end() || forceLoad)
    {
        SPDLOG_INFO("Loading image from file {}", path);
        if(!loadedImages[path].loadFromFile(path))
        {
            SPDLOG_ERROR("Couldn't load image {}", path);
        }

        if (quality < 3)
        {
            SPDLOG_TRACE("Quality is {}, texture needs to be downscaled");

            int ratio = 1;

            switch (quality)
            {
                case 0: {
                    ratio = 6;
                    break;
                }

                case 1: {
                    ratio = 3;
                    break;
                }

                case 2: {
                    ratio = 2;
                    break;
                }
            }

            scaleTexture(path, ratio);
        }
    } else
    {
        //SPDLOG_ERROR("Couldn't load image {}: image already loaded", key);
        //in theory this shouldnt be an error
    }
}

void TextureManager::loadImageFromMemory(const std::string& key, sf::Image& image, bool asTexture)
{
    std::lock_guard<std::mutex> guard(resource_mutex);

    if (!asTexture)
    {
        if (loadedImages.find(key) == loadedImages.end())
        {
            loadedImages[key] = std::move(image); // Move the image into the map
        }
        else
        {
            SPDLOG_ERROR("Couldn't load image {}: image already loaded", key);
        }
    }
    else
    {
        if (loadedImages.find(key) == loadedImages.end())
        {
            if(!loadedTextures[key].loadFromImage(image)) // No need to move here since sf::Image is copied internally
            {
                SPDLOG_ERROR("Couldn't load image {}", key);
            }
        }
        else
        {
            SPDLOG_ERROR("Couldn't load image {}: image already loaded", key);
        }
    }
}

sf::Image& TextureManager::getImage(const std::string& key)
{
    auto it = loadedImages.find(key);
    if(it != loadedImages.end())
    {
        SPDLOG_TRACE("Providing image with key {}", key);
        return it->second;
    }

    SPDLOG_ERROR("Couldn't load image {}: image doesn't exist", key);
    throw TextureManagerException(std::format("Couldn't load image {}: image doesn't exist", key));
}

void TextureManager::loadTextureFromImage(const std::string& img_key)
{
    std::lock_guard<std::mutex> guard(resource_mutex);

    if (loadedImages.find(img_key) == loadedImages.end())
    {
        SPDLOG_WARN("Couldn't find image {}, texture may be missing", img_key);
    }

    if (loadedTextures.find(img_key) == loadedTextures.end() || forceLoad)
    {
        SPDLOG_DEBUG("Loading image {} into texture", img_key);
        if (!loadedTextures[img_key].loadFromImage(loadedImages[img_key])) {
            SPDLOG_ERROR("Couldn't load image {} into texture", img_key);
        }
    }
}

void TextureManager::unloadTexture(const std::string& key)
{
    loadedTextures.erase(key);
}

void TextureManager::unloadImage(const std::string& key)
{
    loadedImages.erase(key);
}

void TextureManager::applyForceLoad(bool force)
{
    forceLoad = force;
}

void TextureManager::reloadTextures()
{
    int quality = CoreManager::getInstance().getConfig()->GetInt("textureQuality");

    for (const auto& texture : loadedTextures)
    {
        std::string path = texture.first;
        loadTexture(path, quality);
    }
}

int TextureManager::getRatio()
{
    int quality = CoreManager::getInstance().getConfig()->GetInt("textureQuality");
    int ratio = 1;

    switch (quality)
    {
        case 0: {
            ratio = 6;
            break;
        }

        case 1: {
            ratio = 3;
            break;
        }

        case 2: {
            ratio = 2;
            break;
        }
    }

    return ratio;
}