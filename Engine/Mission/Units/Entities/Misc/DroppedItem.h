#ifndef DROPPEDITEM_H
#define DROPPEDITEM_H

#include "../../Entity.h"
#include <SFML/Graphics.hpp>

class DroppedItem : public Entity
{
public:
    float hspeed = 0;
    float vspeed = 0;

    struct GlowCircle {
        sf::CircleShape circle;
        float alpha = 64;
        float radius = 20;
    };

    float resRatioX, resRatioY;

    vector<GlowCircle> glow;
    sf::Clock glowTimer;

    ///for the fun pickup animation
    float curXscale = 1, curYscale = 1;
    float destXscale = 1, destYscale = 1;
    float local_xPosDest = 0;
    float local_yPosDest = 0;
    float vspeed2 = 0;
    float hspeed2 = 0;
    float alpha = 255;
    int anim_state = 0;
    sf::Clock pickupClock;

    bool pickedup = false;

    ///picked item data
    string item_group;
    int item_id;
    string picked_item;

    sf::SoundBuffer s_item, s_keyitem;

    DroppedItem();
    void LoadConfig();
    void Draw();
    void OnCollide(CollidableObject* otherObject, int collidedWith = -1, vector<string> collisionData = {});
};

#endif // DROPPEDITEM_H
