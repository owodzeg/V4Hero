#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "CoreManager.h"
#include "SoundManager.h"
#include <future>
#include <thread>
#include <vector>

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
    SPDLOG_DEBUG("SoundManager Destructor.");
}

SoundManager& SoundManager::getInstance()
{
    static SoundManager instance;
    return instance;
}

sf::SoundBuffer& SoundManager::getBuffer(const std::string& path)
{
    return loadedBuffers[path];
}

bool SoundManager::checkBufferExists(const std::string& key)
{
    auto it = loadedBuffers.find(key);
    if (it != loadedBuffers.end())
    {
        return true;
    }
    return false;
}

void SoundManager::loadBufferFromFile(const std::string& path)
{
    if (!checkBufferExists(path))
    {
        SPDLOG_DEBUG("Loading sound buffer from file: {}", path);
        if (!std::filesystem::exists(path))
        {
            SPDLOG_ERROR("Sound file not found: {}", path);
            throw SoundManagerException("Sound file not found: " + path);
        }
        std::lock_guard<std::mutex> lock(resource_mutex);
        if (loadedBuffers.find(path) != loadedBuffers.end())
        {
            SPDLOG_DEBUG("Buffer already loaded, skipping load.");
            return;
        }
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(path))
        {
            SPDLOG_ERROR("Failed to load sound buffer from file: {}", path);
            throw SoundManagerException("Failed to load sound buffer from file: " + path);
        }
        loadedBuffers[path] = buffer;
        SPDLOG_DEBUG("Sound buffer loaded successfully: {}", path);
    }
}

void SoundManager::playSound(const std::string& key, const SoundTag& tag)
{
    if (!checkBufferExists(key))
    {
        loadBufferFromFile(key);
    }

    PataSound ps;
    ps.sound = std::make_unique<sf::Sound>(getBuffer(key));
    ps.tag = tag;
    ps.key = key;
    ps.volume = 100;
    ps.sound->setVolume(ps.volume);
    ps.sound->play();
    activeSounds.push_back(std::move(ps));
}

void SoundManager::playSoundFromBuffer(const sf::SoundBuffer& buffer, const std::string& key, const SoundTag& tag)
{
    PataSound ps;
    ps.sound = std::make_unique<sf::Sound>(buffer);
    ps.tag = tag;
    ps.key = key;
    ps.volume = 100;
    ps.sound->setVolume(ps.volume);
    ps.sound->play();
    activeSounds.push_back(std::move(ps));
}

void SoundManager::stopSound(const std::string& key)
{
    for (auto it = activeSounds.begin(); it != activeSounds.end(); ++it)
    {
        if (it->key == key)
        {
            it->sound->stop();
            activeSounds.erase(it);
            break;
        }
    }
}

void SoundManager::stopTaggedSounds(const SoundTag& tag)
{
    for (auto it = activeSounds.begin(); it != activeSounds.end();)
    {
        if (it->tag == tag)
        {
            it->sound->stop();
            it = activeSounds.erase(it);
        } else
        {
            ++it;
        }
    }
}

void SoundManager::stopAllSounds()
{
    for (auto& sound : activeSounds)
    {
        sound.sound->stop();
    }
    activeSounds.clear();
}