#ifndef MAINMENU_H
#define MAINMENU_H
#include "../Config.h"
#include "../ResourceManager.h"
#include "../Dialog/DialogBox.h"
#include "../Graphics/Menu.h"
#include "../Graphics/PSprite.h"
#include "../Graphics/PText.h"
#include "../Mission/MissionController.h"
#include "IntroductionMenu.h"
#include "NewGameNameEntryMenu.h"
#include "OptionsMenu.h"
#include "Patapolis.h"
#include <SFML/Graphics.hpp>
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
    float fire = 0;
    bool UsingMouseSelection = true;
    float g_x[4], g_dest[4];

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

    float cur_y = 300;
    float dest_y = 300;

    sf::Clock menuClock;
    sf::Clock startClock;
    sf::Clock frClock;
    sf::Clock frwaitClock;
    sf::Clock patapolisClock;

    bool mouseInBounds = false;
    bool initialized = false;

    vector<int> totem_sel_pos = {205, 490, 780, 1080};

    vector<PataDialogBox> dialogboxes;

    MessageCloud msgcloud;

    NewGameNameEntryMenu nameEntryMenu;
    IntroductionMenu introductionMenu;

    void Update();
    void EventFired(sf::Event event);
    void OnExit();
    void SelectMenuOption();
    void UpdateButtons();
    MainMenu();
    ~MainMenu();
};


#endif // CAMERA_H
