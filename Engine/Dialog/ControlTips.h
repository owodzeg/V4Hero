#ifndef CONTROLTIPS_H
#define CONTROLTIPS_H

#include <SFML/Graphics.hpp>

#include "../Graphics/PText.h"

class ControlTips
{
    public:
    float x=0,y=0;
    float ySize=0;
    float rr=0; ///resRatio

    sf::RectangleShape box;
    PText text;

    ControlTips();
    void create(float ysz, sf::Font font, int characterSize, std::wstring displayText, int qualitySetting, sf::Color color=sf::Color(222,222,222,255));
    void draw(sf::RenderWindow& window);
};

#endif // CONTROLTIPS_H
