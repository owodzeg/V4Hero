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
void PatapolisMenu::Initialise(Config *thisConfigs,std::map<int,bool> *keymap,V4Core *parent, Menu *curParentMenu)
{

    sf::Context context;
    Scene::Initialise(thisConfigs,keymap,parent);
    altar_menu.Initialise(thisConfigs,keymap,parent,this);
    barracks_menu.Initialise(thisConfigs,keymap,parent,this);
    obelisk_menu.Initialise(thisConfigs,keymap,parent,this);
    parentMenu = curParentMenu;
    quality = thisConfig->GetInt("textureQuality");
    float ratioX, ratioY;
    switch(quality)
    {
    case 0: ///low
    {
        ratioX = thisConfig->GetInt("resX") / float(640);
        ratioY = thisConfig->GetInt("resY") / float(360);
        break;
    }

    case 1: ///med
    {
        ratioX = thisConfig->GetInt("resX") / float(1280);
        ratioY = thisConfig->GetInt("resY") / float(720);
        break;
    }

    case 2: ///high
    {
        ratioX = thisConfig->GetInt("resX") / float(1920);
        ratioY = thisConfig->GetInt("resY") / float(1080);
        break;
    }

    case 3: ///ultra
    {
        ratioX = thisConfig->GetInt("resX") / float(3840);
        ratioY = thisConfig->GetInt("resY") / float(2160);
        break;
    }
    }

    thisConfig->debugOut->DebugMessage("Patapolis loaded: Patapolis_M.png");
    t_background.clear();
    s_background.clear();

    PSprite ps_temp;
    ps_temp.loadFromFile("resources/graphics/bg/city/Patapolis.png",1);
    ps_temp.setRepeated(true);
    ps_temp.setTextureRect(sf::IntRect(0,0,500000,ps_temp.t.getSize().y)); ///affect later with ratio
    ps_temp.setOrigin(10000,0);
    ps_temp.setColor(sf::Color(255,255,255,255));
    ps_temp.setPosition(-1000,2/ratioY);

    sf::Vector2f tmpp;
    //float xRatio = (thisConfig->GetInt("resX")/1600.0);
    tmpp.x = 2880;
    tmpp.y = 0*ratioY;

    s_background.push_back(ps_temp);
    s_background[0].scaleX=0.60f;
    s_background[0].scaleY=0.60f;
    p_background.push_back(tmpp);
    background_xspeed.push_back(0);

    t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_festival")));
    t_title.setOrigin(t_title.getGlobalBounds().width/2,t_title.getGlobalBounds().height/2);

    possibleMenuPositions = {-6000, -4000,-2880,-730,350};
    currentMenuPosition = 2;
    // write code here
    initialised=true;
    if (doWaitKeyPress)
    {
        v4core->LoadingWaitForKeyPress();
        v4core->ChangeRichPresence("In Patapolis", "logo", "");
    }
}
void PatapolisMenu::EventFired(sf::Event event)
{
    if (altar_menu.isActive)
    {
        altar_menu.EventFired(event);
    }
    else if (barracks_menu.isActive)
    {
        barracks_menu.EventFired(event);
    }
    else if (obelisk_menu.isActive)
    {
        obelisk_menu.EventFired(event);
    }
    else if(isActive)
    {
        if(event.type == sf::Event::KeyPressed)
        {
            // do something here;
            if (event.key.code == sf::Keyboard::Num1)
            {

                animStartVal=p_background[0].x;
                animChangeVal=6000-p_background[0].x;
                anim_timer.restart();
                totalTime=1;
                isAnim = true;
                currentMenuPosition = 0;
                SetTitle(currentMenuPosition);

            }
            else if (event.key.code == sf::Keyboard::Num2)
            {

                animStartVal=p_background[0].x;
                animChangeVal=4000-p_background[0].x;
                anim_timer.restart();
                totalTime=1;
                isAnim = true;
                currentMenuPosition = 1;
                SetTitle(currentMenuPosition);

            }
            else if (event.key.code == sf::Keyboard::Num3)
            {

                animStartVal=p_background[0].x;
                animChangeVal=2880-p_background[0].x;
                anim_timer.restart();
                totalTime=1;
                isAnim = true;
                currentMenuPosition = 2;
                SetTitle(currentMenuPosition);

            }
            else if (event.key.code == sf::Keyboard::Num4)
            {

                animStartVal=p_background[0].x;
                animChangeVal=730-p_background[0].x;
                anim_timer.restart();
                totalTime=1;
                isAnim = true;
                currentMenuPosition = 3;
                SetTitle(currentMenuPosition);

            }
            else if (event.key.code == sf::Keyboard::Num5)
            {

                animStartVal=p_background[0].x;
                animChangeVal=-p_background[0].x-350;
                anim_timer.restart();
                totalTime=1;
                isAnim = true;
                currentMenuPosition = 4;
                SetTitle(currentMenuPosition);

            }
            else if (event.key.code == thisConfig->GetInt("keybindPon") || event.key.code == thisConfig->GetInt("secondaryKeybindPon"))
            {
                currentMenuPosition += 1;
                if (currentMenuPosition>possibleMenuPositions.size()-1)
                {
                    currentMenuPosition=0;
                }
                animStartVal=p_background[0].x;
                animChangeVal=-p_background[0].x-possibleMenuPositions[currentMenuPosition];
                anim_timer.restart();
                totalTime=1;
                isAnim = true;
                SetTitle(currentMenuPosition);


            }
            else if (event.key.code == thisConfig->GetInt("keybindPata") || event.key.code == thisConfig->GetInt("secondaryKeybindPata"))
            {
                currentMenuPosition -= 1;
                if (currentMenuPosition<0)
                {
                    currentMenuPosition=4;
                }
                animStartVal=p_background[0].x;
                animChangeVal=-p_background[0].x-possibleMenuPositions[currentMenuPosition];
                anim_timer.restart();
                totalTime=1;
                isAnim = true;
                SetTitle(currentMenuPosition);

            }
            else if (event.key.code == thisConfig->GetInt("keybindDon") || event.key.code == thisConfig->GetInt("secondaryKeybindDon") || event.key.code == thisConfig->GetInt("keybindMenuEnter"))
            {
                // select the current menu item
                switch (currentMenuPosition)
                {
                case 0:
                    /// trader/random
                    // open the world map
                    break;
                case 1:
                    /// armoury/barracks
                    barracks_menu.Show();
                    barracks_menu.isActive = true;
                    barracks_menu.OpenBarracksMenu();
                    break;
                case 2:
                    /// festival
                    // open barracks screen
                    break;
                case 3:
                    /// altar
                    // open mater menu
                    altar_menu.Show();
                    altar_menu.isActive = true;
                    altar_menu.ShowAltar();
                    break;
                case 4:
                    /// obelisk
                    // idk?
                    // open inventory menu
                    obelisk_menu.Show();
                    obelisk_menu.isActive = true;
                    break;
                default:
                    /// nothing

                    break;
                }

            }
            else if (event.key.code == thisConfig->GetInt("keybindBack"))
            {
                this->Hide();
                this->isActive = false;
                parentMenu->Show();
                parentMenu->isActive=true;
            }


        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            // We use mouse released so a user can change their mind by keeping the mouse held and moving away.
        }
    }

}
void PatapolisMenu::SetTitle(int menuPosition)
{
    switch(menuPosition)
    {
    case 0:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_trader")));
        break;
    case 1:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_barracks")));
        break;
    case 2:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_festival")));
        break;
    case 3:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_altar")));
        break;
    case 4:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_obelisk")));
        break;
    default:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis")));
        break;
    }
    t_title.setOrigin(t_title.getGlobalBounds().width/2,t_title.getGlobalBounds().height/2);
}
float EaseIn (float time, float startValue, float change, float duration)
{
    time /= duration / 2;
    if (time < 1)
    {
        return change / 2 * time * time + startValue;
    }

    time--;
    return -change / 2 * (time * (time - 2) - 1) + startValue;
};
void PatapolisMenu::Update(sf::RenderWindow &window, float fps)
{
    if(isActive)
    {

        window.setView(window.getDefaultView());
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
        if (isAnim)
        {
            p_background[0].x = EaseIn(anim_timer.getElapsedTime().asSeconds(),animStartVal,animChangeVal,totalTime);
        }
        for(int i=0; i<s_background.size(); i++)
        {
            //s_background[i].setTexture(t_background[i]);

            s_background[i].setPosition(p_background[i].x,p_background[i].y);
            //cout << s_background[i].y << endl;
            s_background[i].draw(window);
        }
        if (barracks_menu.isActive)
        {
            barracks_menu.Update(window,fps);
        }
        else if(altar_menu.isActive)
        {
            altar_menu.Update(window,fps);
        }
        else if(obelisk_menu.isActive)
        {
            obelisk_menu.Update(window,fps);
        }
        else
        {
            t_title.setPosition(window.getSize().x/2,110);
            window.draw(t_title);
        }
    }
}

void PatapolisMenu::UpdateButtons()
{
    /// this should update the text on all the buttons
}
void PatapolisMenu::OnExit()
{
    /// when we exit the main menu, we do nothing for now.
    /// perhaps we would want to unload sprites or songs etc
}
PatapolisMenu::~PatapolisMenu()
{
    //dtor
}
