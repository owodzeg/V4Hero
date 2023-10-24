#ifndef RHYTHM_H
#define RHYTHM_H

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>

#include "../Config.h"
#include "../Func.h"

#include "Drum.h"
#include "RhythmController.h"
#include "RhythmGUI.h"
#include "SongController.h"

#include "../Input/InputController.h"


class Rhythm
{
public:
    /// Low and high range for BAD, GOOD and BEST hits (in milliseconds, 0 is the center point) ///
    float BPM = 120.f; ///beats per minute
    float beat_timer = 60.f / (BPM*2) * 1000000.f; ///Amount of microseconds for each halfbeat to be made
    float low_range = beat_timer / 6.25f;  ///Anything below that range will be treated as BAD hit
    float high_range = beat_timer / 3.125f; ///Anything between this and low range will be treated as GOOD hit. Higher will be treated as BEST hit.
    sf::Clock rhythmClock;    ///Main clock for Rhythm purposes
    sf::Clock newRhythmClock;    ///Main clock for Rhythm purposes

    int metronomeVal = 0;
    int metronomeOldVal = 9999;
    int metronomeState = 1;
    bool metronomeClick = false;

private:
    std::shared_ptr<spdlog::logger> logger = spdlog::get("rhythm");
    /// Theme and chant buffers ///
    std::map<std::string, sf::SoundBuffer> b_chant; ///Sound buffer for Patapon chants
    std::vector<std::unique_ptr<SongController>> songController;
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

    sf::Sound s_theme[2];    ///For playing BGM
    sf::Sound s_fever_fail;  ///Dying fever sound
    sf::Sound s_fever_start; ///FEVER!
    sf::Sound s_chant;       ///For playing chants
    sf::Sound s_metronome; //rhythm helper

    /// Initialize clocks ///
    sf::Clock beatCycleClock; ///Clock for proper command inputs and requirements

    /// Initialize Rhythm System values ///
    int combo = 1;     ///Rhythm combo, main navigator through BGM
    int rl_combo = 0;  ///Game combo, directing mechanics and fever

    std::vector<std::string> av_commands = {"PATA-PATA-PATA-PON-",
                                            "PON-PON-PATA-PON-",
                                            "CHAKA-CHAKA-PATA-PON-",
                                            "PON-PON-CHAKA-CHAKA-",
                                            "PATA-PON-DON-CHAKA-",
                                            "PON-PATA-PON-PATA-",
                                            "DON-DON-CHAKA-CHAKA-",
                                            "CHAKA-PATA-CHAKA-PATA-",
                                            "PATA-PON-PATA-PON-",
                                            "DON-DONDON-DONDON-"}; ///Available commands

    std::vector<int> base5_commands = {65109, //pata-pata-pata-pon-
                                        146359, //pon-pon-pata-pon-
                                        308859, //chaka-chaka-pata-pon-
                                        146744, //pon-pon-chaka-chaka-
                                        68494, //pata-pon-don-chaka-
                                        143354, //pon-pata-pon-pata- 
                                        227994, //don-don-chaka-chaka-
                                        299854, //chaka-pata-chaka-pata
                                        68234, //pata-pon-pata-pon
                                        226814, //don-dondon-dondon-
                                        304525, //experimental: chaka-dondonponponpatapata
                                        97656, //experimental: ponponponponponponponpon
                                        66569}; //experimental: pata-ponpon-dondonchaka
                                        
    std::vector<std::string> av_songs = {"patapata",
                                         "ponpon",
                                         "chakachaka",
                                         "ponchaka",
                                         "donchaka",
                                         "ponpata",
                                         "dondon",
                                         "chakapata",
                                         "pause",
                                         "summon"}; ///Available songs
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
    Config* config;
    std::map<int, bool> keyMap;
    std::string current_song = "";

    sf::SoundBuffer s_badrhythm1, s_badrhythm2; ///absolutely terrible! (shoutouts to shockturtle)
    sf::Sound pata_react;

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
    void checkRhythmController();
    void doRhythm();
    void Draw();
};

#endif // RHYTHM_H
