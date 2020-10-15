#include "ScreenFade.h"

ScreenFade::ScreenFade()
{

}

void ScreenFade::Create(Config* config, int mode, float speed)
{
    cur_mode = mode;
    cur_speed = speed;

    r_rect.setSize({config->GetInt("resX"), config->GetInt("resY")});
    //r_rect.setFillColor(sf::Color::Black);

    switch(cur_mode)
    {
        case 0:
        {
            alpha = 255;
            dest_alpha = 0;
            r_rect.setFillColor(sf::Color(0,0,0,alpha));
            break;
        }

        case 1:
        {
            alpha = 0;
            dest_alpha = 255;
            r_rect.setFillColor(sf::Color(0,0,0,alpha));
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
    if(round(alpha) == dest_alpha)
    return true;
    else
    return false;
}

void ScreenFade::draw(sf::RenderWindow& window, float fps)
{
    if(dest_alpha < alpha)
    {
        alpha -= cur_speed / fps;
    }
    else if(dest_alpha > alpha)
    {
        alpha += cur_speed / fps;
    }

    if(alpha <= 0)
    alpha = 0;

    if(alpha >= 255)
    alpha = 255;

    r_rect.setFillColor(sf::Color(0,0,0,alpha));
    window.draw(r_rect);
}
