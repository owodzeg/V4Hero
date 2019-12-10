#ifndef ARMOUR_H
#define ARMOUR_H
#include <SFML/Graphics.hpp>
#include "Item.h"
class Armour : public Item
{
    public:
        int mindmg=0;
        int maxdmg=0;
        int crit=0;
        int hp=0;
        float attackspeed=0.0;/// 0% reduction
        static Armour FromItem(Item item);
};

#endif // ARMOUR_H
