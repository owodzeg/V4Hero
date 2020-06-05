#ifndef OBELISK_H
#define OBELISK_H
#include <SFML/Graphics.hpp>
#include "../Config.h"
#include "../Graphics/Menu.h"
#include "../Graphics/PText.h"
#include "ButtonLists/ObeliskButtonList.h"
class V4Core;
class PatapolisMenu;
class ObeliskMenu : public Menu
{
    public:
        sf::Font font;

        ///worldmap contents
        vector<PSprite> location_bgs;
        vector<PSprite> location_icons;
        vector<PSprite> mission_icons;
        vector<PSprite> worldmap_icons;
        vector<PSprite> worldmap_fields;
        PSprite location_bg_a, location_bg_b;
        PSprite dullpon;

        ///boxes
        PSprite mainbox,descbox,iconbox,missionbox;
        PSprite missionselect;

        ///text
        PText worldmap_title;
        PText location_title;
        PText location_desc;
        PText select_quest;
        PText mission_title;
        PText mission_desc;

        int cur_location = 0;
        float mainbox_destX=640,mainbox_destY=360;
        float alphaA = 0;
        float alphaB = 255;
        float location_bg_a_destAlpha = 0;
        float location_bg_b_destAlpha = 255;

        bool displayMissions = false;

        PSprite unavailable;
        PSprite location_highlight;

        int sel_location = 0;

        vector<int> unlocked = {0, 1, 2};

        sf::RenderTexture render_map;
        sf::Texture tex_render_map;
        sf::Sprite spr_render_map;

        float mapX = 0;
        float mapXdest = 0;

        PatapolisMenu *parentMenu;

        void Initialise(Config *thisConfig, std::map<int,bool> *keymap,V4Core *parent,PatapolisMenu *curParentMenu);
        void Update(sf::RenderWindow &window, float fps);
        void EventFired(sf::Event event);
        void OnExit();
        void UpdateButtons();
        ObeliskMenu();
        ~ObeliskMenu();



};


#endif // OBELISK_H
