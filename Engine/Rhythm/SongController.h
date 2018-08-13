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
    sf::SoundBuffer GetSongByNumber(int songIndex, int songNumber);
    sf::SoundBuffer GetChantByNumber(int songIndex, std::string chantName);
};

#endif // SONGCONTROLLER_H
