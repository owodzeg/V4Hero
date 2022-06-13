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

    PSprite& spr = ResourceManager::getInstance().getSprite(path);
    l_bounds = spr.getLocalBounds();
    g_bounds = spr.getGlobalBounds();
}

void SpriteWrapper::setPosition(sf::Vector2f pos)
{
    position = pos;
}

sf::Vector2f SpriteWrapper::getPosition()
{
    return position;
}

void SpriteWrapper::setOrigin(sf::Vector2f ori)
{
    origin = ori;
}

void SpriteWrapper::setScale(sf::Vector2f sc)
{
    scale = sc;
}

sf::FloatRect SpriteWrapper::getLocalBounds()
{
    return l_bounds;
}

sf::FloatRect SpriteWrapper::getGlobalBounds()
{
    return g_bounds;
}

void SpriteWrapper::draw()
{
    PSprite& spr = ResourceManager::getInstance().getSprite(spritePath);
    spr.setPosition(position.x, position.y);
    spr.setOrigin(origin.x, origin.y);
    spr.setScale(scale.x, scale.y);
    spr.draw();
}