#ifndef OBELISK_H
#define OBELISK_H
#include "../Config.h"
#include "../Dialog/ControlTips.h"
#include "../Graphics/Menu.h"
#include "../Graphics/PataText.h"
#include "../Graphics/SpriteWrapper.h"
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

class V4Core;
class PatapolisMenu;
class ObeliskMenu : public Menu
{
public:
    int quality = 1;

    ControlTips ctrlTips;

    ///worldmap contents
    std::vector<SpriteWrapper> preloaded_location_bgs;
    std::vector<SpriteWrapper> preloaded_worldmap_icons;
    std::vector<SpriteWrapper> location_bgs;
    std::vector<SpriteWrapper> location_icons;
    std::vector<SpriteWrapper> mission_icons;
    std::vector<SpriteWrapper> worldmap_icons;
    std::vector<SpriteWrapper> worldmap_fields;
    SpriteWrapper location_bg_a, location_bg_b;
    SpriteWrapper dullpon;

    ///boxes
    SpriteWrapper mainbox, descbox, iconbox, missionbox;
    SpriteWrapper missionselect;

    ///text
    PataText worldmap_title;
    PataText location_title;
    PataText location_desc;
    PataText select_quest;
    PataText mission_title;
    PataText mission_desc;

    int cur_location = 0;
    float mainbox_destX = 640*3, mainbox_destY = 360*3;
    float alphaA = 0;
    float alphaB = 255;
    float location_bg_a_destAlpha = 0;
    float location_bg_b_destAlpha = 255;

    bool displayMissions = false;
    bool runonce = false;

    SpriteWrapper unavailable;
    SpriteWrapper location_highlight;
    SpriteWrapper mission_select;

    int sel_prevlocation = 1;
    int sel_location = 1;
    int sel_mission = 0;

    std::vector<int> unlocked = {0};
    std::vector<int> missions_unlocked = {1, 2, 3, 6};

    std::vector<int> fields_unlocked = {1};

    struct Mission {
        PataText p_mis;
        int mis_ID = 0; ///mission ID
        int loc_ID = 0; ///location ID
        std::string title;
        std::string desc;
        std::string mission_file;
    };

    std::vector<Mission> missions; ///Missions returned for a specific location

    float mapX = 0;
    float mapXdest = 0;

    sf::View v_render_map;
    sf::View v_render_missions_map;

    bool initialized = false;
    int goto_id = -1;

    void addMission(nlohmann::json missiondata);
    void Reload();
    void Update();
    ObeliskMenu();
    ~ObeliskMenu();
};


#endif // OBELISK_H
