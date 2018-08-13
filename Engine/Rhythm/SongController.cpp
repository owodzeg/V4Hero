#include "SongController.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <string>
using namespace std;

SongController::SongController()
{
    // We create a song controller

}

void SongController::LoadSongController()
{
    // Load the song controller
}

void SongController::LoadSongFromName(std::string songName, std::string chantName)
{
    Song* currentSong = new Song(songName,chantName);
    songs.push_back(*currentSong);
}
const sf::SoundBuffer& SongController::GetSongByNumber(int songIndex, int songNumber){
    return songs[songIndex].GetThemeByNumber(songNumber);
}
const sf::SoundBuffer& SongController::GetChantByNumber(int songIndex, string chantName){
    return songs[songIndex].GetChantByName(chantName);
}

const sf::SoundBuffer& SongController::GetSongByName(string songName, int songNumber){
    return songs[songIndex].GetThemeByNumber(songNumber);
}
const sf::SoundBuffer& SongController::GetChantByName(string songName, string chantName){
    return songs[songIndex].GetChantByName(chantName);
}
