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
void ButtonList::Initialise(sf::Font *font,Config &newConfig,  std::map<int,bool> *thisKeymap,MissionController *controller,MainMenu *mainMenu)
{
    config = &newConfig;
    MenuButton* level1Button = new MenuButton(config->strRepo.GetUnicodeString(L"menu_button_1"),font,42,300,this,0);
    MenuButton* level2Button = new MenuButton(config->strRepo.GetUnicodeString(L"menu_button_2"),font,42,350,this,1);
    MenuButton* optionsButton = new MenuButton(config->strRepo.GetUnicodeString(L"menu_button_3"),font,42,400,this,2);
    buttons.push_back(*level1Button);
    buttons.push_back(*level2Button);
    buttons.push_back(*optionsButton);
    buttons[0].SetSelected(true);
    currentController = controller;
    parentMenu = mainMenu;
    keyMap = thisKeymap;
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
void ButtonList::SelectButton(int index){
    currentIndex = index;
    switch (currentIndex){
            case 0:
                // start level 1
                currentController->Initialise(*config,*keyMap,config->GetString("mission1Background"));
                currentController->StartMission(config->GetString("mission1Theme"));
                parentMenu->inMission = true;
                break;
            case 1:
                // start level 2
                currentController->Initialise(*config,*keyMap,config->GetString("mission2Background"));
                currentController->StartMission(config->GetString("mission2Theme"));
                parentMenu->inMission = true;
                break;
            case 2:
                // start options
                break;
        }
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
    buttons[currentIndex].SetSelected(true);
}
ButtonList::~ButtonList()
{

}
