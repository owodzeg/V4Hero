#ifndef SONGCONTROLLER_H
#define SONGCONTROLLER_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>
#include "Song.h"

class SongController
{
    public:
    std::vector<Song> songs;
    SongController();
    void LoadSongController();
    void LoadSongFromName(std::string songName,std::string chantName);
    sf::SoundBuffer GetSongByNumber(std::int songIndex, std::int songNumber);
    sf::SoundBuffer GetChantByNumber(std::int songIndex, std::int chantNumber);
};

#endif // SONGCONTROLLER_H
