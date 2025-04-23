#include "MouseController.h"

MouseController::MouseController()
{

}

void MouseController::parseEvents(const std::optional<sf::Event>& event)
{
    if (const auto* e = event->getIf<sf::Event::MouseMoved>())
    {
        x = e->position.x;
        y = e->position.y;
    }

    if (const auto* e = event->getIf<sf::Event::MouseButtonPressed>())
    {
        if (e->button == sf::Mouse::Button::Left)
        {
            lmb = true;
        }
        if (e->button == sf::Mouse::Button::Right)
        {
            rmb = true;
        }
    }

    if (const auto* e = event->getIf<sf::Event::MouseButtonReleased>())
    {
        if (e->button == sf::Mouse::Button::Left)
        {
            lmb = false;
        }
        if (e->button == sf::Mouse::Button::Right)
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