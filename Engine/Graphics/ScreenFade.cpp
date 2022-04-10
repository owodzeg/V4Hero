#include "ScreenFade.h"
#include <math.h>

#include "../CoreManager.h"

ScreenFade::ScreenFade()
{
}

void ScreenFade::Create(int mode, float speed)
{
    cur_mode = mode;
    cur_speed = speed;

    Config* config = CoreManager::getInstance().getConfig();

    r_rect.setSize(sf::Vector2f(config->GetInt("resX"), config->GetInt("resY")));
    //r_rect.setFillColor(sf::Color::Black);

    switch (cur_mode)
    {
        case 0: {
            alpha = 255;
            dest_alpha = 0;
            r_rect.setFillColor(sf::Color(0, 0, 0, alpha));
            break;
        }

        case 1: {
            alpha = 0;
            dest_alpha = 255;
            r_rect.setFillColor(sf::Color(0, 0, 0, alpha));
            break;
        }
    }
}

void ScreenFade::FadeIn()
{
}

void ScreenFade::FadeOut()
{
}

bool ScreenFade::checkFinished()
{
    if (round(alpha) == dest_alpha)
        return true;
    else
        return false;
}

//TO-DO: fully replace this draw function with one that doesnt have window and fps in arguments
void ScreenFade::draw(sf::RenderWindow& window, float fps)
{
    if (dest_alpha < alpha)
    {
        alpha -= cur_speed / fps;
    } else if (dest_alpha > alpha)
    {
        alpha += cur_speed / fps;
    }

    if (alpha <= 0)
        alpha = 0;

    if (alpha >= 255)
        alpha = 255;

    r_rect.setFillColor(sf::Color(0, 0, 0, alpha));
    window.draw(r_rect);
}

void ScreenFade::draw()
{
    float fps = CoreManager::getInstance().getCore()->getFPS();
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();

    if (dest_alpha < alpha)
    {
        alpha -= cur_speed / fps;
    } else if (dest_alpha > alpha)
    {
        alpha += cur_speed / fps;
    }

    if (alpha <= 0)
        alpha = 0;

    if (alpha >= 255)
        alpha = 255;

    r_rect.setFillColor(sf::Color(0, 0, 0, alpha));
    window->draw(r_rect);
}
