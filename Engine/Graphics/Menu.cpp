#include "Menu.h"
#include "iostream"
#include "../V4Core.h"
Menu::Menu()
{
    //ctor
    isActive = false;
}
void Menu::Back(){
    /// this should go back to the previous menu or exit.
}
void Menu::Show(){
    isActive=true;
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
