#include "Patapolis.h"
#include "ButtonList.h"
#include "iostream"
#include "../V4Core.h"
#include <sstream>
#include "Altar.h"
PatapolisMenu::PatapolisMenu()
{
    //ctor
    f_font.loadFromFile("resources/fonts/p4kakupop-pro.ttf");
    //f_font.loadFromFile("resources/fonts/arial.ttf");
    t_title.setFont(f_font);

    t_title.setCharacterSize(38);
    t_title.setFillColor(sf::Color::White);

    mm_bigBox.setSize(sf::Vector2f(100,10));
    mm_bigBox.setFillColor(sf::Color(4,0,90));

    mm_titleBox.setSize(sf::Vector2f(100,10));
    mm_titleBox.setFillColor(sf::Color::Red);


    isActive=false;
}
void PatapolisMenu::Initialise(Config *thisConfigs,std::map<int,bool> *keymap,V4Core *parent, Menu *curParentMenu){
    Scene::Initialise(thisConfigs,keymap,parent);
    optionsMenu.Initialise(thisConfigs,keymap,parent,this);
    altar_menu.Initialise(thisConfigs,keymap,parent,this);
    barracks_menu.Initialise(thisConfigs,keymap,parent,this);
    v4core->menus.push_back(&optionsMenu);
    parentMenu = curParentMenu;

    thisConfig->debugOut->DebugMessage("Patapolis loaded: Void_Spacer_Village.png");
    t_background.clear();
    s_background.clear();

    PSprite ps_temp;
    ps_temp.loadFromFile("resources/graphics/bg/city/Void_Spacer_Village.png",1);
    ps_temp.setRepeated(true);
    ps_temp.setTextureRect(sf::IntRect(0,0,500000,ps_temp.t.getSize().y)); ///affect later with ratio
    ps_temp.setOrigin(10000,ps_temp.getLocalBounds().height);
    ps_temp.setColor(sf::Color(255,255,255,255));
    ps_temp.setPosition(-1000,613);

    sf::Vector2f tmpp;

    tmpp.x = -1000;
    tmpp.y = 613;

    s_background.push_back(ps_temp);
    p_background.push_back(tmpp);
    background_xspeed.push_back(0);

    t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis")));
    t_title.setOrigin(t_title.getGlobalBounds().width/2,t_title.getGlobalBounds().height/2);

    possibleMenuPositions = {-2300, -1300,600,1500,3700};
    currentMenuPosition = -1;
}
void PatapolisMenu::EventFired(sf::Event event){
    if (altar_menu.isActive){
            altar_menu.EventFired(event);
    } else if (barracks_menu.isActive){
            barracks_menu.EventFired(event);
    } else if(isActive){
        if(event.type == sf::Event::KeyPressed)
        {
            // do something here;
            if (event.key.code == sf::Keyboard::Num1){

                    animStartVal=p_background[0].x;
                    animChangeVal=2300-p_background[0].x;
                    anim_timer.restart();
                    totalTime=1;
                    isAnim = true;
                    currentMenuPosition = 0;
                    SetTitle(currentMenuPosition);

            } else if (event.key.code == sf::Keyboard::Num2){

                    animStartVal=p_background[0].x;
                    animChangeVal=1300-p_background[0].x;
                    anim_timer.restart();
                    totalTime=1;
                    isAnim = true;
                    currentMenuPosition = 1;
                    SetTitle(currentMenuPosition);

            } else if (event.key.code == sf::Keyboard::Num3){

                    animStartVal=p_background[0].x;
                    animChangeVal=-p_background[0].x-600;
                    anim_timer.restart();
                    totalTime=1;
                    isAnim = true;
                    currentMenuPosition = 2;
                    SetTitle(currentMenuPosition);

            } else if (event.key.code == sf::Keyboard::Num4){

                    animStartVal=p_background[0].x;
                    animChangeVal=-p_background[0].x-1500;
                    anim_timer.restart();
                    totalTime=1;
                    isAnim = true;
                    currentMenuPosition = 3;
                    SetTitle(currentMenuPosition);

            } else if (event.key.code == sf::Keyboard::Num5){

                    animStartVal=p_background[0].x;
                    animChangeVal=-p_background[0].x-3700;
                    anim_timer.restart();
                    totalTime=1;
                    isAnim = true;
                    currentMenuPosition = 4;
                    SetTitle(currentMenuPosition);

            } else if (event.key.code == thisConfig->GetInt("keybindPon") || event.key.code == thisConfig->GetInt("secondaryKeybindPon")){
                    currentMenuPosition += 1;
                    if (currentMenuPosition>possibleMenuPositions.size()){
                        currentMenuPosition=0;
                    }
                    animStartVal=p_background[0].x;
                    animChangeVal=-p_background[0].x-possibleMenuPositions[currentMenuPosition];
                    anim_timer.restart();
                    totalTime=1;
                    isAnim = true;
                    SetTitle(currentMenuPosition);


            } else if (event.key.code == thisConfig->GetInt("keybindPata") || event.key.code == thisConfig->GetInt("secondaryKeybindPata")){
                    currentMenuPosition -= 1;
                    if (currentMenuPosition<0){
                        currentMenuPosition=4;
                    }
                    animStartVal=p_background[0].x;
                    animChangeVal=-p_background[0].x-possibleMenuPositions[currentMenuPosition];
                    anim_timer.restart();
                    totalTime=1;
                    isAnim = true;
                    SetTitle(currentMenuPosition);

            } else if (event.key.code == thisConfig->GetInt("keybindDon") || event.key.code == thisConfig->GetInt("secondaryKeybindDon") || event.key.code == thisConfig->GetInt("keybindMenuEnter")){
                    // select the current menu item
                    switch (currentMenuPosition){
                        case 0:
                            /// obelisk
                            // open the world map
                            break;
                        case 1:
                            /// altar
                            // open inventory menu
                            altar_menu.Show();
                            altar_menu.isActive = true;
                            altar_menu.ShowAltar();
                            break;
                        case 2:
                            /// armoury/barracks
                            // open barracks screen
                            barracks_menu.Show();
                            barracks_menu.isActive = true;
                            barracks_menu.OpenBarracksMenu();
                            break;
                        case 3:
                            /// mater tree
                            // open mater menu
                            break;
                        case 4:
                            /// cave
                            // idk?
                            break;
                        default:
                            /// nothing

                            break;
                    }

            } else if (event.key.code == thisConfig->GetInt("keybindBack")){
                this->Hide();
                this->isActive = false;
                parentMenu->Show();
                parentMenu->isActive=true;
            }


        } else if (event.type == sf::Event::MouseButtonReleased){
            // We use mouse released so a user can change their mind by keeping the mouse held and moving away.
        }
    }

}
void PatapolisMenu::SetTitle(int menuPosition){
    switch(menuPosition){
        case 0:
            t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_obelisk")));
            break;
        case 1:
            t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_altar")));
            break;
        case 2:
            t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_barracks")));
            break;
        case 3:
            t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_tree")));
            break;
        case 4:
            t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_cave")));
            break;
        default:
            t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis")));
            break;
    }
    t_title.setOrigin(t_title.getGlobalBounds().width/2,t_title.getGlobalBounds().height/2);
}
float EaseIn (float time, float startValue, float change, float duration) {
     time /= duration / 2;
     if (time < 1)  {
          return change / 2 * time * time + startValue;
     }

     time--;
     return -change / 2 * (time * (time - 2) - 1) + startValue;
};
void PatapolisMenu::Update(sf::RenderWindow &window, float fps)
{
    if (barracks_menu.isActive){
        barracks_menu.Update(window,fps);
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




        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        auto lastView = window.getView();
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos,lastView);

        //t_pressToContinue.setPosition(window.getSize().x/2,300);
        //window.draw(t_pressToContinue);


        window.setView(window.getDefaultView());
        if(anim_timer.getElapsedTime().asSeconds() >= totalTime)
        {
            isAnim = false;
        }
        if (isAnim){
            p_background[0].x = EaseIn(anim_timer.getElapsedTime().asSeconds(),animStartVal,animChangeVal,totalTime);
        }
        for(int i=0; i<s_background.size(); i++)
        {
            //s_background[i].setTexture(t_background[i]);

            s_background[i].setPosition(p_background[i].x,p_background[i].y);
            //cout << s_background[i].y << endl;
            s_background[i].draw(window);
        }
        if(altar_menu.isActive){
            altar_menu.Update(window,fps);
        } else {
            t_title.setPosition(window.getSize().x/2,110);
            window.draw(t_title);
        }
    }
}

void PatapolisMenu::UpdateButtons(){
    /// this should update the text on all the buttons
}
void PatapolisMenu::OnExit(){
    /// when we exit the main menu, we do nothing for now.
    /// perhaps we would want to unload sprites or songs etc
}
PatapolisMenu::~PatapolisMenu()
{
    //dtor
}
