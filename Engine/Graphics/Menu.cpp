#include "Menu.h"
#include "../V4Core.h"
#include "iostream"
Menu::Menu()
{
    //ctor
    is_active = false;
    b_menu_enter.loadFromFile("resources/sfx/menu/MenuHit2.ogg");
}
void Menu::Back()
{
    /// this should go back to the previous menu or exit.
}
void Menu::Show()
{
    is_active = true;
    //s_menu_enter.setBuffer(b_menu_enter);
    //s_menu_enter.setVolume(float(thisConfig->GetInt("masterVolume"))*(float(thisConfig->GetInt("sfxVolume"))/100.f));
    //s_menu_enter.play();
}
void Menu::Hide()
{
    is_active = false;
}
void Menu::UpdateButtons()
{
    /// this should update the text on this menu
}
Menu::~Menu()
{
    //dtor
}
