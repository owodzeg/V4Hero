#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "TextureManager.h"
#include "CoreManager.h"

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

void TextureManager::loadTexture(const std::string& path, int quality)
{
    loadedTextures[path].loadFromFile(path);

    if (quality < 3)
    {
        SPDLOG_TRACE("Quality is {}, texture needs to be downscaled", quality);
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
                SPDLOG_TRACE("Quality is {}, texture needs to be downscaled", quality);
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
        if(unload)
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
        loadedImages[path].loadFromFile(path);
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
        loadedImages[path].loadFromFile(path);

        if (quality < 3)
        {
            SPDLOG_TRACE("Quality is {}, texture needs to be downscaled", quality);

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

void TextureManager::loadImageFromMemory(const std::string& key, sf::Image image, bool asTexture)
{
    std::lock_guard<std::mutex> guard(resource_mutex);
    SPDLOG_DEBUG("load image {} from memory. asTexture: {}", key, asTexture);

    if (!asTexture)
    {
        if (loadedImages.find(key) == loadedImages.end())
        {
            SPDLOG_INFO("Loading image from memory with key {}", key);
            loadedImages[key] = image;
        } else
        {
            SPDLOG_ERROR("Couldn't load image {}: image already loaded", key);
            //in theory this shouldnt be an error
        }
    } else
    {
        if (loadedImages.find(key) == loadedImages.end())
        {
            SPDLOG_INFO("Loading image from memory into texture with key {}", key);
            loadedTextures[key].loadFromImage(image);
        } else
        {
            SPDLOG_ERROR("Couldn't load image {}: image already loaded", key);
            //in theory this shouldnt be an error
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
        loadedTextures[img_key].loadFromImage(loadedImages[img_key]);
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

void TextureManager::reloadTextures(int quality)
{
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