#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H
#include <SFML/Graphics.hpp>
#include "ButtonLists/OptionsButtonList.h"
#include "../Config.h"
#include "../Graphics/Menu.h"
#include "../Mission/MissionController.h"
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
        Menu *parentMenu;
        OptionsButtonList buttonList;
        void Initialise(Config *thisConfig, std::map<int,bool> *keymap,V4Core *parent,Menu *curParentMenu);
        void Update(sf::RenderWindow &window, float fps);
        void EventFired(sf::Event event);
        void OnExit();
        void Back();
        void UpdateButtons();
        void Show();
        OptionsMenu();
        ~OptionsMenu();

};


#endif // OPTIONSMENU_H
