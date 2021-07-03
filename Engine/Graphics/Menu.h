#ifndef MENU_H
#define MENU_H
#include "../Config.h"
#include "../Graphics/ScreenFade.h"
#include "../Mission/MissionController.h"
#include "Scene.h"
#include <SFML/Graphics.hpp>
class V4Core;
class Menu : public Scene
{
public:
    bool is_active;
    ScreenFade screenFade;
    int goto_id = -1;
    virtual void Back();
    sf::SoundBuffer b_menu_enter;
    sf::Sound s_menu_enter; ///FEVER!
    virtual void Show();
    void Hide();
    virtual void UpdateButtons();
    Menu();
    ~Menu();
};


#endif // MENU_H
