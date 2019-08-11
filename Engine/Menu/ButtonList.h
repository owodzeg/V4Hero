#ifndef BUTTONLIST_H
#define BUTTONLIST_H

#include <string>
#include <SFML/Graphics.hpp>
#include "MenuButton.h"
#include "../Config.h"
#include "../Mission/MissionController.h"
#include "../Graphics/Menu.h"
class Menu;
class ButtonList
{
    public:
        std::vector<MenuButton> buttons;
        int currentIndex;

        Config *config;
        std::map<int,bool> *keyMap;
        MissionController *currentController;
        Menu *parentMenu;
        void UpdateButtons();
        virtual void Initialise(sf::Font *font,Config &newConfig, std::map<int,bool> *keymap,MissionController *controller,Menu *parentMenu);
        virtual void Update(sf::RenderWindow &window, float fps, sf::Vector2f *mousePos);
        virtual void KeyPressedEvent(sf::Event event);
        virtual void MouseReleasedEvent(sf::Event event);
        virtual void HighlightButton(MenuButton *button,int index);
        virtual void SelectButton(int index);

        ButtonList();
        ~ButtonList();

};


#endif // CAMERA_H
