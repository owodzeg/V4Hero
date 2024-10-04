#ifndef ANIMATEDOBJECT_H
#define ANIMATEDOBJECT_H

#include <SFML/Graphics.hpp>
#include "../../Graphics/PNGAnimation.h"
#include <memory>

class AnimatedObjectException : public std::exception {
private:
    std::string message;

public:
    AnimatedObjectException(std::string msg) : message(msg) {}
    char* what () {
        return message.data();
    }
};

class AnimatedObject
{
public:
    PNGAnimation animation;

    sf::Vector2f pos_global = {0, 0}; // global positioning. general movement around the space
    sf::Vector2f pos_local = {0, 0}; // local positioning. a simple offset from global positioning
    sf::Vector2f scale = {1, 1};
    float rotation = 0;

    AnimatedObject();
    ~AnimatedObject();

    // movement
    void setGlobalPosition(sf::Vector2f newPos); // set a predefined global position
    void setLocalPosition(sf::Vector2f newPos); // set a predefined local position
    void moveGlobalPosition(sf::Vector2f moveBy); // move global position by xy
    void moveLocalPosition(sf::Vector2f moveBy); // move local position by xy
    sf::Vector2f getGlobalPosition();
    sf::Vector2f getLocalPosition();

    // transformation
    void setRotation(float newRotation); // set a predefined rotation
    void rotateBy(float degrees); // rotate by amount of degrees
    float getRotation();

    void setScale(sf::Vector2f newScale); // set a predefined scale
    void moveScale(sf::Vector2f moveBy); // increment scale by xy
    sf::Vector2f getScale();

    // animation control
    void playAnimation();
    void stopAnimation();

    void setAnimation(const std::string& shortAnimName);
    void setAnimationSpeed(float newFPS);
    void setAnimationFrame(int newFrame);

    std::string getAnimation();
    float getAnimationSpeed();
    int getAnimationFrame();

    void setAnimationLoop(bool newLoop);
    bool getAnimationLoop();


    // internals
    virtual void LoadConfig(const std::string& anim_path);
    virtual void Draw();
    virtual void Update();
};

#endif // ANIMATEDOBJECT_H
