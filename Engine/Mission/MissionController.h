#ifndef MISSIONCONTROLLER_H
#define MISSIONCONTROLLER_H

#include <SFML/Graphics.hpp>

#include "../Rhythm/Rhythm.h"
#include "Background.h"
#include "Camera.h"
#include "../Config.h"
#include "Patapon.h"
#include <string>
class MissionController
{
    public:
    Background test_bg;
    Rhythm rhythm;
    Patapon patapon;
    Camera camera;
    std::map<int,bool>* missionKeyMap;
    Config* missionConfig;

    void StopMission();
    void Initialise(Config &config, std::map<int,bool> &keymap,std::string backgroundName);
    void StartMission(std::string songName);
    void Update(sf::RenderWindow &window, float fps);
    MissionController();
    ~MissionController();
};


#endif // CAMERA_H
