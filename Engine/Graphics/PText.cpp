#include "PText.h"

PText::PText()
{

}

void PText::createText(sf::Font font, float characterSize, sf::Color color, std::string text_string, int q, int r)
{
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
    t.setColor(color);
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
    return sf::FloatRect(t.getGlobalBounds().left, t.getGlobalBounds().top, t.getGlobalBounds().width / resRatioX, t.getGlobalBounds().height / resRatioY);
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
    t.setCharacterSize(cS);
    t.setString(txt);
    t.setScale(ratioX*scaleX, ratioY*scaleY);
    t.setOrigin(orX,orY);
    t.setPosition(lx*resRatioX, ly*resRatioY);
    t.setRotation(angle*(180/3.14159265358));
    window.draw(t);
}
