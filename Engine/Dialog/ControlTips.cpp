#include "ControlTips.h"

ControlTips::ControlTips()
{
}

void ControlTips::create(float ysz, sf::Font font, int characterSize, std::wstring displayText, int qualitySetting, sf::Color color)
{
    ySize = ysz;

    text.createText(font, characterSize, color, displayText, qualitySetting, 1);
}

void ControlTips::draw(sf::RenderWindow& window)
{
    box.setSize(sf::Vector2f(window.getSize().x, ySize * (window.getSize().x / float(1280))));
    box.setFillColor(sf::Color(0, 0, 0, 128));
    box.setPosition(x, y * (window.getSize().x / float(1280)));

    window.draw(box);

    text.setOrigin(0, text.getLocalBounds().height / 2);
    text.setPosition(x + 16, y + (ySize / 2));
    text.draw(window);
}
