#ifndef MISSIONCONTROLLER_H
#define MISSIONCONTROLLER_H

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include "Background.h"
#include "Camera.h"
#include "Units/Entity.h"
#include "Units/Unit/Hatapon.h"
#include "Units/Unit/Yaripon.h"
#include "Units/Unit/FeverWorm.h"
#include "Units/Projectile.h"

class MissionController
{
public:
    bool initialized = false;
    bool loadingError = false;
    Background bg;
    Camera cam;

    sf::View mission_view;

    std::vector<std::unique_ptr<Hatapon>> hatapons;
    std::vector<std::unique_ptr<Yaripon>> yaripons;
    std::vector<std::unique_ptr<FeverWorm>> feverworms;
    std::vector<std::unique_ptr<Projectile>> projectiles;
    std::vector<std::unique_ptr<Entity>> entities;

    uint64_t lastRhythmCheck;
    sf::Clock advanceClock;
    bool advance = false;

    float followPoint = 340;
    float pataSpeed = 0;
    float pataMaxSpeed = 200;
    float pataCurMaxSpeed = pataMaxSpeed;
    float accelerationFactor = 1.5;
    float decelerationFactor = 2.5;

    bool debug = true;
    int kirajin_hp = 200;

    bool inFever = false;
    bool zoomActivated = false;

    PText unit_count, unit_count_shadow;

    MissionController();
    void LoadMission(const std::string& path);
    void SendProjectile(float x, float y, float hspeed, float vspeed);
    void ExecuteZoom(float speed, float time);

    void Update();

    ~MissionController();
};


#endif
