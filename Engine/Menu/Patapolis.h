#ifndef PATAPOLISMENU_H
#define PATAPOLISMENU_H
#include "../Config.h"
#include "../Dialog/ControlTips.h"
#include "../Dialog/DialogBox.h"
#include "../Graphics/Menu.h"
#include "../Graphics/ScreenFade.h"
#include "../Input/InputController.h"
#include "../Mission/MissionController.h"
#include "Altar.h"
#include "Barracks.h"
#include "Credits.h"
#include "Obelisk.h"
#include "OptionsMenu.h"
#include <SFML/Graphics.hpp>

class V4Core;
class PatapolisMenu : public Menu
{
public:
    sf::RectangleShape mm_bigBox;
    sf::RectangleShape mm_titleBox;
    sf::RectangleShape fade_box;
    float fade_alpha = 255;

    int quality = 0;

    sf::Font f_font;
    PText t_title;

    sf::VertexArray v_background;
    sf::RectangleShape r_ground;
    float floor_height = 0;

    ///Patapolis components
    std::vector<PSprite> layer_6;
    std::vector<PSprite> layer_2;

    PSprite L5;
    PSprite L4;

    float floor_x_base = 0;
    float floor_x = 0;

    PSprite edge;
    PSprite bridge;
    PSprite rainbow;

    PSprite back_layer[4];
    vector<float> back_pos = {0, 3900, 7400, 9560};

    struct Sparkle ///for rainbow
    {
        PSprite sprk;
        float x = 0, y = 0;
        float baseX = 0;
        float curScale = 1;
        float initScale = 1;
        float alpha = 255;
        float timer = 1000;
    };

    struct LightRay ///for egg shine
    {
        sf::VertexArray triangle;
        float angle = 0;
        float max_distance = 150;
        float cur_distance = 150;
        float min_distance = 20;
        bool rise = false;
    };

    struct RayStart ///for egg shine start locations
    {
        float ox1, ox2, x1, y1, x2, y2;
    };

    struct ParagetSparkle ///for paraget
    {
        PSprite sprk;
        float x = 0, y = 0;
        float angle = 0;
        float baseX = 0, baseY = 0;
        float curScale = 1;
        float initScale = 1;
        float alpha = 255;
        float timer = 1000;
    };

    struct Fire {
        PSprite fire[3];
        PSprite glow;
        float x = 0, y = 0;
        float baseX = 0, baseY = 0;
        float curScale = 1;
        float id = 0;
        float glowsize = 0;
    };

    struct SmokeParticle {
        PSprite smk;
        float curScale = 0.1;
        float baseX = 0, baseY = 0;
        float x = 0, y = 0;
        float alpha = 255;
    };

    struct CloudA {
        PSprite cloud;
        float x = 0, y = 0;
    };

    struct CloudB {
        sf::CircleShape cloud;
        float base_x = 0, base_y = 0;
        float x = 0, y = 0;
    };

    vector<RayStart> coords;
    float rayXbase = 12215;
    float rayX = 12215;

    PSprite wakapon;

    AnimatedObject a_wakapon;
    AnimatedObject a_sen;

    PSprite world_egg;
    PSprite light_1, light_2, egg_light;

    float camPos = 1280;
    float camDest = 1280;

    bool light = false;

    std::vector<Sparkle> sparkles;
    std::vector<LightRay> lightrays;
    std::vector<Fire> fires;
    std::vector<SmokeParticle> smoke;
    std::vector<CloudA> clouds_A;
    std::vector<CloudB> clouds_B;

    PSprite p_smoke;
    float smokepath1 = 0;

    std::vector<int> locations;
    int location = 3;
    bool left = false;

    PSprite market;
    PSprite forge_main, forge_back, forge_glow, forge_fence, forge_slab, forge_slab_glow;
    PSprite barracks;
    PSprite festival_main;
    PSprite altar;
    PSprite obelisk;
    PSprite paraget_main, paraget_crystal_glow, paraget_crystal;
    std::vector<ParagetSparkle> paraget_sparkles;

    float crystal_y = 0;
    float crystal_y2 = 0;
    float forge_glowsize = 0;

    Fire forge_big, forge_purple;

    Camera camera;

    sf::Clock anim_timer; ///TEMPORARY FOR SHOWCASE ONLY
    float animStartVal;
    float animChangeVal;
    float totalTime;
    bool isAnim = false;
    bool initialised = false;
    bool doWaitKeyPress = true;
    bool save_loaded = false;

    ControlTips ctrlTips;

    AltarMenu altar_menu;
    Barracks barracks_menu;
    ObeliskMenu obelisk_menu;
    Menu* parentMenu;
    int currentMenuPosition;
    std::vector<float> possibleMenuPositions;

    vector<PataDialogBox> dialogboxes;
    vector<MessageCloud> messageclouds;

    //int goto_id = -1; ///Go-to ID where should Patapolis go after ScreenFade finishes

    Credits credits;

    /// Stores the currently visible objects to the camera, so only them can be drawn (more efficient)
    vector<int> draw_ID = {};

    enum Buildings
    {
        MARKET = 0,
        FORGE = 1,
        BARRACKS = 2,
        FESTIVAL = 3,
        SEN = 4,
        ALTAR = 5,
        OBELISK = 6,
        PARAGET = 7,
        WAKAPON = 8,
        EGG = 9
    };

    sf::SoundBuffer sb_city_loop;
    sf::Sound city_loop;

    void addL6(std::string variant, float x, float y, int q, int r);
    void addL2(std::string variant, float x, float y, int q, int r);
    void addSparkle(float x, float y);
    void addParagetSparkle(float x, float y);
    void addRay(float x1, float y1, float x2, float y2);
    Fire addFire(int type, float x, float y, bool add);
    void addSmokeParticle(float x, float y, PSprite& refer);
    void addCloud(std::string type, float x, float y, float xsize, float ysize, int q, int r);
    void Initialise(Config* thisConfig, V4Core* parent, Menu* curParentMenu);
    void Update(sf::RenderWindow& window, float fps, InputController& inputCtrl);
    void EventFired(sf::Event event);
    void SetTitle(int menuPosition);
    void OnExit();
    void UpdateButtons();
    PatapolisMenu();
    ~PatapolisMenu();
};


#endif // PATAPOLISMENU_H
