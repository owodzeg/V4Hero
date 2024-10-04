#include "MouseController.h"

MouseController::MouseController()
{

}

void MouseController::parseEvents(sf::Event& event)
{
    if (event.type == sf::Event::MouseMoved)
    {
        x = event.mouseMove.x;
        y = event.mouseMove.y;
    }

    if (event.type == sf::Event::MouseButtonPressed)
    {
        if(event.mouseButton.button == sf::Mouse::Left)
        {
            lmb = true;
        }
        if(event.mouseButton.button == sf::Mouse::Right)
        {
            rmb = true;
        }
    }

    if (event.type == sf::Event::MouseButtonReleased)
    {
        if(event.mouseButton.button == sf::Mouse::Left)
        {
            lmb = false;
        }
        if(event.mouseButton.button == sf::Mouse::Right)
        {
            rmb = false;
        }
    }
}

sf::Vector2i MouseController::getMousePos()
{
    return sf::Vector2i(x,y);
}

bool MouseController::getClick(int which)
{
    if(which == 0) // left click
    {
        if(lmb)
        {
            lmb = false;
            return true;
        }
    }

    return false;
}