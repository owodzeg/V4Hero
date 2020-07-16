#ifndef PSPRITESHEET_H
#define PSPRITESHEET_H

#include <SFML/Graphics.hpp>

class PSpritesheet
{
    public:
    int qualitySetting=0, resSetting=0;

    sf::Texture t;
    std::vector<char> t_c;
    std::vector<sf::IntRect> rect;

    PSpritesheet();
    void load(std::string file, int q, int r);
    sf::IntRect get_bounds(int id); ///get bounds by sprite id
    std::vector<char> retrieve_char();
    std::map<int, sf::IntRect> retrieve_rect_as_map();
};

#endif // PSPRITESHEET_H
