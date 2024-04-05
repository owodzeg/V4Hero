#ifndef TESTCHAMBER_H
#define TESTCHAMBER_H

#include <SFML/Graphics.hpp>
#include "../Mission/Units/PNGAnimation.h"

class TestChamber
{
public:
    bool initialized = false;
    sf::RectangleShape bg;

    PNGAnimation pngTest_1, pngTest_2, pngTest_3;

    void Initialize();
    void Update();
    TestChamber();
    ~TestChamber();
};


#endif
