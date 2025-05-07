#ifndef SONGCONTROLLER_H
#define SONGCONTROLLER_H

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
    // identifiers for drums
    enum Drums
    {
        PATA = 0,
        PON = 1,
        DON = 2,
        CHAKA = 3,
        NONE = 4
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

    enum SongType
    {
        IDLE = 0,
        PREFEVER_CALM = 1,
        PREFEVER_INTENSE = 2,
        FEVER = 3,
        START = 4,
        PREFEVER_INTENSE_START = 5,
        FEVER_START = 6
    };

    SongController();
    void LoadTheme(std::string theme);
    sf::SoundBuffer& getDrum(Drums, DrumQuality, DrumType);
    sf::SoundBuffer& getSong(SongType);
    float getBPM();
    void flushOrder();

    Drums drumToEnum(std::string drum);
    DrumQuality drumQualityToEnum(std::string drumQuality);
    DrumType drumTypeToEnum(std::string drumType);

    private:
    float BPM = 120.f;

    struct SongCounter
    {
        int current = 0;
        int max = 1;
    };

    std::map<std::string, Drums> drumTable = {{"pata",  Drums::PATA}, 
                                              {"pon",   Drums::PON}, 
                                              {"chaka", Drums::CHAKA}, 
                                              {"don",   Drums::DON}};

    std::map<std::string, DrumQuality> drumQualityTable = {{"best", DrumQuality::BEST}, 
                                                           {"good", DrumQuality::GOOD}, 
                                                           {"bad",  DrumQuality::BAD}};

    std::map<std::string, DrumType> drumTypeTable = {{"drum",  DrumType::DRUM}, 
                                                    {"voice", DrumType::VOICE}};

    // single-time sounds
    sf::SoundBuffer sb_start; // start theme
    sf::SoundBuffer sb_prefever_intense_start; // start the intense part of prefever
    sf::SoundBuffer sb_fever_start; // start the fever

    // sb_idle_loop[0..x] etc
    std::vector<sf::SoundBuffer> sb_idle_loop; // bgm loop when idling (no combo)
    std::vector<sf::SoundBuffer> sb_prefever_calm_loop; // bgm loop when prefever is calm (bad inputs)
    std::vector<sf::SoundBuffer> sb_prefever_intense_loop; // bgm loop when prefever is intense (good inputs)
    std::vector<sf::SoundBuffer> sb_fever_loop; // bgm loop during fever

    SongCounter sc_idle_loop;
    SongCounter sc_prefever_calm_loop;
    SongCounter sc_prefever_intense_loop;
    SongCounter sc_fever_loop;

    // sb_chant_fever["march"][0..x] <- for each command from commands.json
    std::map<std::string, std::vector<sf::SoundBuffer>> sb_chant_prefever_calm; // chants for calm prefever section
    std::map<std::string, std::vector<sf::SoundBuffer>> sb_chant_prefever_intense; // chants for intense prefever section
    std::map<std::string, std::vector<sf::SoundBuffer>> sb_chant_fever; // chants for fever section

    std::map<std::string, SongCounter> sc_chant_prefever_calm;
    std::map<std::string, SongCounter> sc_chant_prefever_intense;
    std::map<std::string, SongCounter> sc_chant_fever;

    //         PATA     BEST       VOICE
    // sb_drum[drum][drumQuality][DrumType] = sf::SoundBuffer
    // these drums can be supplied from drums.json file or defaults will be loaded
    std::map<Drums, std::map<DrumQuality, std::map<DrumType, sf::SoundBuffer>>> sb_drum;
};

#endif // SONGCONTROLLER_H
