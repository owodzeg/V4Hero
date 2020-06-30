#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H
#include <SFML/Graphics.hpp>
#include "../Config.h"
#include "../Graphics/Menu.h"
#include "../Mission/MissionController.h"
#include "../Graphics/PSprite.h"
#include "../Graphics/PText.h"
class V4Core;
class OptionsMenu : public Menu
{
    public:
        sf::RectangleShape mm_bigBox;
        sf::RectangleShape mm_smallerBox;
        sf::RectangleShape mm_titleBox;

        sf::Font f_font;
        sf::Font m_font;
        sf::Text t_title;

        sf::Text t_disclaimer;
        sf::Text t_pressToContinue;
        bool madeChanges;

        PSprite bg, sword;
        PSprite handle, aura1, aura2, l_fire1, l_fire2, l_fire3, r_fire1, r_fire2, r_fire3;

        float mouseX=0, mouseY=0;
        int state = 0;
        int sel = -1;

        struct Resolution
        {
            float width;
            float height;
        };

        struct ConfigValue
        {
            string index;
            string value;
        };

        PText options_header;
        std::vector<PText> options; ///main options
        std::vector<PText> g_options; /// graphics options
        std::vector<PText> a_options; /// audio options
        std::vector<PText> langs;
        std::vector<Resolution> float_resolutions;
        std::vector<PText> resolutions;
        std::vector<float> float_framerates;
        std::vector<PText> framerates;
        std::vector<PText> qualities;
        std::vector<PText> ms_volume;
        std::vector<PText> switches;
        std::vector<ConfigValue> original_config; ///If the changes are reverted, keep the original settings to return them back
        std::vector<PText> restarts;
        PText t_restart;

        PSprite dg_restart, dg_select;

        Menu *parentMenu;
        void Initialise(Config *thisConfig, std::map<int,bool> *keymap,V4Core *parent,Menu *curParentMenu);
        void Update(sf::RenderWindow &window, float fps);
        void SelectMenuOption();
        void SetConfigValue(std::string key, std::string value);
        void EventFired(sf::Event event);
        void OnExit();
        void Back();
        void UpdateButtons();
        void Show();
        OptionsMenu();
        ~OptionsMenu();

};


#endif // OPTIONSMENU_H
