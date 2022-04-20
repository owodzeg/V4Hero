#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "../Config.h"
#include "../Graphics/PSprite.h"
#include "Camera.h"
#include <SFML/Graphics.hpp>

class Background
{
public:
    Camera camera;

    sf::VertexArray v_background;
    std::vector<sf::Vector2f> vx_pos;
    std::vector<sf::Color> vx_color;

    std::map<int, PSprite> s_background; //changed from vector to map: never use vectors for things that contain textures!!!!!!
    std::vector<sf::Vector2f> p_background;
    std::vector<float> background_xspeed;
    std::vector<float> background_y;
    sf::RectangleShape r_ground;

    float temp_camerax = 0;
    float floor_height = 0;
    int quality = 0;
    int bg_layer = 0;

    std::vector<std::string> t_background;
    std::vector<sf::Color> c_background;

    Background();
    void setCamera(Camera camera);
    void Load(std::string bg_name);
    void Draw();
};

#endif // BACKGROUND_H
