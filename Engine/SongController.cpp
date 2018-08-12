#include "SongController.h"

#include <fstream>
#include <iostream>
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
    std::map<int,sf::SoundBuffer> b_theme;
    std::map<std::string,sf::SoundBuffer> b_chant;
    ///Load the BGM
    b_theme[0].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_start.ogg");
    b_theme[1].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_1.ogg");
    b_theme[2].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_2.ogg");
    b_theme[3].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_3.ogg");
    b_theme[4].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_4.ogg");
    b_theme[5].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_5.ogg");
    b_theme[6].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_6.ogg");
    b_theme[7].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_7.ogg");
    b_theme[8].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_8.ogg");
    b_theme[9].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_9.ogg");
    b_theme[10].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_10.ogg");
    b_theme[11].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_feverstart.ogg");
    b_theme[12].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_fever1.ogg");
    b_theme[13].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_fever2.ogg");
    b_theme[14].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_fever3.ogg");
    b_theme[15].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_fever4.ogg");
    b_theme[16].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_fever5.ogg");
    b_theme[17].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_fever6.ogg");
    b_theme[18].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_fever7.ogg");
    b_theme[19].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_fever8.ogg");
    b_theme[20].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_fever9.ogg");
    b_theme[21].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_fever10.ogg");
    b_theme[22].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_fever11.ogg");
    b_theme[23].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_fever12.ogg");
    b_theme[24].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_fever13.ogg");
    b_theme[25].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_fever14.ogg");
    b_theme[26].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_fever15.ogg");
    b_theme[27].loadFromFile("resources/sfx/bgm/"+songName+"/bgm_fever16.ogg");

    ///All chants
    b_chant["patapata_1"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_patapata1.ogg");
    b_chant["patapata_2"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_patapata2.ogg");
    b_chant["patapata_3"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_patapata3.ogg");
    b_chant["patapata_4"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_patapata4.ogg");
    b_chant["chakachaka_1"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_chakachaka1.ogg");
    b_chant["chakachaka_2"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_chakachaka2.ogg");
    b_chant["chakachaka_3"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_chakachaka3.ogg");
    b_chant["chakachaka_4"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_chakachaka4.ogg");
    b_chant["ponpon_1"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_ponpon1.ogg");
    b_chant["ponpon_2"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_ponpon2.ogg");
    b_chant["ponpon_3"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_ponpon3.ogg");
    b_chant["ponpon_4"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_ponpon4.ogg");
    b_chant["ponpata_1"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_ponpata1.ogg");
    b_chant["ponpata_2"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_ponpata2.ogg");
    b_chant["ponpata_3"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_ponpata3.ogg");
    b_chant["ponchaka_1"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_ponchaka1.ogg");
    b_chant["ponchaka_2"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_ponchaka2.ogg");
    b_chant["ponchaka_3"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_ponchaka3.ogg");
    b_chant["donchaka_1"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_donchaka1.ogg");
    b_chant["donchaka_2"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_donchaka2.ogg");
    b_chant["donchaka_3"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_donchaka3.ogg");
    b_chant["dondon_1"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_dondon1.ogg");
    b_chant["dondon_2"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_dondon2.ogg");
    b_chant["dondon_3"].loadFromFile("resources/sfx/bgm/"+chantName+"/s_dondon3.ogg");
    Song *currentSong = new Song(b_theme,b_chant);
    songs.push_back(*currentSong);
}
sf::SoundBuffer SongController::GetSongByNumber(std::int songIndex, std::int songNumber){
    return songs[songIndex]>>GetSongNumber(songNumber);
}
sf::SoundBuffer SongController::GetChantByNumber(std::int songIndex, std::int chantNumber){
    return songs[songIndex]>>GetSongNumber(chantNumber);
}

