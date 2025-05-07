#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "AnimatedObject.h"

#include <CoreManager.h>
#include <cmath>
#include <spdlog/spdlog.h>
#include <fstream>
#include <Func.h>

AnimatedObject::AnimatedObject()
{

}

AnimatedObject::~AnimatedObject()
{

}

// movement
void AnimatedObject::setGlobalPosition(sf::Vector2f newPos)
{
    pos_global = newPos;
}

void AnimatedObject::setLocalPosition(sf::Vector2f newPos)
{
    pos_local = newPos;
}

void AnimatedObject::moveGlobalPosition(sf::Vector2f moveBy)
{
    pos_global = sf::Vector2f(pos_global.x + moveBy.x, pos_global.y + moveBy.y);
}

void AnimatedObject::moveLocalPosition(sf::Vector2f moveBy)
{
    pos_local = sf::Vector2f(pos_local.x + moveBy.x, pos_local.y + moveBy.y);
}

sf::Vector2f AnimatedObject::getGlobalPosition()
{
    return pos_global;
}

sf::Vector2f AnimatedObject::getLocalPosition()
{
    return pos_local;
}

// transformation
void AnimatedObject::setRotation(float newRotation)
{
    rotation = newRotation;
}

void AnimatedObject::rotateBy(float degrees)
{
    rotation += degrees;
}

float AnimatedObject::getRotation()
{
    return rotation;
}

void AnimatedObject::setScale(sf::Vector2f newScale)
{
    scale = newScale;
}

void AnimatedObject::moveScale(sf::Vector2f moveBy)
{
    scale = sf::Vector2f(scale.x + moveBy.x, scale.y + moveBy.y);
}

sf::Vector2f AnimatedObject::getScale()
{
    return scale;
}

void AnimatedObject::setColor(sf::Color newColor)
{
    color = newColor;
}

sf::Color AnimatedObject::getColor()
{
    return color;
}

// animation control
void AnimatedObject::playAnimation()
{
    animation.isPlaying = true;
}

void AnimatedObject::stopAnimation()
{
    animation.isPlaying = false;
}

void AnimatedObject::restartAnimation()
{
    animation.currentFrame = 0;
}

void AnimatedObject::setAnimation(const std::string& shortAnimName)
{
    animation.setAnimation(shortAnimName);
}

void AnimatedObject::setAnimationSpeed(float newFPS)
{
    animation.animationSpeed = newFPS;
}

void AnimatedObject::setAnimationFrame(float newFrame)
{
    animation.currentFrame = newFrame;
}

std::string AnimatedObject::getAnimation()
{
    return animation.getAnimation();
}

float AnimatedObject::getAnimationSpeed()
{
    return animation.animationSpeed;
}

int AnimatedObject::getAnimationFrame()
{
    return static_cast<unsigned int>(floor(animation.currentFrame));
}

void AnimatedObject::setAnimationLoop(bool newLoop)
{
    animation.isLooping = newLoop;
}

bool AnimatedObject::getAnimationLoop()
{
    return animation.isLooping;
}

void AnimatedObject::addExtra(SpriteWrapper spr, std::string name)
{
    std::pair<std::string, SpriteWrapper> entry;
    entry.first = name;
    entry.second = spr;
    animation.extra.push_back(entry);
}

void AnimatedObject::loadExtra(std::string path, std::string name)
{
    SPDLOG_DEBUG("Loading extra! {} {}", path, name);

    int qualitySetting = CoreManager::getInstance().getConfig()->GetInt("textureQuality");
    float qscale = 1;

    switch (qualitySetting)
    {
        case 0: {
            qscale = 6;
            break;
        }
        case 1: {
            qscale = 3;
            break;
        }
        case 2: {
            qscale = 2;
            break;
        }
    }

    for(auto& a : animation.extra)
    {
        if(a.first == name)
        {
            std::string texPath = "resources/graphics/item/textures/"+path+".png";
            std::string alignPath = "resources/graphics/item/alignment/"+path+".spr";

            SPDLOG_DEBUG("texPath: {}, alignPath: {}", texPath, alignPath);

            a.second.load(texPath);

            
            

            std::ifstream alignFile(alignPath);
            if(alignFile.good())
            {
                std::string alignData;
                std::getline(alignFile, alignData);

                std::vector<std::string> data = Func::Split(alignData, ',');

                float o_x = stof(data[0]);
                float o_y = stof(data[1]);

                a.second.setOrigin(o_x / qscale, o_y / qscale);
            }
            else
            {
                a.second.setOrigin(0, 0);
            }

            SPDLOG_DEBUG("set origin to {} {}", a.second.getOrigin().x, a.second.getOrigin().y);
        }
    }
}

void AnimatedObject::LoadConfig(const std::string& anim_path)
{
    if(anim_path.find(".p4a") != std::string::npos)
    {
        SPDLOG_ERROR("Legacy P4A format found. These animations are unsupported.");
        SPDLOG_ERROR("Aborting while trying to read {}", anim_path);

        throw AnimatedObjectException("Invalid file loaded.");
    }

    try
    {
        animation.Load(anim_path);
    }
    catch(PNGAnimationException& exception)
    {
        SPDLOG_ERROR("An exception occured when trying to load PNGAnimation: {}", exception.what());
        throw AnimatedObjectException(exception.what());
    }
    catch(std::exception& exception)
    {
        SPDLOG_ERROR("A generic exception occured when trying to load PNGAnimation: {}", exception.what());
        throw AnimatedObjectException(exception.what());
    }
    catch(...)
    {
        SPDLOG_ERROR("An unknown exception occurred.");
        throw AnimatedObjectException("An unknown exception occurred.");
    }
}

void AnimatedObject::Draw()
{
    animation.position = sf::Vector2f(pos_global.x + pos_local.x, pos_global.y + pos_local.y);
    animation.scale = scale;
    animation.rotation = rotation;
    animation.color = color;
    animation.Draw();
}

void AnimatedObject::Update()
{

}