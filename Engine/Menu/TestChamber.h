#ifndef TESTCHAMBER_H
#define TESTCHAMBER_H

#include <SFML/Graphics.hpp>
#include "../Mission/Units/AnimatedObject.h"

class TestChamber
{
public:
    bool initialized = false;
    sf::RectangleShape bg;

    AnimatedObject yaripon;
    AnimatedObject hatapon;
    AnimatedObject dropped_item;
    float fun = 0;

    void Initialize();
    void Update();
    TestChamber();
    ~TestChamber();
};


#endif
