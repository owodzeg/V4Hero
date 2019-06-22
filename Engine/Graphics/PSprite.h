#ifndef PSPRITE_H
#define PSPRITE_H

#include <SFML/Graphics.hpp>

///Class for automatic sprite resizing based on the current window resolution and quality options

class PSprite
{
    public:
    sf::Texture t;
    sf::Sprite s;
    int quality;

    PSprite();
    void loadFromFile(std::string file, int q);
    void setRepeated(bool r);
    void setTextureRect(sf::IntRect rect);
    void setOrigin(float x, float y);
    void setColor(sf::Color color);
    void setTexture(sf::Texture& texture);
    void setSprite(sf::Sprite& sprite);
    void setPosition(float x, float y);
    sf::FloatRect getLocalBounds();
    void draw(sf::RenderWindow& window);
};

#endif // PSPRITE_H
