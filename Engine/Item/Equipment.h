#ifndef EQUIPMENT_H
#define EQUIPMENT_H
#include <SFML/Graphics.hpp>
#include "Equipment.h"

class Equipment
{
    public:
        int hp = 0;
        int min_dmg = 0;
        int max_dmg = 0;
        int crit = 0;
        float attack_speed = 0.0; // 0% reduction
};

#endif // EQUIPMENT_H
