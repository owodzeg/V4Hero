#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <spdlog/spdlog.h>
#include <unordered_map>
#include <deque>

class SoundManagerException : public std::exception
{
private:
    std::string message;

public:
    SoundManagerException(std::string msg)
        : message(msg)
    {
    }
    char* what()
    {
        return message.data();
    }
};

class SoundManager
{
public:
    enum SoundTag
    {
        NONE = 0,
        INTERFACE = 1,
        MENU_BGM = 2,
        EFFECT_SFX = 3,
        ENTITY_SFX = 4,
        ENEMY_DMG_SFX = 5,
        PLAYER_DMG_SFX = 6,
        RHYTHM_DRUM = 7,
        RHYTHM_DRUM_CHANT = 8,
        RHYTHM_METRONOME = 9,
        RHYTHM_CHANT = 10,
        RHYTHM_BGM = 11,
        OTHER = 12
    };

    struct PataSound {
        std::unique_ptr<sf::Sound> sound;
        std::string key = "";
        SoundTag tag;
        double volume = 100;
    };

    std::mutex resource_mutex;

    static SoundManager& getInstance();
    sf::SoundBuffer& getBuffer(const std::string& path);
    bool checkBufferExists(const std::string& key);
    void loadBufferFromFile(const std::string& path);
    void playSound(const std::string& key, const SoundTag& tag = SoundTag::NONE);
    void playSoundFromBuffer(const sf::SoundBuffer& buffer, const std::string& key, const SoundTag& tag = SoundTag::NONE);
    void stopSound(const std::string& key);
    void stopTaggedSounds(const SoundTag& tag);
    void stopAllSounds();

private:
    SoundManager();
    ~SoundManager();

    std::unordered_map<std::string, sf::SoundBuffer> loadedBuffers;
    std::deque<PataSound> activeSounds;
};

#endif