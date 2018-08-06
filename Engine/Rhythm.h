#ifndef RHYTHM_H
#define RHYTHM_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Config.h"
#include "Func.h"

class Rhythm
{
    private:
    /// Theme and chant buffers ///
    std::map<int,sf::SoundBuffer> b_theme;
    std::map<std::string,sf::SoundBuffer> b_chant;



    /// Drums ///
    sf::SoundBuffer b_pata[3],b_pon[3],b_chaka[3],b_don[3];
    sf::SoundBuffer b_chpata[3],b_chpon[3],b_chchaka[3],b_chdon[3];
    std::vector<sf::Sound> s_drums;



    /// Low and high range for BAD, GOOD and BEST hits (in milliseconds) ///
    int low_range = 300;
    int high_range = 425;



    /// Initialize sounds ///
    sf::Sound drum_nc; ///Drum with no chant
    sf::Sound drum_c; ///Drum with chant
    sf::Sound s_theme[2]; ///For playing BGM
    sf::Sound s_fever_fail; ///Dying fever sound
    sf::Sound s_chant; ///For playing chants



    /// Initialize clocks ///
    sf::Clock rhythmClock;
    sf::Clock commandTimeout;
    sf::Clock beforeFeverClock;



    /// Initialize Rhythm System values ///
    int combo = 1;
    float masterTimer = 500;
    int masterTimerMode = 0;
    int beatValue = 1;
    int commandValue = 1;
    bool drumAlreadyHit = false;

    std::vector<std::string> command;
    std::vector<std::string> av_commands = {"PATAPATAPATAPON","PONPONPATAPON","CHAKACHAKAPATAPON","PONPONCHAKACHAKA","PATAPONDONCHAKA","PONPATAPONPATA","DONDONCHAKACHAKA","CHAKAPATACHAKAPATA"};
    std::vector<std::string> av_songs = {"patapata","ponpon","chakachaka","ponchaka","donchaka","ponpata","dondon","patapata"};
    std::string song;



    /// Perfction calculator ///
    int perfect = 0;
    std::vector<int> command_perfects;
    std::vector<int> perfects;
    float accuracy = 0;
    int acc_count = 3;



    public:
    /// Default FPS value ///
    float fps = 60;



    /// Config and Keybindings ///
    Config config;
    std::map<int,bool> keyMap;



    /// Visuals ///
    sf::RectangleShape r_rhythm;



    Rhythm();
    void LoadTheme(std::string theme);
    void Draw(sf::RenderWindow& window);
};

#endif // RHYTHM_H
