#ifndef MISSIONCONTROLLER_H
#define MISSIONCONTROLLER_H

#include <SFML/Graphics.hpp>

#include "../Rhythm/Rhythm.h"
#include "Background.h"
#include "Camera.h"
#include "../Config.h"
#include "Units/Patapon.h"
#include "Units/Kacheek.h"
#include "Units/AnimatedObject.h"
#include "Units/CollidableObject.h"
#include "Units/Wall.h"
#include <string>
#include <thread>

class MissionController
{
    public:
    Background test_bg;
    Rhythm rhythm;
    bool isInitialized=false;;
    Patapon patapon;
    Kacheek kacheek;
    Kacheek kacheek2;
    Kacheek kacheek3;

    Camera camera;
    std::map<int,bool>* missionKeyMap;
    Config* missionConfig;

    /// this is a list of things in the level that
    /// we need to check against for collision (but not always damage)
    ///
    /// TODO: rename/refactor wall into tangibleObject class
    std::vector<CollidableObject*> tangibleLevelObjects;

    float pataponY = 200; ///temp
    float wallY = 200; ///temp

    void StopMission();
    void Initialise(Config &config, std::map<int,bool> &keymap,std::string backgroundName);
    void StartMission(std::string songName);
    void Update(sf::RenderWindow &window, float fps);


    MissionController();
    ~MissionController();
};


#endif // CAMERA_H
