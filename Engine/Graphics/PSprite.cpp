#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "PSprite.h"
#include "../TextureManager.h"
#include "../CoreManager.h"
#include <fstream>
#include <iostream>
#include <string>
#include <spdlog/spdlog.h>

void PSprite::loadFromFile(std::string file, int q, bool downscale)
{
    if (qualitySetting != q && qualitySetting != -1)
    {
        SPDLOG_DEBUG("Quality has changed.");

        float a=1, b=1;

        switch (qualitySetting)
        {
            case 0: {
                a = 6;
                break;
            }
            case 1: {
                a = 3;
                break;
            }
            case 2: {
                a = 2;
                break;
            }
        }

        switch (q)
        {
            case 0: {
                b = 6;
                break;
            }
            case 1: {
                b = 3;
                break;
            }
            case 2: {
                b = 2;
                break;
            }
        }

        SPDLOG_DEBUG("current ratio: {}, new ratio: {}", a, b);

        float diff = a / b;

        orX = orX * diff;
        orY = orY * diff;
    }

    qualitySetting = q;
    resSetting = 3;

    SPDLOG_DEBUG("Loading PSprite: {}", file);
    texturePath = file;

    s = std::make_unique<sf::Sprite>(TextureManager::getInstance().getTexture(file, q, downscale));
}

void PSprite::setRepeated(bool r)
{
    SPDLOG_TRACE("Change repeated state of {} to {}", texturePath, r);
    TextureManager::getInstance().getTexture(texturePath).setRepeated(r);
    //s->setTexture(TextureManager::getInstance().getTexture(texturePath), true);
}

void PSprite::setTextureRect(sf::IntRect rect)
{
    SPDLOG_TRACE("Set texture rect of {} to {} {} {} {}", texturePath, rect.position.x, rect.position.y, rect.size.x, rect.size.y);
    s->setTextureRect(rect);
}

sf::IntRect PSprite::getTextureRect()
{
    return s->getTextureRect();
}

void PSprite::setOrigin(float x, float y)
{
    if (x != orX || y != orY)
    {
        //SPDLOG_DEBUG("Change origin of {} to {} {}", texturePath, x, y);
        orX = x;
        orY = y;
        s->setOrigin(sf::Vector2f(orX, orY));
    }
}

void PSprite::setScale(float x, float y)
{
    if (x != scaleX || y != scaleY)
    {
        SPDLOG_TRACE("Change scale of {} to {} {}", texturePath, x, y);
        scaleX = x;
        scaleY = y;
        s->setScale(sf::Vector2f(ratioX * scaleX, ratioY * scaleY));
    }
}

void PSprite::setRotation(float a)
{
    if (a != angle)
    {
        SPDLOG_TRACE("Change rotation of {} to {}", texturePath, a);
        angle = a;
    }
}

void PSprite::setColor(sf::Color color)
{
    if (color != s->getColor())
    {
        SPDLOG_TRACE("Change color of {} to {} {} {} {}", texturePath, color.r, color.g, color.b, color.a);
        s->setColor(color);
    }
}

sf::Color PSprite::getColor()
{
    return s->getColor();
}

void PSprite::setTexture(sf::Texture& texture)
{
    s->setTexture(texture, true);
    exported = false;
}

void PSprite::applyTexture()
{
    //s->setTexture(t, true);
    exported = false;
}

//void PSprite::setSprite(sf::Sprite& sprite)
//{
//    SPDLOG_ERROR("PSPRITE TRIGGER");
    //s = sprite;
//}

void PSprite::setPosition(float x, float y)
{
    if (x != lx || y != ly)
    {
        SPDLOG_TRACE("Change position of {} to {} {}", texturePath, x, y);

        //s->setPosition(x*ratioX,y*ratioY);
        if (baseX == -999)
            baseX = x;

        if (baseY == -999)
            baseY = y;

        lx = x;
        ly = y;

        //std::cout << x << " " << y << "  " << lx << " " << ly << std::endl;
    }
}

sf::Vector2f PSprite::getPosition()
{
    return sf::Vector2f(lx, ly);
}

void PSprite::setScale(float ss)
{
    scaleX = ss;
    scaleY = ss;
    s->setScale(sf::Vector2f(ratioX * scaleX, ratioY * scaleY));
}

sf::FloatRect PSprite::getLocalBounds()
{
    sf::FloatRect rect = s->getLocalBounds();
    SPDLOG_TRACE("Returning local bounds of {}: {} {} {} {}", texturePath, rect.position.y, rect.position.x, rect.size.x, rect.size.y);
    return rect;
}

