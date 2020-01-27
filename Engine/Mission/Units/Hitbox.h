#ifndef HITBOX_H
#define HITBOX_H

#include <SFML/Graphics.hpp>

///Class for managing object hitboxes defined by V4Anim format

class Hitbox
{
    private:
    bool debug = false;

    public:
    float g_x; ///global x pos
    float g_y; ///global y pos
    sf::Rect<float> hitboxObject;

    struct Frame
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
    };

    std::vector<Frame> frames; ///frames

    Hitbox();
    void SetFrame(float time);
    void SetCustomFrame(float in_time, float in_gx, float in_gy, float in_x, float in_y, float in_width, float in_height);
    void SetPos(float time);
    sf::Rect<float> getRect();
    sf::Vector2f getGlobalPosition();
    ~Hitbox();
};

#endif // HITBOX_H
