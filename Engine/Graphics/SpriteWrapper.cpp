#include "SpriteWrapper.h"

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

    PSprite& spr = ResourceManager::getInstance().getSprite(path);
    l_bounds = spr.getLocalBounds();
    g_bounds = spr.getGlobalBounds();
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

void SpriteWrapper::setScale(sf::Vector2f sc)
{
    scale = sc;
}

void SpriteWrapper::setScale(float x, float y)
{
    scale = sf::Vector2f(x, y);
}

sf::FloatRect SpriteWrapper::getLocalBounds()
{
    return l_bounds;
}

sf::FloatRect SpriteWrapper::getGlobalBounds()
{
    return g_bounds;
}

void SpriteWrapper::setColor(sf::Color c)
{
    color = c;
}

void SpriteWrapper::draw()
{
    PSprite& spr = ResourceManager::getInstance().getSprite(spritePath);
    spr.setPosition(position.x, position.y);
    spr.setOrigin(origin.x, origin.y);
    spr.setScale(scale.x, scale.y);
    spr.setColor(color);
    spr.draw();
}