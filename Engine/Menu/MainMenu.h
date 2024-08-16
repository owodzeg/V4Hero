#ifndef MAINMENU_H
#define MAINMENU_H
#include "../Config.h"
#include "../ResourceManager.h"
#include "../Dialog/DialogBox.h"
#include "../Dialog/MessageCloud.h"
#include "../Graphics/Menu.h"
#include "../Graphics/SpriteWrapper.h"
#include "../Graphics/PText.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Graphics/AnimatedCurveShape.h>
class V4Core;
class MainMenu : public Menu
{
public:
    int quality = 0;

    sf::Font f_font;

    sf::SoundBuffer sb_title_loop;
    sf::Sound title_loop;

    std::vector<std::string> temp_menu;

    PText t_option[4];

    sf::VertexArray v_background;

    float maxQX, maxQY;
    int fade = 0;
    float alpha = 240;

    float mouseX = 0, mouseY = 0;

    int totem_sel = 0;
    int old_sel = 0;
    float fire_count = 0;
    bool UsingMouseSelection = true;
    float g_x[4], g_dest[4];
    float g_y[4] = {1890, 2088, 2172, 2172};

    ///Pre-menu screen
    sf::RectangleShape rs_cover;
    sf::RectangleShape rs_cover2;
    PText t_pressanykey;

    sf::SoundBuffer sb_smash;
    sf::Sound s_smash;

    bool premenu = false;
    bool keypressed = false;
    bool firstrun = true;

    float logow_scale = 1.0;
    float logow_shscale = 1.0;
    float t_alpha = 0;
    float cv_alpha = 0;

    float ui_alpha = 0;

    float cur_y = 900;
    float dest_y = 900;

    float temp_anim_t = 0;
    AnimatedCurveShape* shp;

    sf::Clock menuClock;
    sf::Clock startClock;
    sf::Clock frClock;
    sf::Clock frwaitClock;

    bool mouseInBounds = false;
    bool initialized = false;

    std::vector<int> totem_sel_pos = {615, 1470, 2340, 3240};

    std::vector<PataDialogBox> dialogboxes;

    MessageCloud msgcloud;

    // new sprites
    SpriteWrapper logow_bg, logow_text, logow_shadow;
    SpriteWrapper grass[5], totem[5], fire[4], sword[3];
    SpriteWrapper logo, logo_shadow;
    SpriteWrapper aura;

    void Update();
    void EventFired(sf::Event event);
    void SelectMenuOption();
    MainMenu();
    ~MainMenu();
};


#endif // CAMERA_H
