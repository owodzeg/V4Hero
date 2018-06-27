#ifndef RHYTHM_H
#define RHYTHM_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Config.h"
#include "Func.h"

class Rhythm
{
    private:
    ///need to cleanup this shit
    ///and make it in a separate thread yea

    std::map<int,sf::SoundBuffer> b_theme;
    std::map<std::string,sf::SoundBuffer> b_chant;
    sf::Sound s_theme1,s_theme2,s_theme3;
    sf::Sound s_chant;

    sf::SoundBuffer b_pata[3],b_pon[3],b_chaka[3],b_don[3];
    sf::SoundBuffer b_chpata[3],b_chpon[3],b_chchaka[3],b_chdon[3];
    std::vector<sf::Sound> s_drums;

    sf::Sound s_pata;

    sf::Clock tclock;
    sf::Clock cclock;
    sf::Clock erasecommand;
    sf::Clock beforefever;
    int test = 1;
    int test2 = 0;
    float timer = 500;
    int timermode = 0;
    int com = 1;
    int comcycle = 1;
    int ans = 1;
    bool cycle = false;

    int perfect = 0;
    std::vector<int> command_perfects;
    std::vector<int> perfects;

    std::vector<std::string> command;
    std::vector<std::string> av_commands;
    std::vector<std::string> av_songs;
    std::string song;

    public:
    float fps = 60;
    sf::RectangleShape r_rhythm;

    bool leftkey = false;
    bool rightkey = false;
    bool upkey = false;
    bool downkey = false;

    Config config;
    std::map<int,bool> keyMap;

    Rhythm();
    void LoadTheme(std::string theme);
    void Draw(sf::RenderWindow& window);
};

#endif // RHYTHM_H
