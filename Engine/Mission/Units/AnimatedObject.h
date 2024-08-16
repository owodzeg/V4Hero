#ifndef ANIMATEDOBJECT_H
#define ANIMATEDOBJECT_H

#include <SFML/Graphics.hpp>
#include "PNGAnimation.h"

// compatibility
#include "Hitbox.h"
#include "Object.h"
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

    void setAnimation(std::string& shortAnimName);
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

    // fallback compatibility stuff
    int entityID = 0;
    float fps = 60;
    std::vector<Hitbox> hitboxes;
    bool offbounds = false;
    sf::Rect<float> hitBox;
    int qualitySetting;
    std::string custom_img_key;
    float cur_pos,anim_end;
    bool ready_to_erase;
    bool worm_fever;
    float scaleX, scaleY;
    std::string current_animation;
    std::shared_ptr<std::vector<Object>> objects;
    bool force_origin_null = false;
    std::map<int, std::map<int, sf::Vector2f>> animation_origins;
    std::map<int, std::map<int, sf::IntRect>> animation_bounds;
    float curFrame = 0, current_frame = 0;
    float global_x, global_y;
    float local_x, local_y;
    int index;
    float framerate;
    void setAnimationSegment(std::string new_segment_name);
    void setAnimationSegment(std::string new_segment_name, bool force_start);
    std::string getAnimationSegment();
    float getAnimationPos();
    void setColor(sf::Color c);
    sf::Color getColor();
    void applyEquipment(std::vector<int> item_id, int slot, bool offhand = false);
    void setLoop(bool a);
};

#endif // ANIMATEDOBJECT_H
