#include <iostream>
#include <cstdlib>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <windows.h>
#include "V4Core.h"

using namespace std;

V4Core::V4Core()
{
    const unsigned int maxSize = sf::Texture::getMaximumSize();
    cout << "[Debug] Max texture size: " << maxSize << endl;

    sf::RenderTexture rtx;
    cout << "[Debug] Maximum antialiasing level: " << rtx.getMaximumAntialiasingLevel() << endl;

    rpc_details = "Running Patafour "+hero_version;

    auto result = discord::Core::Create(712761245752623226, DiscordCreateFlags_Default, &core);
    state.core.reset(core);
    if (!state.core) {
        std::cout << "Failed to instantiate discord core! (err " << static_cast<int>(result)
                  << ")\n";
        std::exit(-1);
    }

    discord::Activity activity{};
    activity.SetDetails(rpc_details.c_str());
    activity.SetState("In Main menu");
    activity.GetAssets().SetLargeImage("logo");
    activity.SetType(discord::ActivityType::Playing);
    state.core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
        std::cout << ((result == discord::Result::Ok) ? "Succeeded" : "Failed")
                  << " updating activity!\n";
    });

    /** Detect when the build was compiled **/

    time_t t;

    struct stat res;
    if(stat("Patafour.exe", &res) == 0)
    {
        t = res.st_mtime;
    }

    //struct tm *st = localtime(&t);

    /*//int day = st->tm_mday;
    //int month = st->tm_mon;
    //int year = st->tm_year + 1900;

    //vector<string> months = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    //string strDay = to_string(day);

    if((day > 9) && (day < 20))
    {
        strDay += "th";
    }
    else
    {
        if(day%10 == 1)
        strDay += "st";
        else if(day%10 == 2)
        strDay += "nd";
        else if(day%10 == 3)
        strDay += "rd";
        else
        strDay += "th";
    }
    */

    /** Load config from config.cfg **/
    config.LoadConfig(this);

    /** Load save from savereader **/
    savereader.LoadSave(config);
    /** "Alpha release" text **/

    f_font.loadFromFile("resources/fonts/p4kakupop-gothic.ttf");

    t_debug.setFont(f_font);
    t_debug.setCharacterSize(24);
    t_debug.setFillColor(sf::Color::White);
    t_debug.setString(config.strRepo.GetString(L"demo_string"));//+strDay+" "+months[month]+" "+to_string(year)+".");
    t_debug.setOrigin(t_debug.getGlobalBounds().width/2,t_debug.getGlobalBounds().height/2);

    t_version.setFont(f_font);
    t_version.setCharacterSize(24);
    t_version.setFillColor(sf::Color(255,255,255,32));
    t_version.setString("V4Hero Client "+hero_version);

    t_pressAnyKey.setFont(f_font);
    t_pressAnyKey.setCharacterSize(42);
    t_pressAnyKey.setFillColor(sf::Color(255,255,255,255));
    t_pressAnyKey.setString("Press any key to continue...");

    /** Initialize main menu **/
    tipsUtil.LoadBackgrounds(config);
    tipsUtil.LoadIcons(config);
    tipsUtil.LoadStrings(config);
    mainMenu.Initialise(&config,&keyMap,this);

    menus.push_back(&mainMenu);
    config.configDebugID = 10;

    /// If this is a new save (no previous save data) we load up the new game menu
    newGameMenu.Initialise(&config,&keyMap,this);
    menus.push_back(&newGameMenu);
    if(savereader.isNewSave)
    {
        mainMenu.Hide();
    }
    else
    {
        newGameMenu.Hide();
    }
}

void V4Core::ChangeRichPresence(string title, string bg_image, string sm_image)
{
    if(rpc_current != title)
    {
        rpc_current = title;

        discord::Activity activity{};
        activity.SetDetails(rpc_details.c_str());
        activity.SetState(title.c_str());
        activity.GetAssets().SetLargeImage(bg_image.c_str());
        activity.GetAssets().SetSmallImage(sm_image.c_str());
        activity.SetType(discord::ActivityType::Playing);
        state.core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
            std::cout << ((result == discord::Result::Ok) ? "Succeeded" : "Failed")
                      << " updating activity\n";
        });
    }
}

