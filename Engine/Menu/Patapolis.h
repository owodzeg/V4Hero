#ifndef PATAPOLISMENU_H
#define PATAPOLISMENU_H
#include "../Config.h"
#include "../ResourceManager.h"
#include "../Dialog/ControlTips.h"
#include "../Dialog/DialogBox.h"
#include "../Graphics/Menu.h"
#include "../Graphics/ScreenFade.h"
#include "../Input/InputController.h"
#include "../Mission/MissionController.h"
#include "../Mission/Units/AnimatedObject.h"
#include "../Mission/Camera.h"
#include "../Dialog/MessageCloud.h"
#include "Altar.h"
#include "Barracks.h"
#include "Credits.h"
#include "Obelisk.h"
#include "OptionsMenu.h"
#include <SFML/Graphics.hpp>
#include <Menu/MaterOuter.h>

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

        struct L {
            float x;
            float y;
            std::string type;
        };

        ///Patapolis components
        std::map<std::string, PSprite> ps_layer_6, ps_layer_2; ///we split PSprites and the position vectors; we will use only one PSprite to draw all of the elements of the background
        std::vector<L> layer_2, layer_6; ///here we store locations and type only

        float floor_x_base = 0;
        float floor_x = 0;

        std::string back_layer[4];
        std::vector<float> back_pos = {0, 3900, 7400, 9560};

        struct Sparkle ///for rainbow
        {
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
            std::string tx_name;
            float x = 0, y = 0;
            float angle = 0;
            float baseX = 0, baseY = 0;
            float curScale = 1;
            float initScale = 1;
            float alpha = 255;
            float timer = 1000;
        };

        struct Fire {
            std::string fire[3];
            std::string glow;
            float x = 0, y = 0;
            float baseX = 0, baseY = 0;
            float curScale = 1;
            float id = 0;
            float glowsize = 0;
        };

        struct SmokeParticle {
            float curScale = 0.1;
            float baseX = 0, baseY = 0;
            float x = 0, y = 0;
            float alpha = 255;
        };

        struct CloudA {
            float x = 0, y = 0;
        };

        struct CloudB {
            sf::CircleShape cloud;
            float base_x = 0, base_y = 0;
            float x = 0, y = 0;
        };

        std::vector<RayStart> coords;
        float rayXbase = 12215;
        float rayX = 12215;

        AnimatedObject a_wakapon;
        AnimatedObject a_sen;

        float light_value = 0;

        float camPos = 1280;
        float camDest = 1280;
        float materoffset = 200; 

        std::vector<Sparkle> sparkles;
        std::vector<LightRay> lightrays;
        std::vector<Fire> fires;
        std::vector<SmokeParticle> smoke;
        std::vector<CloudA> clouds_A;
        std::vector<CloudB> clouds_B;

        float dest_zoom = 1.0f;
        float smokepath1 = 0;

        std::vector<int> locations;
        int location = 3;
        bool left = false;

        std::vector<ParagetSparkle> paraget_sparkles;

        float crystal_y = 0;
        float crystal_y2 = 0;
        float forge_glowsize = 0;

        Fire forge_big, forge_purple;

        Camera camera;

        float animStartVal;
        float animChangeVal;
        float totalTime;
        bool isAnim = false;
        bool initialised = false;
        bool doWaitKeyPress = true;
        bool save_loaded = false;

        ControlTips ctrlTips;
        Menu* parentMenu;
        int currentMenuPosition;
        std::vector<float> possibleMenuPositions;

        std::vector<PataDialogBox> dialogboxes;
        std::vector<MessageCloud> messageclouds;

        //int goto_id = -1; ///Go-to ID where should Patapolis go after ScreenFade finishes

        Credits credits;

        /// Stores the currently visible objects to the camera, so only them can be drawn (more efficient)
        std::vector<int> draw_ID = {};

        enum Buildings
        {
            MARKET = 0,
            FORGE = 1,
            BARRACKS = 2,
            FESTIVAL = 3,
            SEN = 4,
            ALTAR = 5,
            OBELISK = 6,
            MATER = 7,
            PARAGET = 8,
            WAKAPON = 9,
            EGG = 10,
        };

        sf::SoundBuffer sb_city_loop;
        sf::Sound city_loop;

        sf::View patapolisView;

        bool initialized = false;

        void addL6(std::string variant, float x, float y);
        void addL2(std::string variant, float x, float y);
        void addSparkle(float x, float y);
        void addParagetSparkle(float x, float y);
        void addRay(float x1, float y1, float x2, float y2);
        Fire addFire(int type, float x, float y, bool add);
        void addSmokeParticle(float x, float y);
        void addCloud(std::string type, float x, float y, float xsize, float ysize);
        void Update();
        void EventFired(sf::Event event);
        void SetTitle(int menuPosition);
        void OnExit();
        void UpdateButtons();
        void updateStoryPoint();
        PatapolisMenu();
        ~PatapolisMenu();
};


#endif // PATAPOLISMENU_H
