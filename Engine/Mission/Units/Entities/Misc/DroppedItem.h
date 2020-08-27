#ifndef DROPPEDITEM_H
#define DROPPEDITEM_H

#include <SFML/Graphics.hpp>
#include "../../../../Config.h"
#include "../../Entity.h"

using namespace std;

class DroppedItem : public Entity
{
    public:
    float hspeed=0;
    float vspeed=0;

    struct GlowCircle
    {
        sf::CircleShape circle;
        float alpha=64;
        float radius=20;
    };

    float resRatioX,resRatioY;

    vector<GlowCircle> glow;
    sf::Clock glowTimer;

    ///for the fun pickup animation
    float curXscale=1,curYscale=1;
    float destXscale=1,destYscale=1;
    float local_xPosDest = 0;
    float local_yPosDest = 0;
    float vspeed2 = 0;
    float hspeed2 = 0;
    float alpha = 255;
    int anim_state = 0;
    sf::Clock pickupClock;

    bool pickedup = false;

    ///picked item data
    string spritesheet;
    int spritesheet_id;
    int picked_item;

    DroppedItem();
    void LoadConfig(Config *thisConfigs);
    void Draw(sf::RenderWindow& window);
    void OnCollide(CollidableObject* otherObject, int collidedWith=-1, vector<string> collisionData = {});
};

#endif // DROPPEDITEM_H
