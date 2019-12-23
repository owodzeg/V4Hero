#include "Obelisk.h"
#include "ButtonList.h"
#include "iostream"
#include "../V4Core.h"
#include "math.h"
#include <sstream>
ObeliskMenu::ObeliskMenu()
{
    f_font.loadFromFile("resources/fonts/p4kakupop-pro.ttf");
    //f_font.loadFromFile("resources/fonts/arial.ttf");
    t_titlemenu.setFont(f_font);
    t_titlemenu.setCharacterSize(38);
    t_titlemenu.setFillColor(sf::Color::Black);

    t_itemtitle.setFont(f_font);
    t_itemtitle.setCharacterSize(28);
    t_itemtitle.setFillColor(sf::Color::Black);





    isActive=false;
}
void ObeliskMenu::Initialise(Config *thisConfigs,std::map<int,bool> *keymap,V4Core *parent, PatapolisMenu *curParentMenu){
    Scene::Initialise(thisConfigs,keymap,parent);
    parentMenu = curParentMenu;

    thisConfig->debugOut->DebugMessage("Altar menu image loaded: menurect.png");



}
void ObeliskMenu::EventFired(sf::Event event){
    if(isActive){
        if(event.type == sf::Event::KeyPressed)
        {
            // do something here;
            if (event.key.code == sf::Keyboard::Num1){


            } else if (event.key.code == sf::Keyboard::Num2){



            } else if (event.key.code == sf::Keyboard::Num3){



            } else if (event.key.code == sf::Keyboard::Num4){



            } else if (event.key.code == sf::Keyboard::Num5){


            } else if (event.key.code == thisConfig->GetInt("keybindPon") || event.key.code == thisConfig->GetInt("secondaryKeybindPon")){


            } else if (event.key.code == thisConfig->GetInt("keybindPata") || event.key.code == thisConfig->GetInt("secondaryKeybindPata")){

            } else if (event.key.code == thisConfig->GetInt("keybindChaka") || event.key.code == thisConfig->GetInt("secondaryKeybindChaka")){


            } else if (event.key.code == thisConfig->GetInt("keybindDon") || event.key.code == thisConfig->GetInt("secondaryKeybindDon")){


            } else if (event.key.code == thisConfig->GetInt("keybindBack")){
                this->Hide();
                this->isActive = false;
            }

        } else if (event.type == sf::Event::MouseButtonReleased){
            // We use mouse released so a user can change their mind by keeping the mouse held and moving away.

        }
    }
}

void ObeliskMenu::Update(sf::RenderWindow &window, float fps)
{
    if(isActive){



    }
}

void ObeliskMenu::UpdateButtons(){

}
void ObeliskMenu::OnExit(){

}
ObeliskMenu::~ObeliskMenu()
{
    //dtor
}
