#ifndef EQUIPMENT_H
#define EQUIPMENT_H
#include "Equipment.h"
#include <SFML/Graphics.hpp>

class Equipment
{
public:
    float hp = 0.f;
    float min_dmg = 0.f;
    float max_dmg = 0.f;
    float crit = 0.f;
    float attack_speed = 0.f; // 0% reduction
};

#endif // EQUIPMENT_H
