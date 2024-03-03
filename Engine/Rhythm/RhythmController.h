#ifndef RHYTHMCONTROLLER_H
#define RHYTHMCONTROLLER_H

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <deque>
#include <map>
#include <string>
#include <vector>

#include "../Config.h"
#include "../Input/InputController.h"
#include "SongController.h"

#include <SFML/Audio.hpp>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>

using namespace std;

class RhythmController
{
public:
    int low_range;
    int high_range;
    float masterTimer = 0;
    float masterTimerNoAbs = 0;
    bool belowHitRange = false;
    bool canHit = true;

    bool hit = false;

    int combo = 0;

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

    std::vector<std::string> drum_pngs = {"pata", "pon", "don", "chaka"};

    // keybinds for drums
    std::vector<int> keybinds = {Input::Keys::SQUARE, 
                                Input::Keys::CIRCLE, 
                                Input::Keys::CROSS, 
                                Input::Keys::TRIANGLE};
    

    // store soundbuffers for chant and nonchant drum hits
    std::map<int, std::map<int, sf::SoundBuffer>> b_drum;
    std::map<int, std::map<int, sf::SoundBuffer>> b_drumchant;

    // Table for storing all the drums being hit
    std::deque<sf::Sound> s_drums;                                     

    // sound player for given chant
    sf::Sound drum_c;
    sf::Sound drum_nc;

    // buffer and sound player for perfect measure noise
    sf::SoundBuffer b_perfect;
    sf::Sound s_perfect;

    map<string, int> patterns;
    bool drumAlreadyHit = false;

    ///For output, reset every frame
    string drumToLoad = "";
    int drum_perfection = 0;
    int currentPattern = 0;
    bool breakCombo = false;

    std::vector<int> commandInput; ///Drum input
    std::string current_drum;

    int perfect = 0;                             ///value used for calculating perfect drums in a command
    std::vector<int> command_perfects;           ///vector used for storing perfection of each drum
    std::vector<int> perfects = {4, 4, 4, 4, 4}; ///vector of perfections of inputted commands (with default data)
    std::vector<string> av_commands;
    std::vector<int> base5_commands;
    std::vector<int> rl_input_commands; // amount of real inputs in a command

    std::vector<int> rl_input_perfects;
    int rl_input_perfect = 0;
    float rl_input_perfection = 0;

    float perfection = 0;
    std::vector<int> commandInputProcessed;
    bool commandWithMissingHalfBeat = false;

    RhythmController();
    bool checkForInput();
    void resetDrums();
    void resetCombo();
private:
    std::shared_ptr<spdlog::logger> logger = spdlog::get("rhythm");
};

#endif // RHYTHMCONTROLLER_H
