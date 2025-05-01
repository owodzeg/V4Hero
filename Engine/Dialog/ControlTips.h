#ifndef CONTROLTIPS_H
#define CONTROLTIPS_H

#include <SFML/Graphics.hpp>

#include "../Graphics/PataText.h"

class ControlTips
{
public:
    float x = 0, y = 0;
    float ySize = 0;

    sf::RectangleShape box;
    PataText text;

    ControlTips();
    void create(float ysz, std::string font, int characterSize, std::string displayText, sf::Color color = sf::Color(222, 222, 222, 255));
    void draw();
};

#endif // CONTROLTIPS_H
