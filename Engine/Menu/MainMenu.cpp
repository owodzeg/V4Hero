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

    mm_titleBox.setSize(sf::Vector2f(100,10));
    mm_titleBox.setFillColor(sf::Color::Red);
    isActive=true;
}
void MainMenu::Initialise(Config *thisConfigs,std::map<int,bool> *keymap,V4Core *parent){
    Scene::Initialise(thisConfigs,keymap,parent);
    optionsMenu.Initialise(thisConfigs,keymap,parent,this);
    patapolisMenu.Initialise(thisConfigs,keymap,parent,this);
    v4core->menus.push_back(&optionsMenu);
    buttonList.Initialise(&f_font,*thisConfig,keymap,&(v4core->currentController),this,&optionsMenu,&patapolisMenu);
}
void MainMenu::EventFired(sf::Event event){
    if (patapolisMenu.isActive){
        patapolisMenu.EventFired(event);
    } else if (v4core->currentController.isInitialized){
        if(event.type == sf::Event::KeyPressed)
        {
            // do something here;
            buttonList.KeyPressedEvent(event);
            if(event.key.code==sf::Keyboard::Escape) {
                thisConfig->debugOut->DebugMessage("Returning to pataAAAApolis menu...");
                v4core->currentController.StopMission();
                patapolisMenu.Show();
            }
        }
    } else if(isActive){
        if(event.type == sf::Event::KeyPressed)
        {
            // do something here;
            buttonList.KeyPressedEvent(event);
            if(event.key.code==sf::Keyboard::Escape) {
                thisConfig->debugOut->DebugMessage("Returning to main menu...");
                v4core->currentController.StopMission();
                isActive=true;
            }
        } else if (event.type == sf::Event::MouseButtonReleased){
            // We use mouse released so a user can change their mind by keeping the mouse held and moving away.
            buttonList.MouseReleasedEvent(event);
        }
    } else if (optionsMenu.isActive){
        optionsMenu.EventFired(event);
    }
}
void MainMenu::Update(sf::RenderWindow &window, float fps, std::map<int,bool> *keyMap, std::map<int,bool> *keyMapHeld)
{
    if (v4core->currentController.isInitialized){
            v4core->currentController.Update(window, fps, keyMap,keyMapHeld);
    } else if(isActive){
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
        if (patapolisMenu.isActive){
            patapolisMenu.Update(window,fps);
        } else {
            optionsMenu.Update(window,fps);
        }
    }

}
void MainMenu::UpdateButtons(){
    /// this should update the text on all the buttons
    buttonList.UpdateButtons();
}
void MainMenu::OnExit(){
    /// when we exit the main menu, we do nothing for now.
    /// perhaps we would want to unload sprites or songs etc
}
MainMenu::~MainMenu()
{
    //dtor
}
