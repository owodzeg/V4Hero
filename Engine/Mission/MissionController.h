#ifndef MISSIONCONTROLLER_H
#define MISSIONCONTROLLER_H

#include <SFML/Graphics.hpp>

#include "../Rhythm/Rhythm.h"
#include "Background.h"
#include "Camera.h"
#include "../Config.h"
#include "../Graphics/PText.h"
#include <string>
#include <thread>

#include "Units/EntityList.h"
#include "Units/PlayableUnitList.h"
#include "Units/ProjectileList.h"

#include "Units/HitboxFrame.h"
#include "Units/AnimatedObject.h"
#include "Units/Projectile.h"
#include "Units/CollidableObject.h"
#include "Units/PlayableUnit.h"
#include "../Graphics/PSpritesheet.h"

class V4Core;
class MissionController
{
    public:
    int qualitySetting=1, resSetting=1;

    Background test_bg;
    Rhythm rhythm;
    bool isInitialized=false;
    bool isFinishedLoading=false;

    sf::RectangleShape fade_box;
    float fade_alpha = 255;

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

    PText t_win,t_win_outline;
    PText t_lose,t_lose_outline;

    sf::Time startTime;
    sf::Clock missionTimer;

    sf::Text t_timerMenu;
    Camera camera;
    std::map<int,bool>* missionKeyMap;
    Config* missionConfig;
    V4Core* v4core;

    PSprite s_proj;
    sf::Font f_font;
    sf::Font f_moji;
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


    std::vector<std::unique_ptr<PlayableUnit>> units;
    std::vector<std::unique_ptr<Entity>> tangibleLevelObjects;
    std::vector<std::unique_ptr<Projectile>> levelProjectiles;

    float pataponY = 600; ///temp
    float wallY = 200; ///temp
    float gravity=981;
    float floorY=200;

    float army_X = 800;
    float army_X_dest = 800;

    float prevTime = 0;
    sf::Clock walkClock;
    bool startWalking = false;

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

    bool missionEnd = false;
    float textCurX = -1280;
    float barCurX = 1920;
    float textDestX = 640;
    float barDestX = 640;
    float textCurScale = 1;
    float textDestScale = 1;
    bool textBounce = false;

    float Smoothstep(float time);
    float Clamp(float x, float lowerlimit, float upperlimit);
    void addDmgCounter(int type, int damage, float baseX, float baseY, int q, int r);
    void addItemsCounter(int id, float baseX, float baseY);
    void spawnEntity(string entityName, int entityID, int baseX, int randX, int baseY, int spr_goal, int spr_range, int statLevel, sf::Color color, bool collidable, bool attackable);

    void StartCutscene(const std::wstring& text,bool isBlackScreen,int TimeToShow);

    void FinishLastCutscene();
    bool DoCollisionStepInAxis(float currentAxisAngle, HitboxFrame* currentHitboxFrame,AnimatedObject* targetObject, HitboxFrame* currentObjectHitBoxFrame,float currentObjectX,float CurrentObjectY);
    bool DoCollisionForObject(HitboxFrame* currentObjectHitBoxFrame,float currentObjectX,float CurrentObjectY,int collisionObjectID,vector<string> collisionData = {});
    bool isMoreCutscenes();
    void StopMission();
    void Initialise(Config &config, std::map<int,bool> &keymap,std::string backgroundName,V4Core &v4core_);
    void StartMission(std::string missionFile, bool showCutscene=false);
    void Update(sf::RenderWindow &window, float fps, std::map<int,bool> *keyMap,std::map<int,bool> *keyMapHeld);
    void DoMovement(sf::RenderWindow &window, float fps, std::map<int,bool> *keyMap,std::map<int,bool> *keyMapHeld);
    void DoKeyboardEvents(sf::RenderWindow &window, float fps, std::map<int,bool> *keyMap,std::map<int,bool> *keyMapHeld);
    float pataponMaxProjection(float axisAngle, int id);

    float pataponMinProjection(float axisAngle, int id);


    MissionController();
    ~MissionController();
};


#endif // CAMERA_H
