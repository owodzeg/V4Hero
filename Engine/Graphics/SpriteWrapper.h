#ifndef SPRITEWRAPPER_H
#define SPRITEWRAPPER_H

#include "PSprite.h"

class SpriteWrapper
{
    public:
    std::string spritePath;
    sf::Vector2f position, origin, scale;
    sf::FloatRect l_bounds, g_bounds;
    sf::IntRect texture_rect;
    sf::Color color;
    bool repeated;
    float rotation;

    SpriteWrapper();
    void load(std::string path);
    void setPosition(sf::Vector2f pos);
    void setPosition(float x, float y);
    sf::Vector2f getPosition();
    void setOrigin(sf::Vector2f ori);
    void setOrigin(float x, float y);
    sf::Vector2f getOrigin();
    void setScale(sf::Vector2f sc);
    void setScale(float x, float y);
    void setScale(float x);
    sf::FloatRect getLocalBounds();
    sf::FloatRect getGlobalBounds();
    void setColor(sf::Color c);
    sf::IntRect getTextureRect();
    void setTextureRect(sf::IntRect rect);
    void setRepeated(bool isRepeated);
    void setRotation(float angle);
    float getRotation();
    void draw();
};

#endif