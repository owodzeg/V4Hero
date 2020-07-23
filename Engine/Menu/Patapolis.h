#ifndef PATAPOLISMENU_H
#define PATAPOLISMENU_H
#include <SFML/Graphics.hpp>
#include "../Config.h"
#include "../Graphics/Menu.h"
#include "../Mission/MissionController.h"
#include "OptionsMenu.h"
#include "Altar.h"
#include "Barracks.h"
#include "Obelisk.h"
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

        struct Sparkle ///for rainbow
        {
            PSprite sprk;
            float x=0, y=0;
            float baseX=0;
            float curScale=1;
            float initScale=1;
            float alpha=255;
            float timer=1000;
        };

        struct LightRay ///for egg shine
        {
            sf::VertexArray triangle;
            float angle = 0;
            float max_distance = 150;
            float cur_distance = 150;
            float min_distance = 20;
            bool rise=false;
        };

        struct RayStart ///for egg shine start locations
        {
            float ox1, ox2, x1, y1, x2, y2;
        };

        struct ParagetSparkle ///for paraget
        {
            PSprite sprk;
            float x=0, y=0;
            float angle=0;
            float baseX=0, baseY=0;
            float curScale=1;
            float initScale=1;
            float alpha=255;
            float timer=1000;
        };

        struct Fire
        {
            PSprite fire[3];
            PSprite glow;
            float x=0, y=0;
            float baseX=0, baseY=0;
            float curScale=1;
            float id=0;
            float glowsize = 0;
        };

        struct SmokeParticle
        {
            PSprite smk;
            float curScale=0.1;
            float baseX=0, baseY=0;
            float x=0, y=0;
            float alpha = 255;
        };

        vector<RayStart> coords;
        float rayXbase = 12215;
        float rayX = 12215;

        PSprite wakapon;
        PSprite world_egg;
        PSprite light_1, light_2, egg_light;

        float camPos = 1280;
        float camDest = 1280;

        bool light = false;

        std::vector<Sparkle> sparkles;
        std::vector<LightRay> lightrays;
        std::vector<Fire> fires;
        std::vector<SmokeParticle> smoke;

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
        bool initialised=false;
        bool doWaitKeyPress=true;

        AltarMenu altar_menu;
        Barracks barracks_menu;
        ObeliskMenu obelisk_menu;
        Menu *parentMenu;
        int currentMenuPosition;
        std::vector<float> possibleMenuPositions;
        void addL6(std::string variant, float x, float y, int q, int r);
        void addL2(std::string variant, float x, float y, int q, int r);
        void addSparkle(float x, float y);
        void addParagetSparkle(float x, float y);
        void addRay(float x1, float y1, float x2, float y2);
        Fire addFire(int type, float x, float y, bool add);
        void addSmokeParticle(float x, float y, PSprite& refer);
        void Initialise(Config *thisConfig, std::map<int,bool> *keymap,V4Core *parent,Menu *curParentMenu);
        void Update(sf::RenderWindow &window, float fps);
        void EventFired(sf::Event event);
        void SetTitle(int menuPosition);
        void OnExit();
        void UpdateButtons();
        PatapolisMenu();
        ~PatapolisMenu();



};


#endif // PATAPOLISMENU_H
