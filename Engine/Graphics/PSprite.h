#ifndef PSPRITE_H
#define PSPRITE_H

#include <SFML/Graphics.hpp>

///Class for automatic sprite resizing based on the current window resolution and quality options

class PSprite
{
    public:
    sf::Texture t;
    sf::Sprite s;
    int quality=1;
    float ratioX=1,ratioY=1;
    float scaleX=1,scaleY=1;
    float orX, orY;
    float lx,ly;
    float angle = 0;
    bool DoAutoScale;
    PSprite();
    void loadFromFile(std::string file, int q);
    void setRepeated(bool r);
    void setTextureRect(sf::IntRect rect);
    void setOrigin(float x, float y);
    void setScale(float x, float y);
    void setRotation(float a);
    void setColor(sf::Color color);
    void setTexture(sf::Texture& texture);
    void setSprite(sf::Sprite& sprite);
    void setPosition(float x, float y);
    sf::Vector2f getPosition();
    sf::FloatRect getLocalBounds();
    sf::FloatRect getGlobalBounds();
    void setScale(float s);
    void draw(sf::RenderWindow& window);
};

#endif // PSPRITE_H
