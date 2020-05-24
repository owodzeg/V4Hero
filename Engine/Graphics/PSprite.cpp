#include "PSprite.h"
#include <iostream>

PSprite::PSprite()
{
DoAutoScale=true;
}

void PSprite::loadFromFile(std::string file, int q)
{
    std::cout<<file<<std::endl;
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

    qualitySetting = q;
    resSetting = 1;

    std::cout << "[PSPRITE] Loading " << c << std::endl;

    t.loadFromFile(c);
    t.setSmooth(true);
    s.setTexture(t);
}

void PSprite::loadFromFile(std::string file, int q, int r=1)
{
    std::cout<<file<<std::endl;
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

    qualitySetting = q;
    resSetting = r;

    std::cout << "[PSPRITE] Loading " << c << std::endl;

    t.loadFromFile(c);
    t.setSmooth(true);
    s.setTexture(t);
}

void PSprite::setRepeated(bool r)
{
    t.setRepeated(r);
    s.setTexture(t);
}

void PSprite::setTextureRect(sf::IntRect rect)
{
    rect.height = rect.height * ratioY;
    s.setTextureRect(rect);
}

void PSprite::setOrigin(float x, float y)
{
    orX = x;
    orY = y;
}

void PSprite::setScale(float x, float y)
{
    scaleX = x;
    scaleY = y;
}

void PSprite::setRotation(float a)
{
    angle = a;
}

void PSprite::setColor(sf::Color color)
{
    s.setColor(color);
}

void PSprite::setTexture(sf::Texture& texture)
{
    t = texture;
    s.setTexture(t,true);
}

void PSprite::setSprite(sf::Sprite& sprite)
{
    s = sprite;
}

void PSprite::setPosition(float x, float y)
{
    //s.setPosition(x*ratioX,y*ratioY);
    lx = x;
    ly = y;

    //std::cout << x << " " << y << "  " << lx << " " << ly << std::endl;
}

sf::Vector2f PSprite::getPosition()
{
    return sf::Vector2f(lx,ly);
}

void PSprite::setScale(float s)
{
    scaleX = s;
    scaleY = s;
}

sf::FloatRect PSprite::getLocalBounds()
{
    return s.getLocalBounds();
}

sf::FloatRect PSprite::getGlobalBounds()
{
    return s.getGlobalBounds();
}

sf::FloatRect PSprite::getGlobalBoundsScaled()
{
    float nw = 1;
    float nh = 1;

    if(s.getGlobalBounds().width > 0)
    nw = s.getGlobalBounds().width / resRatioX;

    if(s.getGlobalBounds().height > 0)
    nh = s.getGlobalBounds().height / resRatioY;

    return sf::FloatRect(s.getGlobalBounds().left, s.getGlobalBounds().top, nw, nh);
    //return s.getGlobalBounds();
}

void PSprite::setSmooth(bool smooth)
{
    t.setSmooth(smooth);
    s.setTexture(t);
}

void PSprite::draw(sf::RenderWindow& window)
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

    s.setTexture(t);
    s.setScale(ratioX*scaleX, ratioY*scaleY);
    s.setOrigin(orX,orY);
    s.setPosition(lx*resRatioX, ly*resRatioY);
    s.setRotation(angle*(180/3.14159265358));
    window.draw(s);
}
