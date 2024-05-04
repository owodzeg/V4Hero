#ifndef MISSIONCONTROLLER_H
#define MISSIONCONTROLLER_H

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include "Background.h"
#include "Camera.h"

class MissionController
{
public:
    bool initialized = false;
    bool loadingError = false;
    Background bg;
    Camera cam;

    MissionController();

    void Update();

    ~MissionController();
};


#endif
