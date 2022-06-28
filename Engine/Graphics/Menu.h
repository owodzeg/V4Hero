#ifndef MENU_H
#define MENU_H

#include "../Graphics/ScreenFade.h"
#include <SFML/Graphics.hpp>

class Menu
{
public:
    bool is_active;
    ScreenFade screenFade;
    int goto_id = -1;
    Menu();
};


#endif // MENU_H
