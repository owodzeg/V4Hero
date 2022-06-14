#include "ControlTips.h"
#include "Func.h"
#include "../CoreManager.h"

// TO-DO: make this class look better in game
// what i mean is have it accept parameters as what specific keys are currently used
// and then auto align the text so it looks cool and nice
// it will allow for dynamic swapping between keyboard and joystick when needed

ControlTips::ControlTips()
{
}

void ControlTips::create(float ysz, sf::Font font, int characterSize, std::string displayText, int qualitySetting, sf::Color color)
{
    ySize = ysz;

    text.createText(font, characterSize, color, Func::ConvertToUtf8String(displayText), qualitySetting, 1);
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

void ControlTips::draw()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();

    box.setSize(sf::Vector2f(window->getSize().x, ySize * (window->getSize().x / float(1280))));
    box.setFillColor(sf::Color(0, 0, 0, 128));
    box.setPosition(x, y * (window->getSize().x / float(1280)));

    window->draw(box);

    text.setOrigin(0, text.getLocalBounds().height / 2);
    text.setPosition(x + 16, y + (ySize / 2));
    text.draw(window);
}
