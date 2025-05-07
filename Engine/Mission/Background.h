#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "../Config.h"
#include "../Graphics/SpriteWrapper.h"
#include "Camera.h"
#include <SFML/Graphics.hpp>

class Background
{
public:
    struct BGObject {
        SpriteWrapper texture;
        sf::Color color;
        sf::Vector2f position;
        float x_speed = 0;
    };

    std::vector<BGObject> bg_objects;

    sf::VertexArray v_background;
    sf::VertexArray v_dark_highlight;
    std::vector<sf::Vector2f> vx_pos;
    std::vector<sf::Color> vx_color;
    sf::RectangleShape r_ground;

    float floor_height = 0;
    int quality = 0;
    float pataSpeed = 0;

    sf::View bgView;

    Background();
    void Load(const std::string& bg_name);
    void Draw(Camera& work_camera);
    void DrawFloor();
};

#endif // BACKGROUND_H
