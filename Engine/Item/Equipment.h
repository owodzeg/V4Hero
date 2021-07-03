#ifndef EQUIPMENT_H
#define EQUIPMENT_H
#include "Equipment.h"
#include <SFML/Graphics.hpp>

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
