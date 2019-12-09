#ifndef MASK_H
#define MASK_H
#include <SFML/Graphics.hpp>
#include "Item.h"
class Mask : public Item
{
    public:
        int mindmg=0;
        int maxdmg=0; /// by default, masks add/deal no damage
        int crit=0;
        float attackspeed=0;/// 0% reduction
        static Mask FromItem(Item item);
};

#endif // MASK_H
