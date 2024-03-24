#ifndef MISSIONCONTROLLER_H
#define MISSIONCONTROLLER_H

#include <SFML/Graphics.hpp>

#include "../Config.h"
#include "../Rhythm/Rhythm.h"
#include "Background.h"
#include "Camera.h"

#include "../Graphics/PSpritesheet.h"
#include "../Graphics/PText.h"

#include <random>
#include <string>
#include <thread>

#include "Units/EntityList.h"
#include "Units/PlayableUnitList.h"
#include "Units/ProjectileList.h"

#include "Units/AnimatedObject.h"
#include "Units/CollidableObject.h"
#include "Units/HitboxFrame.h"
#include "Units/PlayableUnit.h"
#include "Units/Projectile.h"

#include "../Dialog/ControlTips.h"
#include "../Dialog/DialogBox.h"
#include "../Dialog/MessageCloud.h"

#include <nlohmann/json.hpp>

class MissionController
{
public:
    int qualitySetting = 1, resSetting = 1;

    float fps = 60;

    ControlTips ctrlTips;

    bool showHitboxes = false;

    Background mission_bg;
    
    bool initialized = false;
    bool isFinishedLoading = false;
    bool loadingError = false;

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

    PText t_win, t_win_outline;
    PText t_lose, t_lose_outline;

    sf::Time startTime;
    sf::Clock missionTimer;

    sf::Text t_timerMenu;
    Camera camera;

    PSprite s_proj;
    sf::Font f_font;
    sf::Font f_moji;
    sf::Font f_unicode;
    /// Things for the cutscenes
    //std::vector<Cutscene> cutscenes;
    std::vector<sf::Text> t_cutscene_text;
    int startAlpha;
    int endAlpha;
    sf::Time targetTime;
    sf::Clock timer;
    sf::RectangleShape fade;
    bool inCutscene;
    bool inFadeTransition;
    bool isBlackScreenCutscene;
    bool cutscenesLeft = false;
    bool showTimer = false;
    int currentCutsceneId;
    std::vector<std::string> cutscene_text_identifiers;
    std::vector<int> cutscene_lengths;
    std::vector<bool> cutscene_blackscreens;
    /// this is a list of things in the level that
    /// we need to check against for collision (but not always damage)

    enum LAYERS // Stuffs
    {
        BUILDINGS = 0,
        NATURE = 50,
        ANIMALS = 150,
        UI = 9999
    };

    ///object lists
    std::vector<std::unique_ptr<PlayableUnit>> units;
    std::vector<std::unique_ptr<Entity>> tangibleLevelObjects;
    std::vector<std::unique_ptr<Projectile>> levelProjectiles;

    ///cloneables
    std::vector<std::unique_ptr<Kirajin_Yari_2>> kirajins;

    float patapon_y = 600;
    float gravity = 981;
    float floor_y = 200;

    float army_x = 800;
    float army_x_dest = 800;
    float army_width = 200;

    float prevTime = 0;
    sf::Clock walkClock;
    bool startWalking = false;
    bool walkBackwards = false;

    struct DamageCounter {
        int type = 0;            ///0 - regular, 1 - high, 2 - crit
        int damage = 0;          ///damage to display
        sf::Clock display_timer; ///timer for displaying later digits

        ///vectors containing texture, pos and scale for every digit
        vector<PSprite> spr;
        vector<sf::Vector2f> pos;
        vector<float> scale;
        vector<float> scale_goal;
        vector<float> alpha;
        vector<bool> mode;
    };

    PSpritesheet dmg_spritesheet;
    std::map<std::string, PSpritesheet> droppeditem_spritesheet;

    std::vector<DamageCounter> dmgCounters;
    std::vector<DroppedItem> droppedItems;

    sf::SoundBuffer spear_hit_enemy, spear_hit_iron, spear_hit_rock, spear_hit_solid, s_heal;

    std::deque<sf::Sound> projectile_sounds;

    bool missionEnd = false;
    float textCurX = -1280;
    float barCurX = 1920;
    float textDestX = 640;
    float barDestX = 640;
    float textCurScale = 1;
    float textDestScale = 1;
    bool textBounce = false;

    struct CollisionEvent ///used to retrieve more data about the collision between objects
    {
        bool collided = false;     ///did collision event happen?
        int collidedEntityID = -1; ///id of entity that has been collided with
        bool isCollidable = true;  ///is the entity collidable?
        bool isAttackable = true;  ///is the entity attackable?
        float defend_factor = 1;   ///how much did the entity defended off?
        int collidedEntityCategory = -1;
    };

    struct PickedItem ///used for item display in the top right corner
    {
        ///data
        string item_name;

        ///visuals
        sf::CircleShape circle;
        PSprite item;
        sf::Vector2f bounds;
    };

