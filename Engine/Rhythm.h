#ifndef RHYTHM_H
#define RHYTHM_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Config.h"
#include "Func.h"
#include "Drum.h"
#include "RhythmController.h"

class Rhythm
{
    private:
    /// Theme and chant buffers ///
    std::map<int,sf::SoundBuffer> b_theme; ///Sound buffer for main theme
    std::map<std::string,sf::SoundBuffer> b_chant; ///Sound buffer for Patapon chants



    /// Drums ///




    /// Low and high range for BAD, GOOD and BEST hits (in milliseconds) ///
    int low_range = 300; ///Anything below that range will be treated as BAD hit
    int high_range = 425; ///Anything between this and low range will be treated as GOOD hit. Higher will be treated as BEST hit.



    /// Initialize sounds ///
    sf::SoundBuffer b_fever_fail;
    sf::SoundBuffer b_fever_start;

    sf::Sound s_theme[2]; ///For playing BGM
    sf::Sound s_fever_fail; ///Dying fever sound
    sf::Sound s_fever_start; ///FEVER!
    sf::Sound s_chant; ///For playing chants



    /// Initialize clocks ///
    sf::Clock rhythmClock; ///Main clock for Rhythm purposes
    sf::Clock commandTimeout; ///Timeout clock to break the combo after not inputting any drum for a while
    sf::Clock beforeFeverClock; ///Clock used while inputting first commands, later it is useless



    /// Initialize Rhythm System values ///
    int combo = 1; ///Rhythm combo, main navigator through BGM
    float masterTimer = 500; ///Master Timer, determines the quality of hit drums
    int masterTimerMode = 0; ///Mode of Master Timer, for synchronizing the time ranges properly
    int beatValue = 1; ///Value for beats (drums) in a command (1, 2, 3, 4)
    int commandValue = 1; ///Value for current command (1 - Patapons sing, 2 - Player input)
    bool drumAlreadyHit = false; ///Made to check if drum has already been hit in 1 beat (to prevent from hitting multiple drums at a time)
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



    /// Perfction calculator ///
    float accuracy = 0; ///value for calculating the accuracy
    int acc_count = 3; ///value for determining on how far back should the accuracy calculation system go in commands



    /// Drums in-game ///
    std::vector<Drum> drums;

    public:
    /// Default FPS value ///
    float fps = 60;




    /// Config and Keybindings ///
    Config config;
    std::map<int,bool> keyMap;
    RhythmController rhythmController;



    /// Visuals ///
    sf::RectangleShape r_rhythm;
    sf::RectangleShape r_rhythm2;

    sf::RectangleShape r_fever;
    sf::RectangleShape r_fever_meter;


    Rhythm();
    void LoadTheme(std::string theme);
    void BreakCombo();
    void checkRhythmController(sf::RenderWindow& window);
    void doVisuals(sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);
};

#endif // RHYTHM_H
