#ifndef PATAPON_H
#define PATAPON_H

#include <SFML/Graphics.hpp>
#include "../../Config.h"

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
    sf::Rect<float> hitBox;
    float scaleX = 1, scaleY = 1; ///TEMPORARY

    vector<string> animation_name;
    vector<int> animation_frames;

    map<string,vector<sf::Texture>> animation_textures;

    Config *thisConfig;
    Patapon();
    void LoadConfig(Config *thisConfigs);
    void Draw(sf::RenderWindow& window);
};

#endif // PATAPON_H
