#ifndef RHYTHM_H
#define RHYTHM_H

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>
#include <chrono>

#include "../Config.h"
#include "../Func.h"

#include "Drum.h"
#include "RhythmController.h"
#include "RhythmGUI.h"
#include "SongController.h"

#include "../Input/InputController.h"

#include <mutex>

class Rhythm
{
public:
    /// Low and high range for BAD, GOOD and BEST hits (in milliseconds, 0 is the center point) ///
    float BPM = 120.f; ///beats per minute
    float beat_timer = 60.f / (BPM*2) * 1000000.f; ///Amount of microseconds for each halfbeat to be made
    float beat_ms = 60.f / BPM * 1000.f; ///Amount of milliseconds for each beat
    float halfbeat_ms = beat_ms / 2.f;
    float measure_ms = beat_ms * 4.f;
    float low_range = beat_timer / 6.25f;  ///Anything below that range will be treated as BAD hit
    float high_range = beat_timer / 3.125f; ///Anything between this and low range will be treated as GOOD hit. Higher will be treated as BEST hit.
    sf::Clock rhythmClock;    ///Main clock for Rhythm purposes
    sf::Clock newRhythmClock;    ///Main clock for Rhythm purposes
    sf::Clock lazyClock;    ///Main clock for Rhythm purposes

    int metronomeVal = 0;
    int metronomeOldVal = 9999;
    int metronomeState = 1;
    bool metronomeClick = false;

private:
    std::shared_ptr<spdlog::logger> logger = spdlog::get("rhythm");
    /// Theme and chant buffers ///
    std::map<std::string, sf::SoundBuffer> b_chant; ///Sound buffer for Patapon chants
    SongController songController;
    std::string currentThemeName;
    /// Check if it's possible to replace cycles with one (max 2) values based on a clock, would make things more reliable
    int cycle = 0;
    int cycle_mode = 0;
    int bgm_cycle = 0;
    bool combobreak = false;
    bool count_cycle = false;

    /// Initialize sounds ///
    sf::SoundBuffer b_fever_fail;
    sf::SoundBuffer b_fever_start;

    sf::SoundBuffer b_metronome;
    sf::SoundBuffer b_ding;
    sf::SoundBuffer b_anvil;

    sf::Sound s_theme[2];    ///For playing BGM
    sf::Sound s_fever_fail;  ///Dying fever sound
    sf::Sound s_fever_start; ///FEVER!
    sf::Sound s_chant;       ///For playing chants
    
    sf::Sound s_metronome; //rhythm helper
    sf::Sound s_ding; //rhythm helper
    sf::Sound s_anvil; //rhythm helper

    /// Initialize clocks ///
    sf::Clock beatCycleClock; ///Clock for proper command inputs and requirements

    /// Initialize Rhythm System values ///
    int combo = 1;     ///Rhythm combo, main navigator through BGM
    int rl_combo = 0;  ///Game combo, directing mechanics and fever

    std::vector<int> base5_commands; // commands in quinary format (pata=0,pon=1,don=2,chaka=3,none=4)  
    std::vector<std::string> av_songs; // song names for patapon singing

    std::vector<float> acc_req = {0, 1, 1, 0.9325, 0.875, 0.8125, 0.75, 0.75, 0.75, 0.6875, 0.625};

    std::vector<float> perfects_reward = {0, 50, 150, 250, 300};
    std::vector<float> satisfaction_requirement = {0, 0, 285, 285, 275, 265, 250, 250, 185, 185, 0};
    std::vector<float> satisfaction_value;

    float last_satisfaction = 0;

    /// Perfection calculator ///
    float accuracy = 0; ///value for calculating the accuracy
    int acc_count = 3;  ///value for determining on how far back should the accuracy calculation system go in commands

public:
    /// Default FPS value ///
    float fps = 60;

    int current_perfect;
    bool advanced_prefever = false; ///When the game is still before fever, but gets more livid
    bool updateworm = false;        ///For fever worm

    /// Config and Keybindings ///
    std::string current_song = "";

    sf::SoundBuffer s_badrhythm1, s_badrhythm2; ///absolutely terrible! (shoutouts to shockturtle)
    sf::Sound pata_react;

    // Callback system for rhythm engine
    enum RhythmAction // describes rhythm actions that are going to be broadcasted to any entity that wants to use the rhythm engine
    {
        DRUM_BEST = 0, // perfect drum timing
        DRUM_GOOD = 1, // okay drum timing
        DRUM_BAD = 2, // terrible drum timing
        FEVER_ON = 3, // fever has started
        FEVER_OFF = 4, // fever has been stopped
        COMMAND = 5, // command was executed
        COMBO_BREAK = 6, // combo has been broken
        COMBO_INCREASE = 7, // combo just raised by one
        PERFECT_COMMAND = 8, // shwing noise called, perfect command detected
        FOUND_COMMAND = 9 // command has been found, not yet executed
    };

    struct RhythmMessage
    {
        RhythmAction action;
        uint64_t timestamp;
        std::string message;
    };

    std::vector<RhythmMessage> messages;

    std::vector<int> command;
    float perfection = 0;

    int metronomeDelay = 2;
    int drumTicks = 0;
    int drumTicksNoInput = 0;

    sf::Clock firstCommandDelayClock; //halfbeat delay for when we use first command without last halfbeat
    sf::Clock commandWaitClock; //clock for command execution (if no input provided within given frame, break combo)
    sf::Clock afterMeasureClock; //clock for patapon singing (lock input)
    sf::Clock afterPerfectClock; //count one beat after a perfect noise was hit (prevents from additional halfbeats after a proper command was done)
    bool firstCommandDelay = false;

    std::mutex mtx;

    uint64_t lastMessageCheck = 0;

    bool started = false;

    Rhythm();
    void Clear();
    void Stop();
    void LoadTheme(std::string theme);
    void Start();
    void BreakCombo();
    int GetCombo();
    int GetBgmCycle();
    int GetRealCombo();
    float GetSatisfaction();
    void addRhythmMessage(RhythmAction action_id, std::string message);
    std::vector<RhythmMessage> fetchRhythmMessages(uint64_t& timestamp);
    void checkRhythmController();
    void doRhythm();
    void Draw();
};

#endif // RHYTHM_H
