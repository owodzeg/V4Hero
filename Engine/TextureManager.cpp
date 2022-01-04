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
    // first texture initialization should be used with quality setting.
    // if you are using this function, that means the texture is already loaded
    // if it's not, this code will crash

    return loadedTextures[path];
}

sf::Texture& TextureManager::getTexture(const std::string& path, int quality)
{
    if (loadedTextures.find(path) != loadedTextures.end())
    {
        SPDLOG_DEBUG("Providing existing texture with path {}", path);
        return loadedTextures[path];
    } else
    {
        if (quality < 3)
        {
            SPDLOG_DEBUG("Quality is {}, texture needs to be downscaled", quality);
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

            SPDLOG_INFO("Loading downscaled texture with path {} and ratio {}", path, ratio);
            return scaleTexture(path, ratio);
        } else
        {
            SPDLOG_INFO("Loading texture with path {}", path);
            loadedTextures[path].loadFromFile(path);
            return loadedTextures[path];
        }
    }
}

sf::Texture& TextureManager::scaleTexture(const std::string& path, int ratio)
{
    if (loadedImages.find(path) != loadedImages.end())
    {
        SPDLOG_DEBUG("Loading source image from {}", path);
        sf::Image source = loadedImages[path];

        int nwidth = ceil(source.getSize().x / float(ratio));
        int nheight = ceil(source.getSize().y / float(ratio));

        sf::Image dest;
        dest.create(nwidth, nheight);

        for (int x=0; x<nwidth; x++)
        {
            for (int y=0; y<nheight; y++)
            {
                int r=0, g=0, b=0, a=0, final_ratio=0;

                for (int p1 = 0; p1 < ratio; p1++)
                {
                    for (int p2 = 0; p2 < ratio; p2++)
                    {
                        if (x * ratio + p1 < source.getSize().x && y * ratio + p2 < source.getSize().y)
                        {
                            sf::Color p = source.getPixel(x * ratio + p1, y * ratio + p2);
                            r += p.r;
                            g += p.g;
                            b += p.b;
                            a += p.a;

                            final_ratio++;
                        }
                    }
                }

                if (final_ratio < 1)
                    final_ratio = 1; //prevent division by zero

                final_ratio = ratio * ratio;

                r = r / final_ratio;
                g = g / final_ratio;
                b = b / final_ratio;
                a = a / final_ratio;

                dest.setPixel(x, y, sf::Color(r, g, b, a));
            }
        }

        loadedImages[path] = dest;

        SPDLOG_DEBUG("Image {} has been downscaled.", path);
        loadTextureFromImage(path);

        //after loading downscaled image to texture, we no longer need to keep it in memory, so we clean it
        unloadImage(path);

        SPDLOG_DEBUG("Providing downscaled texture with path {}", path);
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

void TextureManager::loadImageFromFile(const std::string& path)
{
    if (loadedImages.find(path) == loadedImages.end())
    {
        SPDLOG_DEBUG("Loading image from file {}", path);
        loadedImages[path].loadFromFile(path);
    } else
    {
        //SPDLOG_ERROR("Couldn't load image {}: image already loaded", key);
        //in theory this shouldnt be an error
    }
}

void TextureManager::loadImageFromMemory(const std::string& key, sf::Image image)
{
    if (loadedImages.find(key) == loadedImages.end())
    {
        SPDLOG_DEBUG("Loading image from memory with key {}", key);
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
        SPDLOG_DEBUG("Loading image {} into texture", img_key);
        loadedTextures[img_key].loadFromImage(loadedImages[img_key]);
    }
}

void TextureManager::unloadImage(const std::string& key)
{
    loadedImages.erase(key);
}