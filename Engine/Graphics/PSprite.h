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
    float ratioX=1,ratioY=1;
    float scaleX=1,scaleY=1;
    float lx,ly;
    bool DoAutoScale;
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
    void setScale(float s);
    void draw(sf::RenderWindow& window,float rotation=0);
};

#endif // PSPRITE_H
