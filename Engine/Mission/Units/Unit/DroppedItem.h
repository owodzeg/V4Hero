#ifndef DROPPEDITEM_H
#define DROPPEDITEM_H

#include <Mission/Units/AnimatedObject.h>

class DroppedItem
{
public:
    SpriteWrapper main;

    struct GlowCircle {
        sf::CircleShape circle;
        float alpha = 64;
        float radius = 20;
        float x=0,y=0;
    };

    std::vector<GlowCircle> glow;
    sf::Clock glowTimer;

    float vspeed=0, hspeed=0;

    int itemID = -1;
    std::string category = "main";
    std::vector<int> order_id;

    float local_x=0, local_y=0;
    float off_x=0, off_y=0;
    float global_x=0, global_y=0;

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
    bool ready_to_erase = false;

    DroppedItem(std::vector<int> item_id);
    void Collect();
    void Draw();
};

#endif