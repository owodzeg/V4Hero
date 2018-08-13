#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SFML/Graphics.hpp>

class Background
{
    public:
    sf::VertexArray v_background;
    std::vector<sf::Vector2f> vx_pos;
    std::vector<sf::Color> vx_color;

    std::vector<sf::Texture> t_background;
    std::vector<sf::Sprite> s_background;
    std::vector<float> background_xspeed;
    std::vector<float> background_y;

    float temp_camerax = 0;

    Background();
    void Load(std::string bg_name);
    void Draw(sf::RenderWindow& window);
};

#endif // BACKGROUND_H