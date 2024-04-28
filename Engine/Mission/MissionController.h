#ifndef MISSIONCONTROLLER_H
#define MISSIONCONTROLLER_H

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

class MissionController
{
public:
    bool initialized = false;
    bool loadingError = false;

    MissionController();

    void Update();

    ~MissionController();
};


#endif
