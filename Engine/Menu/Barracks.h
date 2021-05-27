#ifndef BARRACKSMENU_H
#define BARRACKSMENU_H
#include <SFML/Graphics.hpp>
#include "../Config.h"
#include "../Graphics/Menu.h"
#include "../Graphics/PText.h"
#include "../Mission/MissionController.h"
#include "../Input/InputController.h"
#include "../Dialog/ControlTips.h"
#include "../Dialog/DialogBox.h"
#include "../Dialog/RoundedRect.h"

class V4Core;
class Barracks : public Menu
{
    public:
        MissionController* currentController;
        Config *config;
        std::string mission_file;

        ControlTips ctrlTips;

        sf::RectangleShape rs_cover;
        sf::RectangleShape mm_bigBox;
        sf::RectangleShape mm_titleBox;
        sf::RectangleShape mm_selected_item_line;
        Yaripon* patapon;
        Yaripon* patapon2;
        Yaripon* patapon3;

        sf::Texture t_background;
        PSprite s_background;
        float ratioX;
        float ratioY;

        float resRatioX, resRatioY;
        int q=0;

        float pataponY;
        float floorY;

        int missionID = 0;
        float mission_multiplier = 1;

        bool obelisk = false;
        bool missionStarted = false;

        int qualitySetting = 0;

        sf::Font f_font;
        PText t_title;
        int currentItemPosition;
        vector<bool> enabledPositons;

        RoundedRect rr_main, rr_main_sh;
        RoundedRect rr_unitstatus, rr_unitstatus_sh;
        RoundedRect rr_uniticon, rr_uniticon_sh;

        PSprite class_icon;

        PText unit_status, class_name;

        PText unit_stat_level_t, unit_stat_level_v;
        PText unit_stat_exp_t, unit_stat_exp_v;
        PText unit_stat_hp_t, unit_stat_hp_v;
        PText unit_stat_dmg_t, unit_stat_dmg_v;
        PText unit_stat_atkspd_t, unit_stat_atkspd_v;

        PText unit_stat_critc_t, unit_stat_critc_v;
        PText unit_stat_kbc_t, unit_stat_kbc_v;
        PText unit_stat_stgc_t, unit_stat_stgc_v;
        PText unit_stat_firec_t, unit_stat_firec_v;
        PText unit_stat_icec_t, unit_stat_icec_v;

        /// Barracks Equipment Text
        PText t_unit_rarepon_name;
        PText t_weapon_name;
        PText t_weapon2_name;
        PText t_armour_name;
        PText t_mask_name;

        PSprite inv_box;
        int gridSelX=0, gridSelY=0, gridOffsetY=0;

        /// menu background
        PSprite s_menu_bkg;

        /// highlighted unit
        PSprite s_pon_highlight;

        /// class icon
        PSprite s_class_icon;

        /// unit icon
        PSprite s_unit_icon;

        /// weapon icon
        PSprite s_weapon_icon;

        /// weapon2 icon
        PSprite s_weapon2_icon;

        /// armour icon
        PSprite s_armour_icon;

        /// mask icon
        PSprite s_mask_icon;

        int current_selected_pon=0;
        float item_line_flash = 0;

        bool MenuMode = false;

        struct InvBox
        {
            ///data
            Item* data;
            int amount=0;
            int occ_amount=0;
            bool highlight = false;
            vector<int> invIDs;

            ///display
            sf::RectangleShape r_outer;
            sf::RectangleShape r_inner;
            sf::RectangleShape r_highlight;
            PText num, num_shadow;
            PSprite icon;
        };

        vector<InvBox> inventory_boxes;
        sf::RectangleShape r_sel;

        RoundedRect rr_itempreview, rr_itempreview_sh;
        PText item_title, item_desc;

        int currentItemId;

        int activeCategory=1;
        int activeSubcategory=0;

        Camera camera;
        Menu *parentMenu;
        int currentMenuPosition;
        std::vector<float> possibleMenuPositions;
        void Initialise(Config *thisConfig, V4Core *parent, Menu *curParentMenu);
        void Update(sf::RenderWindow &window, float fps, InputController& inputCtrl);
        void UpdateInputControls();
        void EventFired(sf::Event event);
        void SetTitle(int menuPosition);
        sf::RectangleShape mm_description_background;
        sf::RectangleShape mm_highlighted_tile;

        PSprite highlighted_pon;

        PText t_itemtitle;
        std::vector<PText> t_itemdescription;

        vector<PataDialogBox> dialogboxes;

        void OnExit();
        void ReloadInventory();
        void SetInventoryPosition();
        void ApplyEquipment();
        void RefreshStats();
        void UpdatePreviewText();
        void UpdateButtons();
        Barracks();
        ~Barracks();



};


#endif // BARRACKSMENU_H
