#include "Menu.h"
#include "iostream"
#include "../V4Core.h"
Menu::Menu()
{
    //ctor
    isActive = false;
    b_menu_enter.loadFromFile("resources/sfx/menu/MenuHit2.ogg");
}
void Menu::Back(){
    /// this should go back to the previous menu or exit.
}
void Menu::Show(){
    isActive=true;
    s_menu_enter.setBuffer(b_menu_enter);
    s_menu_enter.setVolume(10.4f);
    s_menu_enter.play();
}
void Menu::Hide(){
    isActive=false;
}
void Menu::UpdateButtons(){
    /// this should update the text on this menu
}
Menu::~Menu()
{
    //dtor
}
