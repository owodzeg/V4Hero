#ifndef ALTAR_H
#define ALTAR_H
#include <SFML/Graphics.hpp>
#include "../Config.h"
#include "../Graphics/Menu.h"
class V4Core;
class PatapolisMenu;
class AltarMenu : public Menu
{
    public:

        sf::RectangleShape mm_inventory_background;

        sf::RectangleShape mm_highlighted_tile;

        sf::RectangleShape mm_icon_example_tile;

        sf::Font f_font;
        sf::Text t_titlemenu;
        sf::Text t_itemtitle;
        sf::Text t_itemcategory;
        std::vector<sf::Text> t_itemdescription;
        Camera camera;

        sf::Texture t_menu_bkg;
        PSprite s_menu_bkg;
        sf::Vector2f p_menu_bkg;

        sf::Texture t_titlemenu_bkg;
        PSprite s_titlemenu_bkg;
        sf::Vector2f p_titlemenu_bkg;

        PatapolisMenu *parentMenu;

        /// mask icon
        PSprite mask_icon;
        sf::Vector2f p_mask_icon;

        /// spear icon
        PSprite spear_icon;
        sf::Vector2f p_spear_icon;


        int numItemRows;
        int numItemColumns = 4;
        int currentRow = 0;

        int inventoryGridXPos = 0;
        int inventoryGridYPos = 0;

        void Initialise(Config *thisConfig, std::map<int,bool> *keymap,V4Core *parent,PatapolisMenu *curParentMenu);
        void Update(sf::RenderWindow &window, float fps);
        void EventFired(sf::Event event);
        void ShowCategory();
        void OnExit();
        void UpdateAltarDescriptions();
        void UpdateButtons();
        AltarMenu();
        ~AltarMenu();



};


#endif // ALTARMENU_H
