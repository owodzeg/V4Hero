#ifndef PROJECTILE_H
#define PROJECTILE_H
#include <SFML/Graphics.hpp>
class Projectile
{
    public:
        int mindmg=0;
        int maxdmg=0; /// by default, masks add/deal no damage
        int crit=0;
        float attackspeed=0;/// 0% reduction
        float speed;
        float angle;
};

#endif // PROJECTILE_H
