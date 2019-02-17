#ifndef BUTTONLIST_H
#define BUTTONLIST_H

#include <string>
#include <SFML/Graphics.hpp>
#include "MenuButton.h"
#include "../Config.h"
#include "../Mission/MissionController.h"

class MainMenu;
class ButtonList
{
    public:
        std::vector<MenuButton> buttons;
        int currentIndex;

        Config *config;
        std::map<int,bool> *keyMap;
        MissionController *currentController;
        MainMenu *parentMenu;

        void Initialise(sf::Font *font,Config &newConfig, std::map<int,bool> *keymap,MissionController *controller,MainMenu *parentMenu);
        void Update(sf::RenderWindow &window, float fps, sf::Vector2f *mousePos);
        void KeyPressedEvent(sf::Event event);
        void MouseReleasedEvent(sf::Event event);
        void HighlightButton(MenuButton *button,int index);
        void SelectButton(int index);

        ButtonList();
        ~ButtonList();

};


#endif // CAMERA_H
