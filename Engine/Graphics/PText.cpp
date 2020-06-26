#include "PText.h"
#include <iostream>

using namespace std;

PText::PText()
{

}

void PText::createText(sf::Font font, float characterSize, sf::Color color, std::string text_string, int q, int r)
{
    cout << "[PText] Creating a new PText object: " << characterSize << " " << text_string << " " << q << " " << r << endl;

    f = font;
    cS = characterSize;
    c = color;
    txt = text_string;

    qualitySetting = q;
    resSetting = r;
}

void PText::setOrigin(float x, float y)
{
    orX = x;
    orY = y;
}

sf::Vector2f PText::getScale()
{
    return sf::Vector2f(scaleX, scaleY);
}

void PText::setScale(float x, float y)
{
    scaleX = x;
    scaleY = y;
}

void PText::setRotation(float a)
{
    angle = a;
}

void PText::setColor(sf::Color color)
{
    c = color;
    t.setFillColor(color);
}

void PText::setOutlineColor(sf::Color color)
{
    c = color;
    t.setOutlineColor(color);
}

void PText::setOutlineThickness(int thick)
{
    t.setOutlineThickness(thick);
}

void PText::setPosition(float x, float y)
{
    //s.setPosition(x*ratioX,y*ratioY);
    lx = x;
    ly = y;

    //std::cout << x << " " << y << "  " << lx << " " << ly << std::endl;
}

void PText::setString(std::string text_string)
{
    txt = text_string;
    t.setString(txt);
}

sf::Vector2f PText::getPosition()
{
    return sf::Vector2f(lx,ly);
}

void PText::setScale(float s)
{
    scaleX = s;
    scaleY = s;
}

sf::FloatRect PText::getLocalBounds()
{
    return t.getLocalBounds();
}

sf::FloatRect PText::getGlobalBounds()
{
    return t.getGlobalBounds();
}

sf::FloatRect PText::getGlobalBoundsScaled()
{
    float nw = 1;
    float nh = 1;

    if(t.getGlobalBounds().width > 0)
    nw = t.getGlobalBounds().width / resRatioX;

    if(t.getGlobalBounds().height > 0)
    nh = t.getGlobalBounds().height / resRatioY;

    return sf::FloatRect(t.getGlobalBounds().left, t.getGlobalBounds().top, nw, nh);
    //return t.getGlobalBounds();
}

void PText::draw(sf::RenderWindow& window)
{
    switch(qualitySetting)
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

    switch(resSetting)
    {
        case 0: ///low
        {
            resRatioX = window.getSize().x / float(640);
            resRatioY = window.getSize().y / float(360);
            break;
        }

        case 1: ///med
        {
            resRatioX = window.getSize().x / float(1280);
            resRatioY = window.getSize().y / float(720);
            break;
        }

        case 2: ///high
        {
            resRatioX = window.getSize().x / float(1920);
            resRatioY = window.getSize().y / float(1080);
            break;
        }

        case 3: ///ultra
        {
            resRatioX = window.getSize().x / float(3840);
            resRatioY = window.getSize().y / float(2160);
            break;
        }
    }

    t.setFont(f);
    t.setColor(c);
    t.setCharacterSize(cS);
    t.setString(txt);
    t.setScale(resRatioX*scaleX, resRatioY*scaleY);
    t.setOrigin(orX,orY);
    t.setPosition(lx*resRatioX, ly*resRatioY);
    t.setRotation(angle*(180/3.14159265358));
    window.draw(t);
}

void PText::update(sf::RenderWindow& window)
{
    switch(qualitySetting)
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

    switch(resSetting)
    {
        case 0: ///low
        {
            resRatioX = window.getSize().x / float(640);
            resRatioY = window.getSize().y / float(360);
            break;
        }

        case 1: ///med
        {
            resRatioX = window.getSize().x / float(1280);
            resRatioY = window.getSize().y / float(720);
            break;
        }

        case 2: ///high
        {
            resRatioX = window.getSize().x / float(1920);
            resRatioY = window.getSize().y / float(1080);
            break;
        }

        case 3: ///ultra
        {
            resRatioX = window.getSize().x / float(3840);
            resRatioY = window.getSize().y / float(2160);
            break;
        }
    }

    t.setFont(f);
    t.setColor(c);
    t.setCharacterSize(cS);
    t.setString(txt);
    t.setScale(resRatioX*scaleX, resRatioY*scaleY);
    t.setOrigin(orX,orY);
    t.setPosition(lx*resRatioX, ly*resRatioY);
    t.setRotation(angle*(180/3.14159265358));
}
