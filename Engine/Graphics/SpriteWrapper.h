#ifndef SPRITEWRAPPER_H
#define SPRITEWRAPPER_H

#include "../ResourceManager.h"
#include "PSprite.h"

class SpriteWrapper
{
    public:
    std::string spritePath;
    sf::Vector2f position, origin, scale;
    sf::FloatRect l_bounds, g_bounds;

    SpriteWrapper();
    void load(std::string path);
    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition();
    void setOrigin(sf::Vector2f ori);
    void setScale(sf::Vector2f sc);
    sf::FloatRect getLocalBounds();
    sf::FloatRect getGlobalBounds();
    void draw();
};

#endif