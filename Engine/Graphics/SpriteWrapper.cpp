#include "SpriteWrapper.h"

#include "../ResourceManager.h"
#include "../CoreManager.h"

/* IMPORTANT!!!!!!!
* LOCAL BOUNDS mean before transformations.
* GLOBAL BOUNDS mean everything past transformations, so including scaling, positioning, etc 
* i am dumb
  also take care of resRatio, if object is 800px on screen, we should take resRatio in account because on RR=1 it will be bigger than screen */

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
    /* sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    sf::Vector2u windowSize = window->getSize();

    std::vector<float> x = {640, 1280, 1920, 3840};
    std::vector<float> y = {360, 720, 1080, 2160};

    int quality = ResourceManager::getInstance().getCurrentQuality();

    sf::Vector2f ratio(windowSize.x / x[quality], windowSize.y / y[quality]);
    sf::Vector2f r((fabs(scale.x) * ratio.x), (fabs(scale.y) * ratio.y));

    return sf::FloatRect(l_bounds.position.y * r.x, l_bounds.position.x * r.y, l_bounds.width * r.x, l_bounds.height * r.y); */

    // lol 
    return l_bounds;
}

sf::FloatRect SpriteWrapper::getGlobalBounds()
{
    // turns out everything works fine if you force local bounds everywhere
    return l_bounds;
}

sf::FloatRect SpriteWrapper::getTransformedBounds()
{
    // for positioning purpose only, when something needs to depend on sprite's position
    // we dont want to use local bounds then, because we're taking a whole ass sprite
    // in hopes of explaining it to myself within next few months:
    // since we pretty much load everything in resratio=1, then when the window is huge, the transformed bounds will not be enough
    // totem on 1440p is 800px on screen, but our screen real estate is 720 pixels max. so we need to divide by resratio to bring it to the original 720p dimensions.
    // should change it later to default resratio=3 for more precise alignments

    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    sf::Vector2u windowSize = window->getSize();

    std::vector<float> x = {640, 1280, 1920, 3840};
    std::vector<float> y = {360, 720, 1080, 2160};

    sf::Vector2f resRatio(windowSize.x / float(3840), windowSize.y / float(2160));

    int quality = ResourceManager::getInstance().getCurrentQuality();

    sf::Vector2f ratio(windowSize.x / x[quality], windowSize.y / y[quality]);
    sf::Vector2f r((fabs(scale.x) * ratio.x / resRatio.x), (fabs(scale.y) * ratio.y / resRatio.y));

    return sf::FloatRect(sf::Vector2f(l_bounds.position.x * r.x, l_bounds.position.y * r.y), sf::Vector2f(l_bounds.size.x * r.x, l_bounds.size.y * r.y));
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