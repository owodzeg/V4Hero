#ifndef PSPRITE_H
#define PSPRITE_H

#include <SFML/Graphics.hpp>

///Class for automatic sprite resizing based on the current window resolution and quality options

class PSprite
{
public:
    sf::Texture t; //deprecated
    sf::Sprite s;
    std::string texturePath;
    int qualitySetting = -1;
    int resSetting = 1;
    float ratioX = 1, ratioY = 1;
    float resRatioX = 1, resRatioY = 1;
    float scaleX = 1, scaleY = 1;
    float orX = 0, orY = 0;
    float baseX = -999, baseY = -999;
    float lx = 0, ly = 0;
    float angle = 0;
    bool exported = false;
    PSprite();
    void loadFromFile(std::string file, int q);
    void loadFromFile(std::string file, int q, int r);
    void setRepeated(bool r);
    void setTextureRect(sf::IntRect rect);
    void setOrigin(float x, float y);
    void setScale(float x, float y);
    void setRotation(float a);
    void setColor(sf::Color color);
    sf::Color getColor();
    void setTexture(sf::Texture& texture);
    void applyTexture();
    void setSprite(sf::Sprite& sprite);
    void setPosition(float x, float y);
    sf::Vector2f getPosition();
    sf::FloatRect getLocalBounds();
    sf::FloatRect getGlobalBounds();
    sf::FloatRect getGlobalBoundsScaled();
    void setScale(float ss);
    void setSmooth(bool smooth);
    void draw(sf::RenderWindow& window);
    void update(sf::RenderWindow& window);
    void saveToFile(std::string file);
};

#endif // PSPRITE_H
