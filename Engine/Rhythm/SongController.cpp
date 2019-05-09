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
#include "../Func.h"
using namespace std;

SongController::SongController()
{
    configPath = "resources/sfx/bgm/songList.ini";
    this->SaveControllerIniSettings();
}
void SongController::SaveSongsConfig()
{

    cout<< "[!! IMPORTANT !!] Missing ini file. Attempting to recover" << '\n';
    // They don't have the song controller ini file!
    // We create a song controller and give it some initial songs
    SimpleSong song;
    song.themeFilePath = "donjalalin";
    song.chantsFilePath = "donjalalinChant";
    songListings["Donjalin Theme"] = song;
    ofstream conf(configPath);

    if(conf.is_open())
    {
        ///Safety warning
        conf << "# This file contains listings for all of the background songs used in missions by the game - Modifying this file can cause silence or unexpected sounds to appear if you don't know what you are doing #";
        conf << '\n';
        ///File format explainer
        conf << "# The format for this file is as follows: \"In Game Name|Song file path|Chant File Path\" #";
        conf << '\n';

        for(auto const& x : songListings)
        {
            conf << x.first;         // string (key)
            conf << '|';
            conf << x.second.themeFilePath;
            conf << '|';
            conf << x.second.chantsFilePath;     // string's value
            conf << '\n' ;
        }
    }

    conf.close();
}
void SongController::LoadSongsConfig(){
    cout<< "[RHYTHM] Found song config file - loading songs into database" << '\n';
    ifstream conf(configPath);
    if(conf.good())
    {
        string line;
        while(getline(conf, line))
        {
            ///ignore comments
            if(line.find("#") == std::string::npos)
            {
                /// we expect 3 values
                vector<string> key = Func::Split(line,'|');
                if (key.size()!=3){
                    throw "Invalid songs config file - too many segments on line: "+line;
                } else{
                    SimpleSong song;
                    song.themeFilePath = key[1];
                    song.chantsFilePath = key[2];
                    songListings[key[0]] = song;
                }
            }
        }
    }
    conf.close();
    for(auto const& x : songListings)
        {
            cout << '\t' << x.first;         // string (key)
            cout << '|';
            cout << x.second.themeFilePath;
            cout << '|';
            cout << x.second.chantsFilePath;     // string's value
            cout << '\n' ;
            cout << "Finished song listings" << '\n' << '\n';
        }
}
void SongController::SaveControllerIniSettings()
{
    /// Load the song controller
    /// check resources for a song file if it doesn't exist create one.
    ifstream check(configPath);
    bool exists = check.good();
    check.close();
    ///if config not exists
    if(!exists) {
        this->SaveSongsConfig();
    } else {
        this->LoadSongsConfig();
    }
}

void SongController::LoadSongFromPath(std::string songFilePath, std::string chantFilePath)
{
    Song* currentSong = new Song(songFilePath,chantFilePath);
    songs.push_back(*currentSong);
}
void SongController::LoadSongByName(std::string songName)
{
    Song* currentSong = new Song(songListings[songName].themeFilePath,songListings[songName].chantsFilePath);
    songs.push_back(*currentSong);
}
const sf::SoundBuffer& SongController::GetSongByNumber(int songIndex, int songNumber){
    return songs[songIndex].GetThemeByNumber(songNumber);
}
const sf::SoundBuffer& SongController::GetChantByNumber(int songIndex, string chantName){
    return songs[songIndex].GetChantByName(chantName);
}

const sf::SoundBuffer& SongController::GetSongByName(string songName, int songNumber){
    for ( Song &i : songs ) {
        if (i.name==songName){
            return i.GetThemeByNumber(songNumber);
        }
    }
    throw "No song loaded with the name: "+songName;
}
const sf::SoundBuffer& SongController::GetChantByName(string songName, string chantName){
    for ( Song &i : songs ) {
        if (i.name==chantName){
            return i.GetChantByName(chantName);
        }
    }
    throw "No chant loaded with the name: "+chantName;
}
