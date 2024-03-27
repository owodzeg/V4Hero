#ifndef HITBOX_H
#define HITBOX_H
#include "HitboxFrame.h"
#include <SFML/Graphics.hpp>

///Class for managing object hitboxes defined by V4Anim format

class Hitbox
{
public:
    float g_x; ///global x pos
    float g_y; ///global y pos
    HitboxFrame hitboxObject;

    ///Data passed from AnimatedObject
    float o_x, o_y, o_width, o_height;

    /// frame should no longer be needed: use HitboxFrame instead
    /*struct Frame
    {
        ///timestamp
        float time;

        ///global position, so the hitbox can be moved around as a whole
        float g_x;
        float g_y;

        ///hitboxes' rect that react with other collidables
        float x;
        float y;
        float width;
        float height;
    };*/

    std::vector<HitboxFrame> frames; ///frames

    Hitbox();
    void SetFrame(float time);
    void SetCustomFrame(float in_time, float in_gx, float in_gy, float in_x, float in_y, float angle, float in_width, float in_height);
    void SetPos(float time);
    HitboxFrame getRect();
    sf::Vector2f getGlobalPosition();
    ~Hitbox();
};

#endif // HITBOX_H
