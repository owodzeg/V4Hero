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
    Song(std::string songName, std::string chantName);
    const sf::SoundBuffer& GetThemeByNumber(int number);
    const sf::SoundBuffer& GetChantByName(std::string chantName);
    std::string name;
    std::map<int,sf::SoundBuffer> b_theme; ///Sound buffer for main theme
    std::map<std::string,sf::SoundBuffer> b_chant; ///Sound buffer for Patapon chants
};

#endif // SONG_H
