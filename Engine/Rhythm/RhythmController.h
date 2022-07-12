#ifndef RHYTHMCONTROLLER_H
#define RHYTHMCONTROLLER_H

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <deque>
#include <map>
#include <string>
#include <vector>

#include "../Config.h"
#include "../Input/InputController.h"

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

    bool hit = false;

    int combo = 0;

    ///REPLACE WITH b_drum["pata"][0] and b_drum["ch_pata"][0];
    sf::SoundBuffer b_pata[3], b_pon[3], b_chaka[3], b_don[3];         ///Drums without chants
    sf::SoundBuffer b_chpata[3], b_chpon[3], b_chchaka[3], b_chdon[3]; ///Drums with chants
    std::deque<sf::Sound> s_drums;                                     ///Table for storing all the drums being hit

    sf::Sound drum_c;
    sf::Sound drum_nc;

    sf::SoundBuffer b_perfect;
    sf::Sound s_perfect;

    map<int, bool> keyMap;
    Config* config;

    map<string, int> patterns;
    bool drumAlreadyHit = false;

    ///For output, reset every frame
    string drumToLoad = "";
    int drum_perfection = 0;
    int currentPattern = 0;
    bool breakCombo = false;

    std::vector<std::string> commandInput; ///Drum input
    std::string current_drum;

    int perfect = 0;                             ///value used for calculating perfect drums in a command
    std::vector<int> command_perfects;           ///vector used for storing perfection of each drum
    std::vector<int> perfects = {4, 4, 4, 4, 4}; ///vector of perfections of inputted commands (with default data)
    std::vector<string> av_commands;

    RhythmController();
    bool checkForInput();
    void resetValues();
private:
    std::shared_ptr<spdlog::logger> logger = spdlog::get("rhythm");
};

#endif // RHYTHMCONTROLLER_H
