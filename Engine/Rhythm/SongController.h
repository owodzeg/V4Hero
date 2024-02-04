#ifndef SONGCONTROLLER_H
#define SONGCONTROLLER_H

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <map>
#include <vector>
#include <SFML/Audio.hpp>

class SongControllerException : public std::exception {
    private:
    std::string message;

    public:
    SongControllerException(std::string msg) : message(msg) {}
    char* what () {
        return message.data();
    }
};

class SongController
{
    public:
    SongController();
    void LoadTheme(std::string theme);

    private:

    // identifiers for drums
    enum Drums
    {
        PATA = 0,
        PON = 1,
        DON = 2,
        CHAKA = 3
    };

    enum DrumQuality
    {
        BEST = 0,
        GOOD = 1,
        BAD = 2
    };

    enum DrumType
    {
        DRUM = 0,
        VOICE = 1
    };

    // single-time sounds
    sf::SoundBuffer sb_start; // start theme
    sf::SoundBuffer sb_prefever_intense_start; // start the intense part of prefever
    sf::SoundBuffer sb_fever_start; // start the fever

    // sb_idle_loop[0..x] etc
    std::vector<sf::SoundBuffer> sb_idle_loop; // bgm loop when idling (no combo)
    std::vector<sf::SoundBuffer> sb_prefever_calm_loop; // bgm loop when prefever is calm (bad inputs)
    std::vector<sf::SoundBuffer> sb_prefever_intense_loop; // bgm loop when prefever is intense (good inputs)
    std::vector<sf::SoundBuffer> sb_fever_loop; // bgm loop during fever

    // sb_chant_fever["march"][0..x] <- for each command from commands.json
    std::map<std::string, std::vector<sf::SoundBuffer>> sb_chant_prefever_calm; // chants for calm prefever section
    std::map<std::string, std::vector<sf::SoundBuffer>> sb_chant_prefever_intense; // chants for intense prefever section
    std::map<std::string, std::vector<sf::SoundBuffer>> sb_chant_fever; // chants for fever section

    //         PATA     BEST       VOICE
    // sb_drum[drum][drumQuality][DrumType] = sf::SoundBuffer
    // these drums can be supplied from drums.json file or defaults will be loaded
    std::map<Drums, std::map<DrumQuality, std::map<DrumType, sf::SoundBuffer>>> sb_drum;
};

#endif // SONGCONTROLLER_H
