#include "SaveFileCreated.h"
#include "ButtonList.h"
#include "ButtonLists/NameEntryButtonList.h"
#include "iostream"
#include "../V4Core.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include "MenuButton.h"
SaveFileCreatedMenu::SaveFileCreatedMenu()
{
    //ctor
    f_font.loadFromFile("resources/fonts/p4kakupop-pro.ttf");
    //f_font.loadFromFile("resources/fonts/arial.ttf");

    t_title.setFont(f_font);
    t_title.setCharacterSize(25);
    t_title.setFillColor(sf::Color::White);

    t_welcome1.setFont(f_font);
    t_welcome1.setCharacterSize(25);
    t_welcome1.setFillColor(sf::Color::White);

    t_welcome2.setFont(f_font);
    t_welcome2.setCharacterSize(55);
    t_welcome2.setFillColor(sf::Color::White);

    t_welcome3.setFont(f_font);
    t_welcome3.setCharacterSize(25);
    t_welcome3.setFillColor(sf::Color::White);

    mm_bigBox.setSize(sf::Vector2f(100,10));
    mm_bigBox.setFillColor(sf::Color(4,0,90));

    mm_titleBox.setSize(sf::Vector2f(100,10));
    mm_titleBox.setFillColor(sf::Color::Red);
    isActive=true;
}
void SaveFileCreatedMenu::Initialise(Config *thisConfigs,V4Core *parent,NewGameNameEntryMenu* parentMenu_){
    Scene::Initialise(thisConfigs,parent);
    //v4core->menus.push_back(&optionsMenu);
    buttonList.Initialise(&f_font,*thisConfig,&(v4core->currentController),this);

    t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"newgame_game_save_created")));
    t_title.setOrigin(t_title.getGlobalBounds().width/2,t_title.getGlobalBounds().height/2);

    t_welcome1.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"newgame_welcome")));
    t_welcome1.setOrigin(t_welcome1.getGlobalBounds().width/2,t_welcome1.getGlobalBounds().height/2);

    t_welcome2.setString(Func::ConvertToUtf8String(kamiName));
    t_welcome2.setOrigin(t_welcome2.getGlobalBounds().width/2,t_welcome2.getGlobalBounds().height/2);

    t_welcome3.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"newgame_welcome2")));
    t_welcome3.setOrigin(t_welcome3.getGlobalBounds().width/2,t_welcome3.getGlobalBounds().height/2);
    nameEntryMenu = parentMenu_;

}
void SaveFileCreatedMenu::UpdateKamiName(string newName){
    kamiName=newName;
    t_welcome2.setString(Func::ConvertToUtf8String(kamiName));
    t_welcome2.setOrigin(t_welcome2.getGlobalBounds().width/2,t_welcome2.getGlobalBounds().height/2);
}
void SaveFileCreatedMenu::EventFired(sf::Event event){
    if(isActive){
        if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code==sf::Keyboard::Enter) {
                buttonList.SelectButton(0);
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            // We use mouse released so a user can change their mind by keeping the mouse held and moving away.
            buttonList.MouseReleasedEvent(event);
        }
    }
}
void SaveFileCreatedMenu::Update(sf::RenderWindow &window, float fps)
{
    if(isActive){

        mm_bigBox.setSize(sf::Vector2f(window.getSize().x,window.getSize().y-200));
        //mm_smallerBox.setSize(sf::Vector2f(100,10));
        //mm_titleBox.setSize(sf::Vector2f(100,10));

        mm_bigBox.setPosition(0,85);
        //mm_smallerBox.setPosition(100,10);
        //mm_titleBox.setPosition(100,10);


        window.draw(mm_bigBox);
        //window.draw(mm_smallerBox);
        //window.draw(mm_titleBox);

        t_title.setPosition(window.getSize().x/2,200);
        window.draw(t_title);

        t_welcome1.setPosition(window.getSize().x/2,330);
        window.draw(t_welcome1);

        t_welcome2.setPosition(window.getSize().x/2,360);
        window.draw(t_welcome2);

        t_welcome3.setPosition(window.getSize().x/2,430);
        window.draw(t_welcome3);


        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        auto lastView = window.getView();
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos,lastView);

        //t_pressToContinue.setPosition(window.getSize().x/2,300);
        //window.draw(t_pressToContinue);
        buttonList.Update(window, fps, &worldPos);
        window.setView(window.getDefaultView());

    } else {
        if (v4core->currentController.isInitialized){
            //v4core->currentController.Update(window, fps, keyMap);
        }
    }

}
void SaveFileCreatedMenu::UpdateButtons(){
    /// this should update the text on all the buttons
    buttonList.UpdateButtons();
}
void SaveFileCreatedMenu::OnExit(){
    /// when we exit the main menu, we do nothing for now.
    /// perhaps we would want to unload sprites or songs etc
}
SaveFileCreatedMenu::~SaveFileCreatedMenu()
{
    //dtor
}
