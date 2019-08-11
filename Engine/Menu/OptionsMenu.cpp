#include "OptionsMenu.h"
#include "ButtonList.h"
#include "iostream"
#include "../V4Core.h"
OptionsMenu::OptionsMenu()
{
    //ctor
    f_font.loadFromFile("resources/fonts/patapon.ttf");
    m_font.loadFromFile("resources/fonts/p4kakupop-pro.ttf");
    t_title.setFont(f_font);

    t_title.setCharacterSize(112);
    //t_title.setColor(sf::Color::White);
    t_title.setFillColor(sf::Color::White);


    t_disclaimer.setFont(m_font);

    t_disclaimer.setCharacterSize(42);
    //t_title.setColor(sf::Color::White);
    t_disclaimer.setFillColor(sf::Color::Red);

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
    isActive=false;
    madeChanges=false;
}
void OptionsMenu::Initialise(Config *thisConfigs,std::map<int,bool> *keymap,V4Core *parent, Menu *curParentMenu){
    Scene::Initialise(thisConfigs,keymap,parent);
    buttonList.Initialise(&m_font,*thisConfig,keymap,&(v4core->currentController),this);
    parentMenu = curParentMenu;
    t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"menu_button_3")));
    t_title.setOrigin(t_title.getGlobalBounds().width/2,t_title.getGlobalBounds().height/2);
    t_disclaimer.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"option_disclaimer")));
    t_disclaimer.setOrigin(t_disclaimer.getGlobalBounds().width/2,t_disclaimer.getGlobalBounds().height/2);

}
void OptionsMenu::EventFired(sf::Event event){
    if(event.type == sf::Event::KeyPressed)
    {
        // do something here;
        buttonList.KeyPressedEvent(event);
        if(event.key.code==sf::Keyboard::Escape) {
            thisConfig->debugOut->DebugMessage("Returning to main menu...");
            Back();
        }
    } else if (event.type == sf::Event::MouseButtonReleased){
        // We use mouse released so a user can change their mind by keeping the mouse held and moving away.
        buttonList.MouseReleasedEvent(event);
    }
}
void OptionsMenu::Update(sf::RenderWindow &window, float fps)
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

        t_title.setPosition(window.getSize().x/2,100);
        t_disclaimer.setPosition(window.getSize().x/2,550);
        window.draw(t_title);
        if (madeChanges){
            window.draw(t_disclaimer);
        }
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        auto lastView = window.getView();
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos,lastView);

        //t_pressToContinue.setPosition(window.getSize().x/2,300);
        //window.draw(t_pressToContinue);

        buttonList.Update(window, fps, &worldPos);
        window.setView(window.getDefaultView());
    }

}
void OptionsMenu::Back(){
    /// this should go back to the previous menu.
    Hide();
    parentMenu->Show();
    OnExit();
}
void OptionsMenu::OnExit(){
    /// when we exit the options menu, we should save the user's settings to a text file

}
void OptionsMenu::UpdateButtons(){
    /// this should update the text on all the buttons
    buttonList.UpdateButtons();
}
void OptionsMenu::Show(){
    Menu::Show();
    buttonList.Show();
    t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"menu_button_3")));
    t_title.setOrigin(t_title.getGlobalBounds().width/2,t_title.getGlobalBounds().height/2);
    t_disclaimer.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"option_disclaimer")));
    t_disclaimer.setOrigin(t_disclaimer.getGlobalBounds().width/2,t_disclaimer.getGlobalBounds().height/2);
}
OptionsMenu::~OptionsMenu()
{
    //dtor
}
