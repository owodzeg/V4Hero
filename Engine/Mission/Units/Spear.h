#ifndef SPEAR_H
#define SPEAR_H
#include <SFML/Graphics.hpp>
#include "Projectile.h"
#include "../../Item/Item.h"
class Spear : public Projectile
{
    public:
        static Spear* FromItem(Item* item);
};

#endif // SPEAR_H
