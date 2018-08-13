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
    const sf::SoundBuffer& GetSongByNumber(int songIndex, int songNumber);
    const sf::SoundBuffer& GetChantByNumber(int songIndex, std::string chantName);
};

#endif // SONGCONTROLLER_H
