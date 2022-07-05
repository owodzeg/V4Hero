#include "SpriteWrapper.h"

#include "../ResourceManager.h"
#include "../CoreManager.h"

/* IMPORTANT!!!!!!!
* when taking a position of something, we want it scaled, so we use local bounds
* when we set the origin, sfml scales the origin itself, so we use global bounds to prevent double scaling
* remember this forever please im so tired figuring out for the 100th time why something is misaligned
* thanks */

SpriteWrapper::SpriteWrapper()
{

}

void SpriteWrapper::load(std::string path)
{
    spritePath = path;
    position = sf::Vector2f(0, 0);
    origin = sf::Vector2f(0, 0);
    scale = sf::Vector2f(1, 1);
    color = sf::Color(255, 255, 255, 255);
    repeated = false;
    rotation = 0;

    PSprite& spr = ResourceManager::getInstance().getSprite(path);
    l_bounds = spr.getLocalBounds();
    g_bounds = spr.getGlobalBounds();
    texture_rect = spr.getTextureRect();
}

void SpriteWrapper::setPosition(sf::Vector2f pos)
{
    position = pos;
}

void SpriteWrapper::setPosition(float x, float y)
{
    position = sf::Vector2f(x,y);
}

sf::Vector2f SpriteWrapper::getPosition()
{
    return position;
}

void SpriteWrapper::setOrigin(sf::Vector2f ori)
{
    origin = ori;
}

void SpriteWrapper::setOrigin(float x, float y)
{
    origin = sf::Vector2f(x, y);
}

sf::Vector2f SpriteWrapper::getOrigin()
{
    return origin;
}

void SpriteWrapper::setScale(sf::Vector2f sc)
{
    scale = sc;
}

void SpriteWrapper::setScale(float x, float y)
{
    scale = sf::Vector2f(x, y);
}

void SpriteWrapper::setScale(float x)
{
    scale = sf::Vector2f(x, x);
}

sf::FloatRect SpriteWrapper::getLocalBounds()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    sf::Vector2u windowSize = window->getSize();

    std::vector<float> x = {640, 1280, 1920, 3840};
    std::vector<float> y = {360, 720, 1080, 2160};

    int quality = ResourceManager::getInstance().getCurrentQuality();

    sf::Vector2f ratio(windowSize.x / x[quality], windowSize.y / y[quality]);
    sf::Vector2f r((fabs(scale.x) * ratio.x), (fabs(scale.y) * ratio.y));

    return sf::FloatRect(l_bounds.top * r.x, l_bounds.left * r.y, l_bounds.width * r.x, l_bounds.height * r.y);
}

sf::FloatRect SpriteWrapper::getGlobalBounds()
{
    return g_bounds;
}

void SpriteWrapper::setColor(sf::Color c)
{
    color = c;
}

sf::IntRect SpriteWrapper::getTextureRect()
{
    return texture_rect;
}

void SpriteWrapper::setTextureRect(sf::IntRect rect)
{
    texture_rect = rect;
}

void SpriteWrapper::setRepeated(bool isRepeated)
{
    repeated = isRepeated;
}

void SpriteWrapper::setRotation(float angle)
{
    rotation = angle;
}

float SpriteWrapper::getRotation()
{
    return rotation;
}

void SpriteWrapper::draw()
{
    PSprite& spr = ResourceManager::getInstance().getSprite(spritePath);
    spr.setTextureRect(texture_rect);
    spr.setRepeated(repeated);
    spr.setPosition(position.x, position.y);
    spr.setOrigin(origin.x, origin.y);
    spr.setScale(scale.x, scale.y);
    spr.setRotation(rotation);
    spr.setColor(color);
    spr.draw();
}