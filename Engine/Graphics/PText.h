#ifndef PTEXT_H
#define PTEXT_H

#include <SFML/Graphics.hpp>

///Class for automatic text resizing based on the current window resolution and quality options

class PText
{
public:
    sf::Font f;
    sf::Text t;

    sf::String txt;

    int qualitySetting, resSetting;
    float cS = 1;
    sf::Color c;

    float ratioX, ratioY;
    float resRatioX, resRatioY;

    float orX = 0, orY = 0;
    float scaleX = 1, scaleY = 1;
    float lx = 0, ly = 0;
    float angle = 0;

    bool wide = false;
    bool rendered = false;

    PText();
    void createText(sf::Font& font, float characterSize, sf::Color color, sf::String text_string, int q, int r);
    void setOrigin(float x, float y);
    sf::Vector2f getScale();
    void setScale(float x, float y);
    void setRotation(float a);
    void setColor(sf::Color color);
    void setOutlineColor(sf::Color color);
    void setOutlineThickness(int thick);
    void setPosition(float x, float y);
    void setString(std::string text_string);
    void setString(sf::String text_string);
    void setString(const char* text_string);
    void setWString(std::string text_string);
    sf::Vector2f getPosition();
    sf::FloatRect getLocalBounds();
    sf::FloatRect getGlobalBounds();
    sf::FloatRect getGlobalBoundsScaled();
    void setScale(float s);
    void draw(sf::RenderWindow& window);
    void draw(sf::RenderWindow* window);
    void update(sf::RenderWindow& window);
    sf::Text getText();
};

#endif // PTEXT_H
