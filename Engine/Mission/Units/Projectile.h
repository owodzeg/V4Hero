#ifndef PROJECTILE_H
#define PROJECTILE_H
#include <SFML/Graphics.hpp>
#include "CollidableObject.h"
#include "../../Graphics/PSprite.h"
class Projectile
{
    public:
        int mindmg=0;
        int maxdmg=0; /// by default, masks add/deal no damage
        int crit=0;
        float attackspeed=0;/// 0% reduction
        float speed;
        float angle;
        float xPos;
        float yPos;

        PSprite* sprite;
        Projectile(PSprite& tsprite);

        float GetXSpeed();
        float GetYSpeed();
        void SetNewSpeedVector(float xSpeed, float ySpeed);
        virtual void OnCollide(CollidableObject* otherObject);
        virtual void Update(sf::RenderWindow &window, float fps);
        virtual void Draw(sf::RenderWindow &window, float fps);
};

#endif // PROJECTILE_H
