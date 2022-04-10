#ifndef OBELISK_H
#define OBELISK_H
#include "../Config.h"
#include "../Dialog/ControlTips.h"
#include "../Graphics/Menu.h"
#include "../Graphics/PText.h"
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

class V4Core;
class PatapolisMenu;
class ObeliskMenu : public Menu
{
public:
    sf::Font font;
    int quality = 1;

    ControlTips ctrlTips;

    ///worldmap contents
    std::vector<PSprite> location_bgs;
    std::vector<PSprite> location_icons;
    std::vector<PSprite> mission_icons;
    std::vector<PSprite> worldmap_icons;
    std::vector<PSprite> worldmap_fields;
    PSprite location_bg_a, location_bg_b;
    PSprite dullpon;

    ///boxes
    PSprite mainbox, descbox, iconbox, missionbox;
    PSprite missionselect;

    ///text
    PText worldmap_title;
    PText location_title;
    PText location_desc;
    PText select_quest;
    PText mission_title;
    PText mission_desc;

    int cur_location = 0;
    float mainbox_destX = 640, mainbox_destY = 360;
    float alphaA = 0;
    float alphaB = 255;
    float location_bg_a_destAlpha = 0;
    float location_bg_b_destAlpha = 255;

    bool displayMissions = false;
    bool runonce = false;

    PSprite unavailable;
    PSprite location_highlight;
    PSprite mission_select;

    int sel_prevlocation = 1;
    int sel_location = 1;
    int sel_mission = 0;

    std::vector<int> unlocked = {0};
    std::vector<int> missions_unlocked = {1, 2, 3, 6};

    std::vector<int> fields_unlocked = {1};

    struct Mission {
        PText p_mis;
        int mis_ID = 0; ///mission ID
        int loc_ID = 0; ///location ID
        std::string title;
        std::string desc;
        std::string mission_file;
    };

    std::vector<Mission> missions; ///Missions returned for a specific location

    sf::Texture test_tex;
    sf::Sprite test_spr;

    float mapX = 0;
    float mapXdest = 0;

    sf::View v_render_map;
    sf::View v_render_missions_map;

    bool initialized = false;
    int goto_id = -1;

    PatapolisMenu* parentMenu;

    void addMission(nlohmann::json missiondata);
    void Initialise();
    void Reload();
    void Update(sf::RenderWindow& window, float fps, InputController& inputCtrl);
    void Update();
    void EventFired(sf::Event event);
    void OnExit();
    void UpdateButtons();
    ObeliskMenu();
    ~ObeliskMenu();
};


#endif // OBELISK_H
