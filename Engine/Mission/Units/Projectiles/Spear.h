#ifndef SPEAR_H
#define SPEAR_H
#include "../../../Graphics/PSprite.h"
#include "../../../Item/Item.h"
#include "../Projectile.h"
#include <SFML/Graphics.hpp>

class Spear : public Projectile
{
public:
    static Spear* FromItem(Item* item, PSprite sprite);
    Spear(PSprite& tsprite);
};

#endif // SPEAR_H
