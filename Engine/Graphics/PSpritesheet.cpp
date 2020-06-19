#include "PSpritesheet.h"
#include "../Func.h"
#include <fstream>
#include <iostream>

using namespace std;

PSpritesheet::PSpritesheet()
{

}

void PSpritesheet::load(std::string file, int q, int r)
{
    std::cout<<file<<std::endl;
    std::string a = file.substr(0,file.find_last_of("."));
    std::string b = file.substr(file.find_last_of("."));
    std::string sq = "";

    switch(q)
    {
        case 0:
        sq = "L";
        break;

        case 1:
        sq = "M";
        break;

        case 2:
        sq = "H";
        break;

        case 3:
        sq = "U";
        break;
    }

    std::string c = a+"_"+sq+b;
    std::string d = a+"_"+sq+".spr";

    qualitySetting = q;
    resSetting = r;

    std::cout << "[PSPRITESHEET] Loading " << c << std::endl;

    t.loadFromFile(c);
    t.setSmooth(true);

    ifstream spr(d);
    string buff;

    while(getline(spr, buff))
    {
        vector<string> s_rect = Func::Split(buff, ',');

        rect.push_back(sf::IntRect(atoi(s_rect[0].c_str()),atoi(s_rect[1].c_str()),atoi(s_rect[2].c_str()),atoi(s_rect[3].c_str())));
    }

    spr.close();
}

sf::IntRect PSpritesheet::get_bounds(int id)
{
    return rect[id];
}
