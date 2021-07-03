#include "PSpritesheet.h"
#include "../Func.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

PSpritesheet::PSpritesheet()
{
}

void PSpritesheet::load(std::string file, int q, int r)
{
    std::cout << file << std::endl;
    std::string a = file.substr(0, file.find_last_of("."));
    std::string b = file.substr(file.find_last_of("."));
    std::string sq = "";

    switch (q)
    {
        case 0:
            sq = "";
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

    std::string c = a + "_" + sq + b;
    std::string d = a + "_" + sq + ".spr";

    qualitySetting = q;
    resSetting = r;

    std::cout << "[PSPRITESHEET] Loading " << c << std::endl;

    std::ifstream sfile(c, ios::binary);
    std::ostringstream ss;
    ss << sfile.rdbuf();
    const std::string& s = ss.str();
    std::vector<char> vec(s.begin(), s.end());
    t_c = vec;

    cout << "[PSPRITESHEET] Loading the binary buffer from " << c << " size: " << t_c.size() << endl;

    t.loadFromFile(c);
    t.setSmooth(true);

    ifstream spr(d);
    string buff;

    while (getline(spr, buff))
    {
        vector<string> s_rect = Func::Split(buff, ',');

        rect.push_back(sf::IntRect(atoi(s_rect[0].c_str()), atoi(s_rect[1].c_str()), atoi(s_rect[2].c_str()), atoi(s_rect[3].c_str())));
    }

    spr.close();
}

sf::IntRect PSpritesheet::get_bounds(int id)
{
    return rect[id];
}

std::vector<char> PSpritesheet::retrieve_char()
{
    return t_c;
}

std::map<int, sf::IntRect> PSpritesheet::retrieve_rect_as_map()
{
    std::map<int, sf::IntRect> a;
    for (int i = 0; i < rect.size(); i++)
    {
        a[i] = rect[i];
    }

    return a;
}
