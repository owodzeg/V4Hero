#ifndef WEAPON_H
#define WEAPON_H
#include <SFML/Graphics.hpp>
#include "Item.h"
class Weapon : public Item
{
    public:
        int mindmg=0;
        int maxdmg=1;
        int crit=0;
        float attackspeed=0.0;/// 0% reduction
        static Weapon* FromItem(Item* item);
};

#endif // WEAPON_H
