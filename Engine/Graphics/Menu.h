#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>
#include "../Config.h"
#include "../Mission/MissionController.h"
#include "Scene.h"
class V4Core;
class Menu : public Scene
{
    public:
        bool isActive;
        virtual void Back();
        virtual void Show();
        void Hide();
        virtual void UpdateButtons();
        Menu();
        ~Menu();

};


#endif // MENU_H
