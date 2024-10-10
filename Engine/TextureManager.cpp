#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "TextureManager.h"
#include "CoreManager.h"
#include <vector>
#include <thread>
#include <future>

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
    if(loadedImages.find(path) != loadedImages.end() && ratio == 1)
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

        int nwidth = ceil(originalWidth / float(ratio));
        int nheight = ceil(originalHeight / float(ratio));

        sf::Image dest;
        dest.create(nwidth, nheight);

        // Step 1: Build the integral image for faster block summation
        std::vector<std::vector<int>> integralR(originalWidth, std::vector<int>(originalHeight, 0));
        std::vector<std::vector<int>> integralG(originalWidth, std::vector<int>(originalHeight, 0));
        std::vector<std::vector<int>> integralB(originalWidth, std::vector<int>(originalHeight, 0));
        std::vector<std::vector<int>> integralA(originalWidth, std::vector<int>(originalHeight, 0));

        // Precompute the integral image (summing up color channels separately)
        for (int x = 0; x < originalWidth; ++x)
        {
            for (int y = 0; y < originalHeight; ++y)
            {
                sf::Color pixel = source.getPixel(x, y);

                integralR[x][y] = pixel.r + ((x > 0) ? integralR[x-1][y] : 0) + ((y > 0) ? integralR[x][y-1] : 0) - ((x > 0 && y > 0) ? integralR[x-1][y-1] : 0);
                integralG[x][y] = pixel.g + ((x > 0) ? integralG[x-1][y] : 0) + ((y > 0) ? integralG[x][y-1] : 0) - ((x > 0 && y > 0) ? integralG[x-1][y-1] : 0);
                integralB[x][y] = pixel.b + ((x > 0) ? integralB[x-1][y] : 0) + ((y > 0) ? integralB[x][y-1] : 0) - ((x > 0 && y > 0) ? integralB[x-1][y-1] : 0);
                integralA[x][y] = pixel.a + ((x > 0) ? integralA[x-1][y] : 0) + ((y > 0) ? integralA[x][y-1] : 0) - ((x > 0 && y > 0) ? integralA[x-1][y-1] : 0);
            }
        }

        // Step 2: Downsample using the integral image
        auto downscaleBlock = [&](int xStart, int xEnd) {
            for (int x = xStart; x < xEnd; ++x)
            {
                for (int y = 0; y < nheight; ++y)
                {
                    int srcX1 = x * ratio;
                    int srcY1 = y * ratio;
                    int srcX2 = std::min((x + 1) * ratio - 1, originalWidth - 1);
                    int srcY2 = std::min((y + 1) * ratio - 1, originalHeight - 1);

                    int area = (srcX2 - srcX1 + 1) * (srcY2 - srcY1 + 1);

                    int r = integralR[srcX2][srcY2]
                            - ((srcX1 > 0) ? integralR[srcX1-1][srcY2] : 0)
                            - ((srcY1 > 0) ? integralR[srcX2][srcY1-1] : 0)
                            + ((srcX1 > 0 && srcY1 > 0) ? integralR[srcX1-1][srcY1-1] : 0);

                    int g = integralG[srcX2][srcY2]
                            - ((srcX1 > 0) ? integralG[srcX1-1][srcY2] : 0)
                            - ((srcY1 > 0) ? integralG[srcX2][srcY1-1] : 0)
                            + ((srcX1 > 0 && srcY1 > 0) ? integralG[srcX1-1][srcY1-1] : 0);

                    int b = integralB[srcX2][srcY2]
                            - ((srcX1 > 0) ? integralB[srcX1-1][srcY2] : 0)
                            - ((srcY1 > 0) ? integralB[srcX2][srcY1-1] : 0)
                            + ((srcX1 > 0 && srcY1 > 0) ? integralB[srcX1-1][srcY1-1] : 0);

                    int a = integralA[srcX2][srcY2]
                            - ((srcX1 > 0) ? integralA[srcX1-1][srcY2] : 0)
                            - ((srcY1 > 0) ? integralA[srcX2][srcY1-1] : 0)
                            + ((srcX1 > 0 && srcY1 > 0) ? integralA[srcX1-1][srcY1-1] : 0);

                    dest.setPixel(x, y, sf::Color(r / area, g / area, b / area, a / area));
                }
            }
        };

        // Step 3: Multithreading for parallel downscaling
        unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::future<void>> futures;

        int chunkSize = nwidth / numThreads;
        for (unsigned int i = 0; i < numThreads; ++i)
        {
            int xStart = i * chunkSize;
            int xEnd = (i == numThreads - 1) ? nwidth : (i + 1) * chunkSize;

            futures.push_back(std::async(std::launch::async, downscaleBlock, xStart, xEnd));
        }

        for (auto& future : futures) {
            future.get(); // Wait for all threads to finish
        }

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
    //SPDLOG_DEBUG("load image {} from memory. asTexture: {}", key, asTexture);

    if (!asTexture)
    {
        if (loadedImages.find(key) == loadedImages.end())
        {
            //SPDLOG_INFO("Loading image from memory with key {}", key);
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
            //SPDLOG_INFO("Loading image from memory into texture with key {}", key);
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