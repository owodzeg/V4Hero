#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "PSprite.h"
#include "../TextureManager.h"
#include "../CoreManager.h"
#include <fstream>
#include <iostream>
#include <string>
#include <spdlog/spdlog.h>

PSprite::PSprite() 
{
}

void PSprite::loadFromFile(std::string file, int q)
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
    resSetting = 1;

    SPDLOG_INFO("Loading PSprite: {}", file);
    texturePath = file;

    s.setTexture(TextureManager::getInstance().getTexture(file, q), true);
}

void PSprite::loadFromFile(std::string file, int q, int r = 1)
{
    qualitySetting = q;
    resSetting = r;

    SPDLOG_INFO("Loading PSprite: {}", file);
    texturePath = file;

    s.setTexture(TextureManager::getInstance().getTexture(file, q), true);
}

void PSprite::setRepeated(bool r)
{
    SPDLOG_TRACE("Change repeated state of {} to {}", texturePath, r);
    TextureManager::getInstance().getTexture(texturePath).setRepeated(r);
    //s.setTexture(TextureManager::getInstance().getTexture(texturePath), true);
}

void PSprite::setTextureRect(sf::IntRect rect)
{
    SPDLOG_TRACE("Set texture rect of {} to {} {} {} {}", texturePath, rect.top, rect.left, rect.width, rect.height);
    s.setTextureRect(rect);
}

sf::IntRect PSprite::getTextureRect()
{
    return s.getTextureRect();
}

void PSprite::setOrigin(float x, float y)
{
    if (x != orX || y != orY)
    {
        SPDLOG_DEBUG("Change origin of {} to {} {}", texturePath, x, y);
        orX = x;
        orY = y;
        s.setOrigin(orX, orY);
    }
}

void PSprite::setScale(float x, float y)
{
    if (x != scaleX || y != scaleY)
    {
        SPDLOG_TRACE("Change scale of {} to {} {}", texturePath, x, y);
        scaleX = x;
        scaleY = y;
        s.setScale(ratioX * scaleX, ratioY * scaleY);
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
    if (color != s.getColor())
    {
        SPDLOG_TRACE("Change color of {} to {} {} {} {}", texturePath, color.r, color.g, color.b, color.a);
        s.setColor(color);
    }
}

sf::Color PSprite::getColor()
{
    return s.getColor();
}

void PSprite::setTexture(sf::Texture& texture)
{
    s.setTexture(texture, true);
    exported = false;
}

void PSprite::applyTexture()
{
    //s.setTexture(t, true);
    exported = false;
}

void PSprite::setSprite(sf::Sprite& sprite)
{
    s = sprite;
}

void PSprite::setPosition(float x, float y)
{
    if (x != lx || y != ly)
    {
        SPDLOG_TRACE("Change position of {} to {} {}", texturePath, x, y);

        //s.setPosition(x*ratioX,y*ratioY);
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
    s.setScale(ratioX * scaleX, ratioY * scaleY);
}

sf::FloatRect PSprite::getLocalBounds()
{
    sf::FloatRect rect = s.getLocalBounds();
    SPDLOG_TRACE("Returning local bounds of {}: {} {} {} {}", texturePath, rect.top, rect.left, rect.width, rect.height);
    return rect;
}

sf::FloatRect PSprite::getGlobalBounds()
{
    sf::FloatRect rect = s.getGlobalBounds();
    SPDLOG_TRACE("Returning global bounds of {}: {} {} {} {}", texturePath, rect.top, rect.left, rect.width, rect.height);
    return rect;
}

sf::FloatRect PSprite::getGlobalBoundsScaled()
{
    float nw = 1;
    float nh = 1;

    if (s.getGlobalBounds().width > 0)
        nw = s.getGlobalBounds().width / resRatioX;

    if (s.getGlobalBounds().height > 0)
        nh = s.getGlobalBounds().height / resRatioY;

    return sf::FloatRect(s.getGlobalBounds().left, s.getGlobalBounds().top, nw, nh);
    //return s.getGlobalBounds();
}

void PSprite::setSmooth(bool smooth)
{
    TextureManager::getInstance().getTexture(texturePath).setSmooth(smooth);
    s.setTexture(TextureManager::getInstance().getTexture(texturePath), true);
}

void PSprite::draw(sf::RenderWindow& window)
{
    switch (qualitySetting)
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

    switch (resSetting)
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

    //s.setTexture(t);
    s.setScale(ratioX * scaleX, ratioY * scaleY);
    s.setOrigin(orX, orY);
    s.setPosition(lx * resRatioX, ly * resRatioY);
    s.setRotation(angle * (180 / 3.14159265358));
    window.draw(s);

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) && (sf::Keyboard::isKeyPressed(sf::Keyboard::F9)))
    {
        if (!exported)
        {
                sf::Image img;
                img = s.getTexture()->copyToImage();
                int rrr = rand() % 100000000;
                img.saveToFile("texDump/" + std::to_string(rrr) + ".png");

                exported = true;
        }
    }
}

void PSprite::draw(sf::RenderWindow* window)
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

        resRatioX = ratio.x;
        resRatioY = ratio.y;
    }

    s.setScale(ratioX * scaleX, ratioY * scaleY);
    s.setOrigin(orX, orY);
    s.setPosition(lx * resRatioX, ly * resRatioY);
    s.setRotation(angle * (180 / 3.14159265358));
    window->draw(s);

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

        resRatioX = ratio.x;
        resRatioY = ratio.y;
    }

    s.setScale(ratioX * scaleX, ratioY * scaleY);
    s.setOrigin(orX, orY);
    s.setPosition(lx * resRatioX, ly * resRatioY);
    s.setRotation(angle * (180 / 3.14159265358));
    window->draw(s);

    oldQualitySetting = qualitySetting;
    oldResSetting = resSetting;
}

void PSprite::update(sf::RenderWindow& window)
{
    ///Update updates the sprite without drawing it on screen. Useful for RenderTexture

    switch (qualitySetting)
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

    switch (resSetting)
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

    //s.setTexture(t);
    s.setScale(ratioX * scaleX, ratioY * scaleY);
    s.setOrigin(orX, orY);
    s.setPosition(lx * resRatioX, ly * resRatioY);
    s.setRotation(angle * (180 / 3.14159265358));

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) && (sf::Keyboard::isKeyPressed(sf::Keyboard::F9)))
    {
        if (!exported)
        {
            sf::Image img;
            img = s.getTexture()->copyToImage();
            int rrr = rand() % 100000000;
            img.saveToFile("texDump/" + std::to_string(rrr) + ".png");

            exported = true;
        }
    }
}

void saveToFile(std::string file)
{
}
