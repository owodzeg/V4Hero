#ifndef ALTAR_H
#define ALTAR_H
#include <SFML/Graphics.hpp>
#include "../Config.h"
#include "../Graphics/Menu.h"
#include "../Input/InputController.h"
#include "../Dialog/ControlTips.h"
#include "../Dialog/RoundedRect.h"

class V4Core;
class PatapolisMenu;
class AltarMenu : public Menu
{
    public:
        float ratioX, ratioY;
        float resRatioX, resRatioY;
        int q = 1;

        sf::Font f_font;
        ControlTips ctrlTips;

        PSprite altar_main;

        float highlight_x = 0;

        struct InvBox
        {
            ///data
            Item* data;
            int amount=0;
            bool highlight = false;

            ///display
            sf::RectangleShape r_outer;
            sf::RectangleShape r_inner;
            sf::RectangleShape r_highlight;
            PText num, num_shadow;
            PSprite icon;
        };

        vector<InvBox> inventory_boxes;
        RoundedRect rr_title, rr_title_sh;
        RoundedRect rr_desc, rr_desc_sh;

        PText altar_title;
        PText altar_kaching;
        PText altar_item_title, altar_item_category, altar_item_desc;

        int gridSelX=0, gridSelY=0, gridOffsetY=0;
        sf::RectangleShape r_sel;

        bool loadedSave = false;

        PatapolisMenu *parentMenu;

        void Initialise(Config *thisConfig, V4Core *parent, PatapolisMenu *curParentMenu);
        void Update(sf::RenderWindow &window, float fps, InputController& inputCtrl);
        void EventFired(sf::Event event);
        void ReloadInventory();
        void ShowCategory();
        void OnExit();
        void ShowAltar();
        void UpdateAltarDescriptions();
        void UpdateButtons();
        AltarMenu();
        ~AltarMenu();



};


#endif // ALTARMENU_H
