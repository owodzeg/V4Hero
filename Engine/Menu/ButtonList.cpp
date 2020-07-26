#include "ButtonList.h"
#include "string"
#include "MenuButton.h"
#include<iostream>
#include "../Mission/MissionController.h"
#include "MainMenu.h"
using namespace std;
ButtonList::ButtonList()
{

}
void ButtonList::Initialise(sf::Font *font, Config &newConfig, MissionController *controller, Menu *mainMenu)
{
    config = &newConfig;
    currentController = controller;
    parentMenu = mainMenu;
    //keyMap = thisKeymap;
    currentIndex = 0;
}
void ButtonList::Update(sf::RenderWindow &window, float fps, sf::Vector2f *mousePos)
{
    for (std::vector<MenuButton>::iterator it = buttons.begin(); it != buttons.end(); ++it)
        (*it).Update(window,fps,mousePos);
}
void ButtonList::MouseReleasedEvent(sf::Event event){
    for (std::vector<MenuButton>::iterator it = buttons.begin(); it != buttons.end(); ++it)
        (*it).MouseUp(event.mouseButton.x,event.mouseButton.y);
}
void ButtonList::HighlightButton(MenuButton *button,int index){
    currentIndex = index;
    for (std::vector<MenuButton>::iterator it = buttons.begin(); it != buttons.end(); ++it)
        (*it).SetSelected(false);
    buttons[index].SetSelected(true);
}
void ButtonList::DeselectButton(int index){
    buttons[index].SetSelected(false);
}
void ButtonList::SelectButton(int index){

}
void ButtonList::KeyPressedEvent(sf::Event event)
{
    int keyCode = event.key.code;
    if(keyCode == config->GetInt("keybindChaka") || keyCode == config->GetInt("secondaryKeybindChaka"))
    {

        currentIndex -=1;
        if (currentIndex<0){
            currentIndex = buttons.size()-1;
        }
    }
    if(keyCode == config->GetInt("keybindDon") || keyCode == config->GetInt("secondaryKeybindDon"))
    {
        currentIndex +=1;
        if (currentIndex>buttons.size()-1){
            currentIndex = 0;
        }
    }
    if(keyCode == config->GetInt("keybindMenuEnter")){ // enter
        SelectButton(currentIndex);
    }
    for (std::vector<MenuButton>::iterator it = buttons.begin(); it != buttons.end(); ++it)
        (*it).SetSelected(false);
    if(SelectButtons){
        buttons[currentIndex].SetSelected(true);
    }
}
void ButtonList::UpdateButtons(){
    /// this should update the text on all the buttons
    for (std::vector<MenuButton>::iterator it = buttons.begin(); it != buttons.end(); ++it)
        (*it).UpdateText();
}
ButtonList::~ButtonList()
{

}
