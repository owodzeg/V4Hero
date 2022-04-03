#ifndef ROUNDEDRECT_H
#define ROUNDEDRECT_H

#include <SFML/Graphics.hpp>

class RoundedRect
{
public:
    sf::CircleShape edges[4];
    sf::RectangleShape connections[4];
    sf::RectangleShape contents;

    float x = 0, y = 0;
    float orx = 0, ory = 0;
    float width = 0, height = 0;
    float edge_size = 0;

    RoundedRect();
    void Create(float nwidth, float nheight, float nedge_size, float resRatio, sf::Color rectColor = sf::Color(255, 237, 210, 255));
    void setOrigin(sf::Vector2f origin);
    void Draw();
    void Draw(sf::RenderWindow& window);
};

#endif // ROUNDEDRECT_H
