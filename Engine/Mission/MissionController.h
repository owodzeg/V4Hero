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
#include "Units/Projectile.h"
#include "Units/CollidableObject.h"
#include "Units/Wall.h"
#include <string>
#include <thread>
#include "Units/EndFlag.h"
#include "Units/FeverWorm.h"

class MissionController
{
    public:
    Background test_bg;
    Rhythm rhythm;
    bool isInitialized=false;;
    bool isFinishedLoading=false;
    Patapon* patapon;
    Kacheek* kacheek;
    Kacheek* kacheek2;
    Kacheek* kacheek3;
    EndFlag* endFlag1;
    FeverWorm* feverworm;
        sf::Time startTime;
        sf::Clock missionTimer;

    sf::Text t_timerMenu;
    Camera camera;
    std::map<int,bool>* missionKeyMap;
    Config* missionConfig;

    sf::Font f_font;
    /// Things for the cutscenes
        std::vector<sf::Text> t_cutscene_text;
        int startAlpha;
        int endAlpha;
        sf::Time targetTime;
        sf::Clock timer;
        sf::RectangleShape fade;
        bool inCutscene;
        bool inFadeTransition;
        bool isBlackScreenCutscene;
        bool cutscenesLeft=false;
        bool showTimer=false;
        int currentCutsceneId;
        std::vector<std::wstring> cutscene_text_identifiers;
        std::vector<int> cutscene_lengths;
        std::vector<bool> cutscene_blackscreens;
    /// this is a list of things in the level that
    /// we need to check against for collision (but not always damage)
    std::vector<CollidableObject*> tangibleLevelObjects;
    std::vector<std::unique_ptr<Projectile>> levelProjectiles;

    float pataponY = 200; ///temp
    float wallY = 200; ///temp
    float gravity=981;
    float floorY=200;
    void StartCutscene(const std::wstring& text,bool isBlackScreen,int TimeToShow);

    void FinishLastCutscene();

    bool isMoreCutscenes();
    void StopMission();
    void Initialise(Config &config, std::map<int,bool> &keymap,std::string backgroundName);
    void StartMission(std::string songName,int missionID,bool showCutscene=false);
    void Update(sf::RenderWindow &window, float fps, std::map<int,bool> *keyMap);
    void DoMovement(sf::RenderWindow &window, float fps, std::map<int,bool> *keyMap);
    void DoKeyboardEvents(sf::RenderWindow &window, float fps, std::map<int,bool> *keyMap);


    MissionController();
    ~MissionController();
};


#endif // CAMERA_H
