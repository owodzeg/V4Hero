#ifndef PATAPOLISMENU_H
#define PATAPOLISMENU_H
#include <SFML/Graphics.hpp>
#include "ButtonLists/MainMenuButtonList.h"
#include "../Config.h"
#include "../Graphics/Menu.h"
#include "../Mission/MissionController.h"
#include "OptionsMenu.h"
#include "Altar.h"
class V4Core;
class PatapolisMenu : public Menu
{
    public:
        sf::RectangleShape mm_bigBox;
        sf::RectangleShape mm_titleBox;

        sf::Font f_font;
        sf::Text t_title;

        std::vector<sf::Texture> t_background;
        std::vector<PSprite> s_background;
        std::vector<sf::Vector2f> p_background;
        std::vector<float> background_xspeed;
        std::vector<float> background_y;
        Camera camera;

        sf::Clock anim_timer; ///TEMPORARY FOR SHOWCASE ONLY
        float animStartVal;
        float animChangeVal;
        float totalTime;
        bool isAnim = false;

        AltarMenu altar_menu;
        OptionsMenu optionsMenu;
        Menu *parentMenu;
        int currentMenuPosition;
        std::vector<float> possibleMenuPositions;
        void Initialise(Config *thisConfig, std::map<int,bool> *keymap,V4Core *parent,Menu *curParentMenu);
        void Update(sf::RenderWindow &window, float fps);
        void EventFired(sf::Event event);
        void SetTitle(int menuPosition);
        void OnExit();
        void UpdateButtons();
        PatapolisMenu();
        ~PatapolisMenu();



};


#endif // PATAPOLISMENU_H
