#include "Song.h"
#include "Func.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <string>

using namespace std;

Song::Song(std::map<int,sf::SoundBuffer> b_theme, std::map<std::string,sf::SoundBuffer> b_chant)
{

}

void Song::ReloadSong()
{

}

sf::SoundBuffer Song::GetThemeByNumber(std::int number){
    return this.b_theme[number];
}
sf::SoundBuffer Song::GetChantByNumber(std::int number){
    return this.b_chant[number];
}
