#include "PSprite.h"
#include <iostream>

PSprite::PSprite()
{

}

void PSprite::loadFromFile(std::string file, int q)
{
    std::string a = file.substr(0,file.find_last_of("."));
    std::string b = file.substr(file.find_last_of("."));
    std::string sq = "";

    switch(q)
    {
        case 0:
        sq = "L";
        break;

        case 1:
        sq = "M";
        break;

        case 2:
        sq = "H";
        break;

        case 3:
        sq = "U";
        break;
    }

    std::string c = a+"_"+sq+b;

    quality = q;

    std::cout << "[PSPRITE] Loading " << c << std::endl;

    t.loadFromFile(c);
    s.setTexture(t);
}

void PSprite::setRepeated(bool r)
{
    t.setRepeated(true);
    s.setTexture(t);
}

void PSprite::setTextureRect(sf::IntRect rect)
{
    s.setTextureRect(rect);
}

void PSprite::setOrigin(float x, float y)
{
    s.setOrigin(x,y);
}

void PSprite::setColor(sf::Color color)
{
    s.setColor(color);
}

void PSprite::setTexture(sf::Texture& texture)
{
    t = texture;
    s.setTexture(t);
}

void PSprite::setSprite(sf::Sprite& sprite)
{
    s = sprite;
}

void PSprite::setPosition(float x, float y)
{
    s.setPosition(x,y);
}

sf::FloatRect PSprite::getLocalBounds()
{
    return s.getLocalBounds();
}

void PSprite::draw(sf::RenderWindow& window)
{
    float ratioX=1,ratioY=1;

    switch(quality)
    {
        case 0: ///low
        {
            ratioX = window.getSize().x / float(640);
            ratioY = window.getSize().y / float(360);
            break;
        }

        case 1: ///med
        {
            ratioX = window.getSize().x / float(1280);
            ratioY = window.getSize().y / float(720);
            break;
        }

        case 2: ///high
        {
            ratioX = window.getSize().x / float(1920);
            ratioY = window.getSize().y / float(1080);
            break;
        }

        case 3: ///ultra
        {
            ratioX = window.getSize().x / float(3840);
            ratioY = window.getSize().y / float(2160);
            break;
        }
    }

    s.setTexture(t);
    s.setScale(ratioX, ratioY);
    window.draw(s);
}
