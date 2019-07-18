#ifndef WALL_H
#define WALL_H

#include <SFML/Graphics.hpp>
#include "../Config.h"


using namespace std;

class Wall
{
    public:
    sf::Sprite s_wall;
    string current_animation = "idle";
    float current_frame = 0;
    float x = 0;
    float y = 520;
    float fps = 60;
    sf::Rect<float> hitBox;
    float width = 0;
    float scaleX = 1, scaleY = 1; ///TEMPORARY
    sf::Clock walk_timer; ///TEMPORARY FOR SHOWCASE ONLY

    map<string,vector<sf::Texture>> animation_textures;

    vector<string> animation_name;
    vector<int> animation_frames;
    Config *thisConfig;
    Wall();
    void LoadConfig(Config *thisConfigs);
    void Draw(sf::RenderWindow& window);
};

#endif // WALL_H
