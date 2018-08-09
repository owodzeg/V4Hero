#include "Drum.h"
#include <iostream>

using namespace std;

Drum::Drum()
{

}

void Drum::Load(string drum, sf::RenderWindow& window)
{
    if(drum == "pata")
    {
        t_drum.loadFromFile("resources/graphics/rhythm/drums/pata.png");
        x = 10;
        y = window.getSize().y/2-30;
    }

    if(drum == "pon")
    {
        t_drum.loadFromFile("resources/graphics/rhythm/drums/pon.png");
        x = window.getSize().x-150;
        y = window.getSize().y/2-30;
    }

    if(drum == "don")
    {
        t_drum.loadFromFile("resources/graphics/rhythm/drums/don.png");
        x = window.getSize().x/2-20;
        y = window.getSize().y-150;
    }

    if(drum == "chaka")
    {
        t_drum.loadFromFile("resources/graphics/rhythm/drums/chaka.png");
        x = window.getSize().x/2-30;
        y = 30;
    }

    t_drum.setSmooth(true);

    drumClock.restart();
}

void Drum::Draw(sf::RenderWindow& window)
{
    float ratio_X = window.getSize().x / float(1280);
    float ratio_Y = window.getSize().y / float(720);

    if(drumClock.getElapsedTime().asSeconds() > 0.5)
    {
        alpha -= float(510) / fps;

        if(alpha <= 0)
        alpha = 0;
    }

    s_drum.setTexture(t_drum);
    s_drum.setScale(ratio_X,ratio_Y);
    s_drum.setColor(sf::Color(255,255,255,alpha));
    s_drum.setPosition(x,y);
    window.draw(s_drum);
}
