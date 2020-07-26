#ifndef DIALOGBOX_H
#define DIALOGBOX_H

#include <SFML/Graphics.hpp>
#include "RoundedRect.h"
#include "../Graphics/PText.h"
#include "../Graphics/PSprite.h"

class PataDialogBox
{
    public:
    RoundedRect rr_main;
    PText t_dialogType, t_dialogText;
    std::vector<PText> t_options;

    sf::RectangleShape highlight;
    int option = 0;

    PSprite arrow;

    float arrow_x;

    float width=0, height=0;
    float x=0, y=0;

    PataDialogBox();
    void Create(sf::Font font, std::string text, std::vector<std::string> options, int qualitySetting, float resRatio);
    void Readjust();
    int CheckSelectedOption();
    void Draw(sf::RenderWindow& window, float fps);
};

#endif // DIALOGBOX_H
