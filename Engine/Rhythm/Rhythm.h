#ifndef RHYTHM_H
#define RHYTHM_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "../Config.h"
#include "../Func.h"

#include "Drum.h"
#include "SongController.h"
#include "RhythmController.h"
#include "RhythmGUI.h"
class Rhythm
{
    private:
    /// Theme and chant buffers ///
    std::map<std::string,sf::SoundBuffer> b_chant; ///Sound buffer for Patapon chants
    SongController *songController;
    std::string currentThemeName;

    /// Low and high range for BAD, GOOD and BEST hits (in milliseconds, 250 is the center point, 250-range = ms gap) ///
    int low_range = 135; ///Anything below that range will be treated as BAD hit
    int high_range = 225; ///Anything between this and low range will be treated as GOOD hit. Higher will be treated as BEST hit.
    int beat_timer = 500; ///Amount of milliseconds for each beat to be made
    /// Check if it's possible to replace cycles with one (max 2) values based on a clock, would make things more reliable
    int cycle = 0;
    int cycle_mode = 0;
    int bgm_cycle = 0;
    bool combobreak = false;
    bool count_cycle = false;

    /// Initialize sounds ///
    sf::SoundBuffer b_fever_fail;
    sf::SoundBuffer b_fever_start;

    sf::Sound s_theme[2]; ///For playing BGM
    sf::Sound s_fever_fail; ///Dying fever sound
    sf::Sound s_fever_start; ///FEVER!
    sf::Sound s_chant; ///For playing chants



    /// Initialize clocks ///
    sf::Clock rhythmClock; ///Main clock for Rhythm purposes
    sf::Clock beatCycleClock; ///Clock for proper command inputs and requirements

    /// Initialize Rhythm System values ///
    int combo = 1; ///Rhythm combo, main navigator through BGM
    int rl_combo = 0; ///Game combo, directing mechanics and fever
    float flicker = 0; ///For beat frame flickering

    std::vector<std::string> av_commands = {"PATAPATAPATAPON",
                                            "PONPONPATAPON",
                                            "CHAKACHAKAPATAPON",
                                            "PONPONCHAKACHAKA",
                                            "PATAPONDONCHAKA",
                                            "PONPATAPONPATA",
                                            "DONDONCHAKACHAKA",
                                            "CHAKAPATACHAKAPATA"}; ///Available commands
    std::vector<std::string> av_songs = {"patapata",
                                        "ponpon",
                                        "chakachaka",
                                        "ponchaka",
                                        "donchaka",
                                        "ponpata",
                                        "dondon",
                                        "patapata"}; ///Available songs
    std::vector<float> acc_req = {0,1,1,0.9325,0.875,0.8125,0.75,0.75,0.75,0.6875,0.625};

    std::vector<float> perfects_reward = {0, 50, 150, 250, 300};
    std::vector<float> satisfaction_requirement = {0, 0, 285, 285, 275, 265, 250, 250, 185, 185, 0};
    std::vector<float> satisfaction_value;

    float last_satisfaction = 0;


    /// Perfection calculator ///
    float accuracy = 0; ///value for calculating the accuracy
    int acc_count = 3; ///value for determining on how far back should the accuracy calculation system go in commands


    /// Drums in-game ///
    std::vector<Drum> drums;

    public:
    RhythmGUI r_gui;
    /// Default FPS value ///
    float fps = 60;

    int current_perfect;
    bool advanced_prefever = false; ///When the game is still before fever, but gets more livid
    bool updateworm = false; ///For fever worm
    /// Config and Keybindings ///
    Config config;
    std::map<int,bool> keyMap;
    RhythmController rhythmController;
    std::string current_song = "";





    Rhythm();
    void Stop();
    void LoadTheme(std::string theme);
    void BreakCombo();
    int GetCombo();
    int GetRealCombo();
    float GetSatisfaction();
    void checkRhythmController(sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);

};

#endif // RHYTHM_H
