#ifndef BARRACKSMENU_H
#define BARRACKSMENU_H
#include <SFML/Graphics.hpp>
#include "../Config.h"
#include "../Graphics/Menu.h"
class V4Core;
class Barracks : public Menu
{
    public:
        sf::RectangleShape mm_bigBox;
        sf::RectangleShape mm_titleBox;
        sf::RectangleShape mm_selected_item_line;

        sf::Font f_font;
        sf::Text t_title;
        int currentItemPosition;
        vector<bool> enabledPositons;
        /// Barracks text
        sf::Text t_unit_name;
        sf::Text t_unit_status_text;

        sf::Text t_unit_level;
        sf::Text t_unit_experience;
        sf::Text t_unit_damage;

        sf::Text t_unit_hp;
        sf::Text t_unit_crit;
        sf::Text t_unit_attack_speed;


        /// Barracks stat text
        sf::Text s_unit_level;
        sf::Text s_unit_experience;
        sf::Text s_unit_damage;

        sf::Text s_unit_hp;
        sf::Text s_unit_crit;
        sf::Text s_unit_attack_speed;

        /// Barracks Equipment Text
        sf::Text t_unit_rarepon_name;
        sf::Text t_weapon_name;
        sf::Text t_weapon2_name;
        sf::Text t_armour_name;
        sf::Text t_mask_name;


        /// menu background
        PSprite s_menu_bkg;
        sf::Vector2f p_menu_bkg;

        /// class icon
        PSprite s_class_icon;
        sf::Vector2f p_class_icon;

        /// unit icon
        PSprite s_unit_icon;
        sf::Vector2f p_unit_icon;

        /// weapon icon
        PSprite s_weapon_icon;
        sf::Vector2f p_weapon_icon;

        /// weapon2 icon
        PSprite s_weapon2_icon;
        sf::Vector2f p_weapon2_icon;

        /// armour icon
        PSprite s_armour_icon;
        sf::Vector2f p_armour_icon;

        /// mask icon
        PSprite s_mask_icon;
        sf::Vector2f p_mask_icon;

        int current_selected_pon=0;

        bool MenuMode = false;


        /// mask icon
        PSprite mask_icon;

        /// spear icon
        PSprite spear_icon;

        /// misc icon
        PSprite misc_icon;

        /// armour icon
        PSprite armour_icon;


        int activeCategory=1;
        int activeSubcategory=0;

        int numItemRows;
        int numItemColumns = 4;
        int currentRow = 0;

        int inventoryGridXPos = 0;
        int inventoryGridYPos = 0;
        sf::RectangleShape mm_inventory_background;
        sf::RectangleShape mm_description_background;
        sf::RectangleShape mm_highlighted_tile;

        int current_pon=0;

        sf::Text t_itemtitle;
        std::vector<sf::Text> t_itemdescription;

        Camera camera;
        Menu *parentMenu;
        int currentMenuPosition;
        std::vector<float> possibleMenuPositions;
        void Initialise(Config *thisConfig, std::map<int,bool> *keymap,V4Core *parent,Menu *curParentMenu);
        void Update(sf::RenderWindow &window, float fps);
        void EventFired(sf::Event event);
        void SetTitle(int menuPosition);
        void OnExit();
        void OpenBarracksMenu();
        void RefreshStats();
        void UpdateButtons();
        Barracks();
        ~Barracks();



};


#endif // BARRACKSMENU_H