void V4Core::LoadingWaitForKeyPress()
{
    bool biff = true;
    pressAnyKey = true;
    while (biff)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::KeyPressed)
            {
                cout << "[DEBUG] Key released: " << event.key.code << endl;
                //keyMapHeld[event.key.code] = false;
                biff=false;
                pressAnyKey = false;
                continueLoading = false;
            }

        }
    }
}
void V4Core::LoadingThread()
{
    ChangeRichPresence("Watching tips", "logo", "");

    sf::Context context;
    window.setActive(true);
    window.draw(t_version);
    window.clear();
    window.display();
    int i=0;
    srand (time(NULL));
    int tipBackground = rand() % tipsUtil.t_backgrounds.size();
    int tipIcon = rand() % tipsUtil.t_icons.size();
    int tipText = rand() % tipsUtil.tipTitles.size();
    int ScrWidth = 3840;
    int ScrHeight = 2160;

    int WScrWidth = config.GetInt("resX");
    int WScrHeight = config.GetInt("resY");


    sf::Text t_tipTitle = sf::Text();
    t_tipTitle.setFont(f_font);
    t_tipTitle.setCharacterSize(42*config.GetInt("resX")/1280);
    t_tipTitle.setFillColor(sf::Color(255,255,255,255));
    t_tipTitle.setString(tipsUtil.tipTitles[tipText]);
    std::vector<std::string> lines = Func::Split(tipsUtil.tipTexts[tipText],'\\');
    std::vector<sf::Text> t_tipTextLines;
    int textSize = 28*config.GetInt("resX")/1280;
    for (auto it = lines.begin();it<lines.end();++it){
        sf::Text t_tipText = sf::Text();
        t_tipText.setFont(f_font);
        t_tipText.setCharacterSize(textSize);
        t_tipText.setFillColor(sf::Color(255,255,255,255));
        t_tipText.setString(*it);
        t_tipTextLines.push_back(t_tipText);
    }


    while (continueLoading)
    {
        i++;
        window.clear();
        auto lastView = window.getView();
        window.setView(window.getDefaultView());

        // drawing some text
        if (!pressAnyKey){
            t_version.setPosition(config.GetInt("resX")/2-50-100*sin(i/50.0),config.GetInt("resY")/2-20-100*sin((i+30)/50.0));
            tipsUtil.t_backgrounds[tipBackground].setPosition(0,0);
            tipsUtil.t_backgrounds[tipBackground].draw(window);

            tipsUtil.t_icons[tipIcon].setPosition((ScrWidth*3)/4,ScrHeight/4);
            tipsUtil.t_icons[tipIcon].draw(window);
            t_tipTitle.setPosition(24,42*WScrWidth/1280);
            window.draw(t_tipTitle);
            for (int i = 0;i<t_tipTextLines.size();++i){
                t_tipTextLines[i].setPosition(24,152*WScrWidth/1280+(textSize+4)*i);
                window.draw(t_tipTextLines[i]);
            }
            window.draw(t_version);
        } else {
            t_pressAnyKey.setPosition(12,config.GetInt("resY")-54);
            tipsUtil.t_backgrounds[tipBackground].setPosition(0,0);
            tipsUtil.t_backgrounds[tipBackground].draw(window);

            tipsUtil.t_icons[tipIcon].setPosition((ScrWidth*3)/4,ScrHeight/4);
            tipsUtil.t_icons[tipIcon].draw(window);

            t_tipTitle.setPosition(24,42*WScrWidth/1280);
            window.draw(t_tipTitle);

            for (int i = 0;i<t_tipTextLines.size();++i){
                t_tipTextLines[i].setPosition(24,152*WScrWidth/1280+(textSize+4)*i);
                window.draw(t_tipTextLines[i]);
            }
            window.draw(t_version);
            window.draw(t_pressAnyKey);
        }

        window.setView(lastView);
        window.display();

    }

    window.setActive(false);



}
void V4Core::ShowTip()
{
    //loadingThreadInstance = sf::Thread(LoadingThread);
    //loadingThreadInstance.launch();
    continueLoading=true;

}
void V4Core::Init()
{
    /// turned off because it doesn't work for owocek
    // DisableProcessWindowsGhosting();
    srand(time(NULL));

    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;

    if(config.GetInt("enableFullscreen"))
        window.create(sf::VideoMode(config.GetInt("resX"), config.GetInt("resY")), "Patafour", sf::Style::Fullscreen, settings);
    else if(config.GetInt("enableBorderlessWindow"))
        window.create(sf::VideoMode(config.GetInt("resX"), config.GetInt("resY")), "Patafour", sf::Style::None, settings);
    else
        window.create(sf::VideoMode(config.GetInt("resX"), config.GetInt("resY")), "Patafour", sf::Style::Titlebar | sf::Style::Close, settings);

    window.setFramerateLimit(config.GetInt("framerateLimit"));
    window.setKeyRepeatEnabled(false);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::KeyPressed)
            {
                ///keyMap[event.key.code] = true/false??? would that do the trick?
                cout << "[DEBUG] Key pressed: " << event.key.code << endl;

                keyMap[event.key.code] = true;
                keyMapHeld[event.key.code] = true;

                //if (!inMission){
                //inMission=true;
                //currentController.StartMission();
                //} else if(event.key.code==59) {
                //    cout<<"Returning to main menu...";
                //    inMission=false;
                //}
            }

            if(event.type == sf::Event::KeyReleased)
            {
                cout << "[DEBUG] Key released: " << event.key.code << endl;
                keyMapHeld[event.key.code] = false;
            }

            /**
            Joystick map (x360, ds4 is turned off by default, unsure about custom joysticks)
            0 - square
            1 - cross
            2 - circle
            3 - triangle
            4 - L1
            5 - R1
            6 - L2
            7 - R2
            8 - share (start)
            9 - options (select)
            10 - L3 (analog)
            11 - R3 (analog)
            12 - PS/XB button
            13 - DS4 touchpad
            **/

            if (event.type == sf::Event::JoystickButtonPressed)
            {
                if(event.joystickButton.joystickId == 0)
                {
                    std::cout << "[DEBUG] Joystick (" << event.joystickButton.joystickId << ") key pressed: " << event.joystickButton.button << std::endl;

                    /// TEMPORARY SOLUTION! Make joysticks mappable in future, just like keyboard. //
                    switch(event.joystickButton.button)
                    {
                    case 0:
                    {
                        keyMap[config.GetInt("keybindPata")] = true;
                        keyMapHeld[config.GetInt("keybindPata")] = true;
                        break;
                    }

                    case 1:
                    {
                        keyMap[config.GetInt("keybindDon")] = true;
                        keyMapHeld[config.GetInt("keybindDon")] = true;
                        break;
                    }

                    case 2:
                    {
                        keyMap[config.GetInt("keybindPon")] = true;
                        keyMapHeld[config.GetInt("keybindPon")] = true;
                        break;
                    }

                    case 3:
                    {
                        keyMap[config.GetInt("keybindChaka")] = true;
                        keyMapHeld[config.GetInt("keybindChaka")] = true;
                        break;
                    }

                    case 4:
                    {
                        keyMap[sf::Keyboard::Q] = true;
                        keyMapHeld[sf::Keyboard::Q] = true;
                        break;
                    }

                    case 5:
                    {
                        keyMap[sf::Keyboard::E] = true;
                        keyMapHeld[sf::Keyboard::E] = true;
                        break;
                    }
                    }
                }
            }

            if (event.type == sf::Event::JoystickButtonReleased)
            {
                if(event.joystickButton.joystickId == 0)
                {
                    std::cout << "[DEBUG] Joystick (" << event.joystickButton.joystickId << ") key released: " << event.joystickButton.button << std::endl;

                    /// TEMPORARY SOLUTION! Make joysticks mappable in future, just like keyboard. //
                    switch(event.joystickButton.button)
                    {
                    case 0:
                    {
                        keyMapHeld[config.GetInt("keybindPata")] = false;
                        break;
                    }

                    case 1:
                    {
                        keyMapHeld[config.GetInt("keybindDon")] = false;
                        break;
                    }

                    case 2:
                    {
                        keyMapHeld[config.GetInt("keybindPon")] = false;
                        break;
                    }

                    case 3:
                    {
                        keyMapHeld[config.GetInt("keybindChaka")] = false;
                        break;
                    }

                    case 4:
                    {
                        keyMapHeld[sf::Keyboard::Q] = false;
                        break;
                    }

                    case 5:
                    {
                        keyMapHeld[sf::Keyboard::E] = false;
                        break;
                    }
                    }
                }
            }

            if (event.type == sf::Event::JoystickMoved)
            {
                if(event.joystickMove.joystickId == 0)
                {
                    if (event.joystickMove.axis == sf::Joystick::PovX)
                    {
                        if(event.joystickMove.position == -100)
                        {
                            cout << "left" << endl;
                        }

                        if(event.joystickMove.position == 100)
                        {
                            cout << "right" << endl;
                        }
                    }

                    if (event.joystickMove.axis == sf::Joystick::PovY)
                    {
                        if(event.joystickMove.position == -100)
                        {
                            cout << "down" << endl;
                        }

                        if(event.joystickMove.position == 100)
                        {
                            cout << "up" << endl;
                        }
                    }
                }
            }

            if (savereader.isNewSave)
            {
                newGameMenu.EventFired(event);
            }
            mainMenu.EventFired(event);
        }

        fps = float(1000000) / fpsclock.getElapsedTime().asMicroseconds();
        fpsclock.restart();

        //cout << fps << endl;

        window.clear();
        // Something important goes here ok cool thanks
        //if(inMission){
        //currentController.Update(window,fps);

        //} else {
        //if (savereader.isNewSave){
        //    newGameMenu.Update(window,fps,&keyMap,&keyMapHeld);
        //} else {
        mainMenu.Update(window,fps,&keyMap,&keyMapHeld);
        //}
        //}

        auto lastView = window.getView();
        window.setView(window.getDefaultView());

        //t_debug.setPosition(window.getSize().x/2,window.getSize().y-20);
        //window.draw(t_debug);

        t_version.setPosition(4,4);
        window.draw(t_version);
        window.display();

        window.setView(lastView);

        keyMap.clear();
        if(closeWindow)
        {
            window.close();
        }

        state.core->RunCallbacks();
    }
    cout<<"Main game loop exited. Shutting down..."<<endl;
}
