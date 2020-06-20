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
#include "Units/PlayableUnit.h"
#include "Units/Wall.h"
#include <string>
#include <thread>
#include "Units/EndFlag.h"
#include "Units/FeverWorm.h"
#include "Units/Hatapon.h"
#include "Units/HitboxFrame.h"
#include "../Graphics/PSpritesheet.h"

class V4Core;
class MissionController
{
    public:
    Background test_bg;
    Rhythm rhythm;
    bool isInitialized=false;
    bool isFinishedLoading=false;

    sf::Time startTime;
    sf::Clock missionTimer;

    sf::Text t_timerMenu;
    Camera camera;
    std::map<int,bool>* missionKeyMap;
    Config* missionConfig;
    V4Core* v4core;

    PSprite s_proj;
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


    std::vector<std::unique_ptr<PlayableUnit>> units; ///temporary until a better solution is possible
    std::vector<std::unique_ptr<CollidableObject>> tangibleLevelObjects;
    std::vector<std::unique_ptr<Projectile>> levelProjectiles;

    float pataponY = 600; ///temp
    float wallY = 200; ///temp
    float gravity=981;
    float floorY=200;

    float army_X = 800;

    struct DamageCounter
    {
        int type=0; ///0 - regular, 1 - high, 2 - crit
        int damage=0; ///damage to display
        sf::Clock display_timer; ///timer for displaying later digits

        ///vectors containing texture, pos and scale for every digit
        vector<PSprite> spr;
        vector<sf::Vector2f> pos;
        vector<float> scale;
        vector<float> scale_goal;
        vector<float> alpha;
        vector<bool> mode;
    };

    struct DroppedItem
    {
        int itemID=0; ///id of item
        float x=0,y=0,scale=1; ///position and scale of the item
        sf::Sprite spr; ///sprite to display the item
    };

    PSpritesheet dmg_spritesheet;
    PSpritesheet item_spritesheet;

    std::vector<DamageCounter> dmgCounters;
    std::vector<DroppedItem> droppedItems;

    void addDmgCounter(int type, int damage, float baseX, float baseY);
    void addItemsCounter(int id, float baseX, float baseY);

    void StartCutscene(const std::wstring& text,bool isBlackScreen,int TimeToShow);

    void FinishLastCutscene();
    bool DoCollisionStepInAxis(float currentAxisAngle, HitboxFrame* currentHitboxFrame,AnimatedObject* targetObject, HitboxFrame* currentObjectHitBoxFrame,float currentObjectX,float CurrentObjectY);
    bool DoCollisionForObject(HitboxFrame* currentObjectHitBoxFrame,float currentObjectX,float CurrentObjectY,int collisionObjectID,vector<string> collisionData = {});
    bool isMoreCutscenes();
    void StopMission();
    void Initialise(Config &config, std::map<int,bool> &keymap,std::string backgroundName,V4Core &v4core_);
    void StartMission(std::string songName,int missionID,bool showCutscene=false);
    void Update(sf::RenderWindow &window, float fps, std::map<int,bool> *keyMap,std::map<int,bool> *keyMapHeld);
    void DoMovement(sf::RenderWindow &window, float fps, std::map<int,bool> *keyMap,std::map<int,bool> *keyMapHeld);
    void DoKeyboardEvents(sf::RenderWindow &window, float fps, std::map<int,bool> *keyMap,std::map<int,bool> *keyMapHeld);
    float pataponMaxProjection(float axisAngle, int id);

    float pataponMinProjection(float axisAngle, int id);


    MissionController();
    ~MissionController();
};


#endif // CAMERA_H
