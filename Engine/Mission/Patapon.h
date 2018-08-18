#ifndef PATAPON_H
#define PATAPON_H

#include <SFML/Graphics.hpp>

using namespace std;

class Patapon
{
    public:
    sf::Sprite s_patapon;
    string current_animation = "walk";
    float current_frame = 0;

    float x = 0;
    float y = 520;
    float fps = 60;

    vector<string> animation_name;
    vector<int> animation_frames;

    map<string,vector<sf::Texture>> animation_textures;

    Patapon();
    void Draw(sf::RenderWindow& window);
};

#endif // PATAPON_H
