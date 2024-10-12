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
#include "Units/Unit/EndFlag.h"
#include "Units/Unit/DroppedItem.h"
#include "Units/Projectile.h"
#include <Dialog/DialogBox.h>

using json = nlohmann::json;

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
    std::vector<std::unique_ptr<EndFlag>> endflags;
    std::vector<std::unique_ptr<Projectile>> projectiles;
    std::vector<std::unique_ptr<DroppedItem>> droppeditems;
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

    bool debug = false;
    int kirajin_hp = 200;

    bool inFever = false;
    bool zoomActivated = false;

    PText unit_count, unit_count_shadow;

    std::vector<std::vector<int>> dropped_ids;
    std::vector<PataDialogBox> dialogboxes;

    sf::RectangleShape fade_box;
    float fade_alpha = 0;

    sf::RectangleShape fadeout_box;
    float fadeout_alpha = 0;

    sf::Clock missionEndTimer;
    bool playJingle = false;

    sf::SoundBuffer sb_win_jingle;
    sf::SoundBuffer sb_lose_jingle;
    sf::Sound s_jingle;

    sf::SoundBuffer sb_cheer1, sb_cheer2, sb_cheer3;
    sf::Sound s_cheer;
    bool playCheer[3];

    PSprite bar_win;
    PSprite bar_lose;

    PText t_win, t_lose;

    bool missionEnd = false;
    float textCurX = -3840;
    float barCurX = 3840*1.5;
    float textDestX = 1920;
    float barDestX = 1920;
    float textCurScale = 1;
    float textDestScale = 1;
    bool textBounce = false;

    bool failure = false;
    bool returnToPatapolis = false;

    MissionController();
    void LoadMission(const std::string& path);
    void SendProjectile(float x, float y, float hspeed, float vspeed, std::string prj_tex);
    void SendItemDrop(std::vector<int> order_id, float x, float y);
    void ExecuteZoom(float speed, float time);
    void ProcessDroppedItems();
    void DoMissionEnd();

    void Update();

    ~MissionController();
};


#endif
