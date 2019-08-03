#include "MainMenu.h"
#include "ButtonList.h"
#include "iostream"
#include "../V4Core.h"
MainMenu::MainMenu()
{
    //ctor
    f_font.loadFromFile("resources/fonts/patapon.ttf");
    //f_font.loadFromFile("resources/fonts/arial.ttf");
    t_title.setFont(f_font);

    t_title.setCharacterSize(112);
    //t_title.setColor(sf::Color::White);
    t_title.setFillColor(sf::Color::White);
    t_title.setString("PATAFOUR");
    t_title.setOrigin(t_title.getGlobalBounds().width/2,t_title.getGlobalBounds().height/2);

    //t_pressToContinue.setFont(f_font);
    //t_pressToContinue.setCharacterSize(32);
    //t_pressToContinue.setColor(sf::Color::White);
    //t_pressToContinue.setFillColor(sf::Color::White);
    //t_pressToContinue.setString("Press any key to continue...");
    //t_pressToContinue.setOrigin(t_pressToContinue.getGlobalBounds().width/2,t_pressToContinue.getGlobalBounds().height/2);

    mm_bigBox.setSize(sf::Vector2f(100,10));
    mm_bigBox.setFillColor(sf::Color(4,0,90));

    mm_smallerBox.setSize(sf::Vector2f(100,10));
    mm_smallerBox.setFillColor(sf::Color::Red);

    mm_titleBox.setSize(sf::Vector2f(100,10));
    mm_titleBox.setFillColor(sf::Color::Red);
    inMission=false;
}
void MainMenu::Initialise(Config *thisConfigs,std::map<int,bool> *keymap,V4Core *parent){
    thisConfig = thisConfigs;
    thisConfig->GetInt("keybindDon");
    v4core = parent;
    buttonList.Initialise(&f_font,*thisConfig,keymap,&(v4core->currentController),this);
}
void MainMenu::EventFired(sf::Event event){
    if(event.type == sf::Event::KeyPressed)
    {
        // do something here;
        buttonList.KeyPressedEvent(event);
        if(event.key.code==59) {
            thisConfig->debugOut->DebugMessage("Returning to main menu...");
            v4core->currentController.StopMission();
            inMission=false;
        }
    } else if (event.type == sf::Event::MouseButtonReleased){
        // We use mouse released so a user can change their mind by keeping the mouse held and moving away.
        buttonList.MouseReleasedEvent(event);
    }
}
void MainMenu::Update(sf::RenderWindow &window, float fps)
{
    if(!inMission){
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


        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        auto lastView = window.getView();
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos,lastView);

        //t_pressToContinue.setPosition(window.getSize().x/2,300);
        //window.draw(t_pressToContinue);

        buttonList.Update(window, fps, &worldPos);
        window.setView(window.getDefaultView());
    } else {
        v4core->currentController.Update(window, fps);
    }

}

MainMenu::~MainMenu()
{
    //dtor
}