    struct UnitThumb ///used for unit display in the top left corner
    {
        ///data
        int unit_class;

        ///visuals
        sf::CircleShape circle;
        PSprite thumb;
        PSprite equip_1, equip_2;
        PText unit_count;
        PText unit_count_shadow;
        PSprite hpbar_back;
        PSprite hpbar_ins;
        int width;
    };

    vector<PickedItem> pickedItems;
    vector<UnitThumb> unitThumbs;

    sf::RectangleShape r_floor;

    bool debug_map_drop = false;
    bool failure = false;

    int curMissionID = 0;
    int spawnOrder = 0;

    vector<PataDialogBox> dialog_boxes;
    vector<MessageCloud> message_clouds;

    float command_padding;
    vector<PText> command_descs;
    vector<PText> command_inputs;

    struct DroppableCache {
        sf::Image img;
        bool cached = false;
    };

    std::map<std::string, std::map<int, DroppableCache>> droppable_cache; ///this map stores images for droppable items. droppable_cache[item_group][item_id] = sf::Image;

    bool verboseLogs = false;

    bool returnToPatapolis = false;

    std::vector<std::thread> missionThreads;

    /** Resolve enums **/
    int layerStr2Enum(string layer);

    /** Collisions **/
    static bool DoCollisionStepInAxis(float currentAxisAngle, HitboxFrame* currentHitboxFrame, AnimatedObject* targetObject, HitboxFrame* currentObjectHitBoxFrame, float currentObjectX, float CurrentObjectY) ;
    static vector<MissionController::CollisionEvent> DoCollisions(HitboxFrame* currentObjectHitBoxFrame, float currentObjectX, float currentObjectY, int collisionObjectID, vector<CollidableObject*> collisionObjects, vector<string> collisionData = {}) ;

    /** Cutscenes **/
    void StartCutscene(const std::string& text, bool isBlackScreen, int TimeToShow);
    void FinishLastCutscene();
    bool isMoreCutscenes();

    /** Utils **/
    float Smoothstep(float time);
    float Clamp(float x, float lowerlimit, float upperlimit);

    /** Save data management **/
    void submitPickedItems();
    void updateMissions();

    /** Load up the assets **/
    void addDmgCounter(int type, int damage, float baseX, float baseY, int q, int r);
    void addItemsCounter(int id, float baseX, float baseY);
    void parseEntityLoot(std::mt19937& gen, std::uniform_real_distribution<double>& roll, nlohmann::json loot, std::vector<Entity::Loot>& to_drop);
    void spawnEntity(int id, bool collidable, bool attackable, int xpos, int xrange, bool cloneable, float clone_delay, float spawnrate, float stat_mult, int mindmg, int maxdmg, int hp, float ypos, float baseY, sf::Color color = sf::Color::White, int layer = 9999, int parent = -1, nlohmann::json loot = {}, nlohmann::json additional_data = {});
    void addPickedItem(std::string spritesheet, int spritesheet_id, std::string picked_item);
    void addUnitThumb(int unit_id);
    void spawnProjectile(PSprite& sprite, float xPos, float yPos, float speed, float hspeed, float vspeed, float angle, float maxdmg, float mindmg, float crit, bool enemy = false);

    /** Load up the mission **/
    void StartMission(std::string missionFile, bool showCutscene = false, int missionID = 0, float mission_multiplier = 1);

    /** Stop the mission **/
    void StopMission();

    /** Mission update stuff **/
    void DoMovement();
    void DoRhythm();
    void ClearMissionMemory();
    void DoMissionEnd();
    void DoVectorCleanup(std::vector<int> units_rm, std::vector<int> dmg_rm, std::vector<int> tlo_rm, std::vector<int> pr_rm);
    void drawCommandList(sf::RenderWindow& window);
    std::vector<int> DrawProjectiles();
    void DrawUnitThumbs(sf::RenderWindow& window);
    void DrawPickedItems(sf::RenderWindow& window);
    void DrawHitboxes(sf::RenderWindow& window);
    std::vector<int> DrawDamageCounters();
    std::vector<int> DrawEntities();
    std::vector<int> DrawUnits();

    /** Main update function **/
    void Update(sf::RenderWindow& window, float cfps, InputController& inputCtrl);
    void Update();

    /** Events **/
    void DoKeyboardEvents();


    MissionController();
    ~MissionController();
private:
    static bool isColliding(PlayableUnit* unit, const unique_ptr<Entity>& entity);
    static vector<CollisionEvent> computeCollisions(HitboxFrame* currentObjectHitBoxFrame, float currentObjectX, float currentObjectY, int collisionObjectID, vector<string>& collisionData, CollidableObject* collisionObject);
};


#endif // CAMERA_H