sf::FloatRect PSprite::getGlobalBounds()
{
    sf::FloatRect rect = s->getGlobalBounds();
    SPDLOG_TRACE("Returning global bounds of {}: {} {} {} {}", texturePath, rect.position.y, rect.position.x, rect.size.x, rect.size.y);
    return rect;
}

sf::FloatRect PSprite::getGlobalBoundsScaled()
{
    float nw = 1;
    float nh = 1;

    if (s->getGlobalBounds().size.x > 0)
        nw = s->getGlobalBounds().size.x / CoreManager::getInstance().getCore()->resRatio;

    if (s->getGlobalBounds().size.y > 0)
        nh = s->getGlobalBounds().size.y / CoreManager::getInstance().getCore()->resRatio;

    return sf::FloatRect(sf::Vector2f(s->getGlobalBounds().position.x, s->getGlobalBounds().position.y), sf::Vector2f(nw, nh));
    //return s->getGlobalBounds();
}

void PSprite::setSmooth(bool smooth)
{
    TextureManager::getInstance().getTexture(texturePath).setSmooth(smooth);
    s->setTexture(TextureManager::getInstance().getTexture(texturePath), true);
}

void PSprite::drawShader(sf::RenderWindow* window, sf::Shader& shader)
{
    qualitySetting = ResourceManager::getInstance().getCurrentQuality();

    if (oldQualitySetting != qualitySetting)
    {
        sf::Vector2u windowSize = window->getSize();

        std::vector<float> x = {640, 1280, 1920, 3840};
        std::vector<float> y = {360, 720, 1080, 2160};

        sf::Vector2f ratio(windowSize.x / x[qualitySetting], windowSize.y / y[qualitySetting]);

        ratioX = ratio.x;
        ratioY = ratio.y;
    }

    if (oldResSetting != resSetting)
    {
        sf::Vector2u windowSize = window->getSize();

        std::vector<float> x = {640, 1280, 1920, 3840};
        std::vector<float> y = {360, 720, 1080, 2160};

        sf::Vector2f ratio(windowSize.x / x[resSetting], windowSize.y / y[resSetting]);
    }

    s->setTexture(TextureManager::getInstance().getTexture(texturePath));
    s->setScale(sf::Vector2f(ratioX * scaleX, ratioY * scaleY));
    s->setOrigin(sf::Vector2f(orX, orY));
    s->setPosition(sf::Vector2f(lx * CoreManager::getInstance().getCore()->resRatio, ly * CoreManager::getInstance().getCore()->resRatio));
    s->setRotation(sf::degrees(angle));

    window->draw(*s,&shader);

    oldQualitySetting = qualitySetting;
    oldResSetting = resSetting;
}

void PSprite::draw()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();

    qualitySetting = ResourceManager::getInstance().getCurrentQuality();

    if (oldQualitySetting != qualitySetting)
    {
        sf::Vector2u windowSize = window->getSize();

        std::vector<float> x = {640, 1280, 1920, 3840};
        std::vector<float> y = {360, 720, 1080, 2160};

        sf::Vector2f ratio(windowSize.x / x[qualitySetting], windowSize.y / y[qualitySetting]);

        ratioX = ratio.x;
        ratioY = ratio.y;
    }

    if (oldResSetting != resSetting)
    {
        sf::Vector2u windowSize = window->getSize();

        std::vector<float> x = {640, 1280, 1920, 3840};
        std::vector<float> y = {360, 720, 1080, 2160};

        sf::Vector2f ratio(windowSize.x / x[resSetting], windowSize.y / y[resSetting]);
    }

    s->setTexture(TextureManager::getInstance().getTexture(texturePath));
    s->setScale(sf::Vector2f(ratioX * scaleX, ratioY * scaleY));
    s->setOrigin(sf::Vector2f(orX, orY));
    s->setPosition(sf::Vector2f(lx * CoreManager::getInstance().getCore()->resRatio, ly * CoreManager::getInstance().getCore()->resRatio));
    s->setRotation(sf::degrees(angle));
    window->draw(*s);

    oldQualitySetting = qualitySetting;
    oldResSetting = resSetting;
}

void PSprite::update(sf::RenderWindow& window)
{
    ///Update updates the sprite without drawing it on screen. Useful for RenderTexture
    s->setTexture(TextureManager::getInstance().getTexture(texturePath));
    s->setScale(sf::Vector2f(ratioX * scaleX, ratioY * scaleY));
    s->setOrigin(sf::Vector2f(orX, orY));
    s->setPosition(sf::Vector2f(lx * CoreManager::getInstance().getCore()->resRatio, ly * CoreManager::getInstance().getCore()->resRatio));
    s->setRotation(sf::degrees(angle));
}

void saveToFile(std::string file)
{
}
