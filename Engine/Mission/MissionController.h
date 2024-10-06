#ifndef MISSIONCONTROLLER_H
#define MISSIONCONTROLLER_H

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include "Background.h"
#include "Camera.h"
#include "Units/AnimatedObject.h"

class MissionController
{
public:
    bool initialized = false;
    bool loadingError = false;
    Background bg;
    Camera cam;

    AnimatedObject hatapon;

    uint64_t lastRhythmCheck;
    sf::Clock advanceClock;
    bool advance = false;

    float followPoint = 340;
    float pataSpeed = 0;
    float pataMaxSpeed = 400;
    float accelerationFactor = 1.0;
    float decelerationFactor = 2.5;

    MissionController();

    void Update();

    ~MissionController();
};


#endif
