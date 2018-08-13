#ifndef SONGCONTROLLER_H
#define SONGCONTROLLER_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>
#include "Song.h"
struct SimpleSong {
    public:
    std::string themeFilePath;
    std::string chantsFilePath;
};
class SongController
{
    public:
    std::vector<Song> songs;
    std::map<std::string,SimpleSong> songListings;
    SongController();

    void SaveControllerIniSettings();
    void LoadSongByName(std::string songName);
    void LoadSongFromPath(std::string songFilePath, std::string chantFilePath);
    const sf::SoundBuffer& GetSongByNumber(int songIndex, int songNumber);
    const sf::SoundBuffer& GetChantByNumber(int songIndex, std::string chantName);
    const sf::SoundBuffer& GetSongByName(std::string songName, int songNumber);
    const sf::SoundBuffer& GetChantByName(std::string songName, std::string chantName);

    private:
    void SaveSongsConfig();
    void LoadSongsConfig();
    std::string configPath;
};

#endif // SONGCONTROLLER_H
