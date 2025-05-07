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

void ControlTips::create(float ysz, std::string font, int characterSize, std::string displayText, sf::Color color)
{
    ySize = ysz;

    //TO-DO: PataText rework
    //text.defaultStyleSetFont(font);
    //text.defaultStyleSetCharSize(characterSize);
    //text.setColor(color);
    //text.setString(displayText);
}

void ControlTips::draw()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();

    box.setSize(sf::Vector2f(static_cast<float>(window->getSize().x), ySize * static_cast<float>((window->getSize().x / float(1280)))));
    box.setFillColor(sf::Color(0, 0, 0, 128));
    box.setPosition(sf::Vector2f(x, y * static_cast<float>(window->getSize().x / float(1280))));

    window->draw(box);
    
    //TO-DO: PataText rework
    //text.setOrigin(0, text.getLocalBounds().size.y / 2);
    //text.setPosition(x + 16, y + (ySize / 2));
    //text.draw();
}
