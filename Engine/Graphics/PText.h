#ifndef PTEXT_H
#define PTEXT_H

#include <SFML/Graphics.hpp>

///Class for automatic text resizing based on the current window resolution and quality options

class PText
{
    public:
    sf::Font f;
    sf::Text t;

    std::string txt;

    int qualitySetting, resSetting;
    float cS = 1;
    sf::Color c;

    float ratioX, ratioY;
    float resRatioX, resRatioY;

    float orX=0, orY=0;
    float scaleX=1, scaleY=1;
    float lx=0,ly=0;
    float angle=0;

    PText();
    void createText(sf::Font font, float characterSize, sf::Color color, std::string text_string, int q, int r);
    void setOrigin(float x, float y);
    sf::Vector2f getScale();
    void setScale(float x, float y);
    void setRotation(float a);
    void setColor(sf::Color color);
    void setPosition(float x, float y);
    void setString(std::string text_string);
    sf::Vector2f getPosition();
    sf::FloatRect getLocalBounds();
    sf::FloatRect getGlobalBounds();
    void setScale(float s);
    void draw(sf::RenderWindow& window);
};

#endif // PTEXT_H
