#ifndef SONG_H
#define SONG_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <map>
#include <string>

class Song
{
    public:
    Song(std::map<int,sf::SoundBuffer> b_theme, std::map<std::string,sf::SoundBuffer> b_chant);
    std::string name;
    std::map<int,sf::SoundBuffer> b_theme; ///Sound buffer for main theme
    std::map<std::string,sf::SoundBuffer> b_chant; ///Sound buffer for Patapon chants
    sf::SoundBuffer GetThemeByNumber(std::int number);
    sf::SoundBuffer GetChantByNumber(std::int number);
};

#endif // SONG_H
