#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "../../Graphics/PSprite.h"
#include "CollidableObject.h"
#include <SFML/Graphics.hpp>
class Projectile
{
public:
    int projectile_id = 0;
    bool enemy = false;

    int min_dmg = 0;
    int max_dmg = 0; /// by default, masks add/deal no damage
    int crit = 0;
    float attack_speed = 0; /// 0% reduction
    float speed;
    float angle;
    float xPos;
    float yPos;

    ///temp implementation
    float hspeed;
    float vspeed;

    PSprite nsprite;
    PSprite* sprite;
    Projectile(PSprite& tsprite);

    float GetXSpeed();
    float GetYSpeed();
    void SetNewSpeedVector(float xSpeed, float ySpeed);
    virtual void OnCollide(CollidableObject* otherObject);
    virtual void Update();
    virtual void Draw();
};

#endif // PROJECTILE_H
