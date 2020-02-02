#ifndef SPEAR_H
#define SPEAR_H
#include <SFML/Graphics.hpp>
#include "Projectile.h"
#include "../../Item/Item.h"
#include "../../Graphics/PSprite.h"
class Spear : public Projectile
{
    public:
        static Spear* FromItem(Item* item,PSprite sprite);
        Spear(PSprite& tsprite);

};

#endif // SPEAR_H
