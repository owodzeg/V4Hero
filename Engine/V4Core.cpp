#include <iostream>
#include <cstdlib>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <windows.h>
#include "V4Core.h"

using namespace std;

inline bool exists(const std::string& name)
{
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

V4Core::V4Core()
{
    std::cout << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;

    ofstream dbg("V4Hero-"+hero_version+"-latest.log", ios::trunc);
    dbg.close();

    rpc_details = "Running Patafour "+hero_version;
    SaveToDebugLog(rpc_details);

    const unsigned int maxSize = sf::Texture::getMaximumSize();
    cout << "[Debug] Max texture size: " << maxSize << endl;
    SaveToDebugLog("[GPU] Max texture size: "+to_string(maxSize));

    sf::RenderTexture rtx;
    cout << "[Debug] Maximum antialiasing level: " << rtx.getMaximumAntialiasingLevel() << endl;
    SaveToDebugLog("[GPU] Maximum antialiasing level: "+to_string(rtx.getMaximumAntialiasingLevel()));

    if(!exists("WINE"))
    {
        system("systeminfo > syslog.txt");
        ifstream sl("syslog.txt");
        string buff;

        while(getline(sl, buff))
        {
            if(buff.find("OS Name:") != std::string::npos)
            SaveToDebugLog(buff);
            else if(buff.find("OS Version:") != std::string::npos)
            SaveToDebugLog(buff);
            else if(buff.find("System Manufacturer:") != std::string::npos)
            SaveToDebugLog(buff);
            else if(buff.find("System Model:") != std::string::npos)
            SaveToDebugLog(buff);
            else if(buff.find("System Type:") != std::string::npos)
            SaveToDebugLog(buff);
            else if(buff.find("Processor(s):") != std::string::npos)
            SaveToDebugLog(buff);
            else if((buff.find("Family") != std::string::npos) && (buff.find("Model") != std::string::npos) && (buff.find("Stepping") != std::string::npos))
            SaveToDebugLog(buff);
            else if(buff.find("Total Physical Memory:") != std::string::npos)
            SaveToDebugLog(buff);
        }

        sl.close();

        system("del syslog.txt");
    }

    auto result = discord::Core::Create(712761245752623226, DiscordCreateFlags_NoRequireDiscord, &core);
    state.core.reset(core);
    if (!state.core) {
        std::cout << "Failed to instantiate discord core! (err " << static_cast<int>(result)
                  << ")\n";
    }

    if(state.core)
    {
        discord::Activity activity{};
        activity.SetDetails(rpc_details.c_str());
        activity.SetState("In Main menu");
        activity.GetAssets().SetLargeImage("logo");
        activity.SetType(discord::ActivityType::Playing);
        state.core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
            std::cout << ((result == discord::Result::Ok) ? "Succeeded" : "Failed")
                      << " updating activity!\n";
        });
    }

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

    t_fps.setFont(f_font);
    t_fps.setCharacterSize(24);
    t_fps.setFillColor(sf::Color(255,255,255,96));
    t_fps.setOutlineColor(sf::Color(0,0,0,96));
    t_fps.setOutlineThickness(1);
    t_fps.setString("FPS: ");

    /** Initialize main menu **/
    tipsUtil.LoadBackgrounds(config);
    tipsUtil.LoadIcons(config);
    tipsUtil.LoadStrings(config);
    mainMenu.Initialise(&config,this);

    menus.push_back(&mainMenu);
    config.configDebugID = 10;
}

void V4Core::SaveToDebugLog(string data)
{
    ofstream dbg("V4Hero-"+hero_version+"-latest.log", ios::app);
    dbg << data;
    dbg << "\r\n";
    dbg.close();
}

void V4Core::ChangeRichPresence(string title, string bg_image, string sm_image)
{
    if(state.core)
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
}

void V4Core::LoadingWaitForKeyPress()
{
    bool biff = true;
    pressAnyKey = true;

    while (biff)
    {
        Sleep(16); ///force it 60fps

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                biff = false;
            }

            ///Cannot use input controller here because this while loop completely blocks the other event access from happening
            if((event.type == sf::Event::KeyPressed) || (event.type == sf::Event::JoystickButtonPressed))
            {
                biff=false;
                pressAnyKey = false;
                continueLoading = false;
            }

        }
    }
}
void V4Core::LoadingThread()
{
    ChangeRichPresence("Reading tips", "logo", "");

    //sf::Context context;
    window.setActive(true);
    window.draw(t_version);
    window.clear();
    window.display();

    srand(time(NULL));

    float resRatioX = window.getSize().x / float(1280);
    float resRatioY = window.getSize().y / float(720);

    sf::RectangleShape box_1,box_2;
    box_1.setSize(sf::Vector2f(1280*resRatioX, 80*resRatioY));
    box_2.setSize(sf::Vector2f(1280*resRatioX, 514*resRatioY));

    PSprite tip_logo;
    tip_logo.loadFromFile("resources/graphics/ui/tips/tip-logo.png", config.GetInt("textureQuality"), 1);

    PSprite loading_head, loading_eye1, loading_eye2;
    loading_head.loadFromFile("resources/graphics/ui/tips/loading_head.png", config.GetInt("textureQuality"), 1);
    loading_eye1.loadFromFile("resources/graphics/ui/tips/loading_eye.png", config.GetInt("textureQuality"), 1);
    loading_eye2.loadFromFile("resources/graphics/ui/tips/loading_eye.png", config.GetInt("textureQuality"), 1);

    loading_eye1.setOrigin(loading_eye1.getLocalBounds().width*0.85, loading_eye1.getLocalBounds().height*0.85);
    loading_eye2.setOrigin(loading_eye2.getLocalBounds().width*0.85, loading_eye2.getLocalBounds().height*0.85);

    ///20 from top
    ///80 box1
    ///20 gap
    ///480 box2
    ///20 from bottom
    ///66 for floor

    box_1.setPosition(0,20*resRatioY);
    box_2.setPosition(0,120*resRatioY);

    box_1.setFillColor(sf::Color(0,0,0,192));
    box_2.setFillColor(sf::Color(0,0,0,192));

    int tipBackground = rand() % tipsUtil.t_backgrounds.size();
    int tipIcon = rand() % tipsUtil.t_icons.size();
    int tipText = (rand() % tipsUtil.tip_amount)+1;

    string title_key = "tip"+to_string(tipText)+"_title";
    string desc_key = "tip"+to_string(tipText)+"_desc";

    wstring wtitle_key(title_key.begin(), title_key.end());
    wstring wdesc_key(desc_key.begin(), desc_key.end());

    PText t_tipTitle;
    t_tipTitle.createText(f_font, 48, sf::Color(255,255,255,255), Func::ConvertToUtf8String(config.strRepo.GetUnicodeString(wtitle_key)), config.GetInt("textureQuality"), 1);

    std::string str_tipText = Func::ConvertToUtf8String(config.strRepo.GetUnicodeString(wdesc_key));
    //for(int t=0; t<str_tipText.size(); t++)
    //{
    //    if(str_tipText[t] == '\\')
    //    str_tipText[t] = '\n';
    //}

    PText t_tipText;
    t_tipText.createText(f_font, 32, sf::Color(255,255,255,255), str_tipText, config.GetInt("textureQuality"), 1);

    PText t_pressAnyKey;
    t_pressAnyKey.createText(f_font, 46, sf::Color(255,255,255,255), Func::ConvertToUtf8String(config.strRepo.GetUnicodeString(L"tips_anykey")), config.GetInt("textureQuality"), 1);

    PText t_nowLoading;
    t_nowLoading.createText(f_font, 46, sf::Color(255,255,255,255), Func::ConvertToUtf8String(config.strRepo.GetUnicodeString(L"tips_loading")), config.GetInt("textureQuality"), 1);

    float maxFps = config.GetInt("framerateLimit");

    if(maxFps == 0)
    maxFps = 240;

    while (continueLoading)
    {
        Sleep(1000/maxFps);

        window.clear();
        auto lastView = window.getView();
        window.setView(window.getDefaultView());

        tipsUtil.t_backgrounds[tipBackground].setPosition(0,0);
        tipsUtil.t_backgrounds[tipBackground].draw(window);

        window.draw(box_1);
        window.draw(box_2);

        tip_logo.setPosition(1060,20);
        tip_logo.draw(window);

        tipsUtil.t_icons[tipIcon].setOrigin(tipsUtil.t_icons[tipIcon].getLocalBounds().width/2, tipsUtil.t_icons[tipIcon].getLocalBounds().height/2);
        tipsUtil.t_icons[tipIcon].setPosition(1040,380);
        tipsUtil.t_icons[tipIcon].draw(window);

        t_tipTitle.setPosition(24,32);
        t_tipTitle.draw(window);

        t_tipText.setPosition(24,130);
        t_tipText.draw(window);

        // drawing some text
        if(pressAnyKey)
        {
            t_pressAnyKey.setPosition(722,658);
            t_pressAnyKey.draw(window);
        }
        else
        {
            loading_head.setPosition(670+230,656);
            loading_eye1.setPosition(689+230,699);
            loading_eye1.setRotation(loading_eye1.angle+(5.0 / maxFps));
            loading_head.draw(window);
            loading_eye1.draw(window);

            loading_head.setPosition(985+230,656);
            loading_eye2.setPosition(1004+230,699);
            loading_eye2.setRotation(loading_eye2.angle-(5.0 / maxFps));
            loading_head.draw(window);
            loading_eye2.draw(window);

            t_nowLoading.setPosition(722+230,658);
            t_nowLoading.draw(window);
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
    window.setVerticalSyncEnabled(config.GetInt("verticalSync"));

    framerateLimit = config.GetInt("framerateLimit");
    if(framerateLimit == 0)
    framerateLimit = 1000;

    inputCtrl.LoadKeybinds(config);

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
                SaveToDebugLog("[DEBUG] Key pressed: "+to_string(event.key.code));

                inputCtrl.keyRegistered = true;
                inputCtrl.currentKey = event.key.code;
                inputCtrl.keyMap[event.key.code] = true;
                inputCtrl.keyMapHeld[event.key.code] = true;
            }

            if(event.type == sf::Event::KeyReleased)
            {
                cout << "[DEBUG] Key released: " << event.key.code << endl;
                SaveToDebugLog("[DEBUG] Key released: "+to_string(event.key.code));

                inputCtrl.keyMapHeld[event.key.code] = false;
            }

            /** Joystick buttons need to be somewhat manually assigned **/

            if (event.type == sf::Event::JoystickButtonPressed)
            {
                if(event.joystickButton.joystickId == 0)
                {
                    std::cout << "[DEBUG] Joystick (" << event.joystickButton.joystickId << ") key pressed: " << event.joystickButton.button << std::endl;

                    inputCtrl.keyRegistered = true;
                    inputCtrl.currentKey = 1000+event.joystickButton.button;
                    inputCtrl.keyMap[1000+event.joystickButton.button] = true;
                    inputCtrl.keyMapHeld[1000+event.joystickButton.button] = true;
                }
            }

            if(event.type == sf::Event::JoystickButtonReleased)
            {
                if(event.joystickButton.joystickId == 0)
                {
                    std::cout << "[DEBUG] Joystick (" << event.joystickButton.joystickId << ") key pressed: " << event.joystickButton.button << std::endl;

                    inputCtrl.keyMapHeld[1000+event.joystickButton.button] = false;
                }
            }

            if(event.type == sf::Event::JoystickMoved)
            {
                if(event.joystickMove.joystickId == 0)
                {
                    if (event.joystickMove.axis == sf::Joystick::PovX)
                    {
                        if(event.joystickMove.position == -100) ///left
                        {
                            inputCtrl.keyRegistered = true;
                            inputCtrl.currentKey = 1100;
                            inputCtrl.keyMap[1100] = true;
                            inputCtrl.keyMapHeld[1100] = true;
                        }
                        else
                        {
                            inputCtrl.keyMapHeld[1100] = false;
                        }

                        if(event.joystickMove.position == 100) ///right
                        {
                            inputCtrl.keyRegistered = true;
                            inputCtrl.currentKey = 1101;
                            inputCtrl.keyMap[1101] = true;
                            inputCtrl.keyMapHeld[1101] = true;
                        }
                        else
                        {
                            inputCtrl.keyMapHeld[1101] = false;
                        }
                    }

                    if (event.joystickMove.axis == sf::Joystick::PovY)
                    {
                        if(event.joystickMove.position == -100) ///down
                        {
                            inputCtrl.keyRegistered = true;
                            inputCtrl.currentKey = 1102;
                            inputCtrl.keyMap[1102] = true;
                            inputCtrl.keyMapHeld[1102] = true;
                        }
                        else
                        {
                            inputCtrl.keyMapHeld[1102] = false;
                        }

                        if(event.joystickMove.position == 100) ///up
                        {
                            inputCtrl.keyRegistered = true;
                            inputCtrl.currentKey = 1103;
                            inputCtrl.keyMap[1103] = true;
                            inputCtrl.keyMapHeld[1103] = true;
                        }
                        else
                        {
                            inputCtrl.keyMapHeld[1103] = false;
                        }
                    }
                }
            }

            mainMenu.EventFired(event);
        }

        fps = float(1000000) / fpsclock.getElapsedTime().asMicroseconds();
        float rawFps = fps;
        frameTimes.push_back(fps);
        fpsclock.restart();

        auto n = frameTimes.size();
        float average = 0.0f;
        if(n != 0)
        {
             average = accumulate(frameTimes.begin(), frameTimes.end(), 0.0) / (n-1);
        }

        if(fps <= 1)
        fps = average;
        else
        fps = rawFps;

        while(frameTimes.size() > framerateLimit)
        frameTimes.erase(frameTimes.begin());

        //cout << fps << endl;

        window.clear();

        mainMenu.Update(window,fps,inputCtrl);

        auto lastView = window.getView();
        window.setView(window.getDefaultView());

        t_version.setPosition(4,4);
        window.draw(t_version);

        if(config.GetInt("showFPS"))
        {
            t_fps.setString("FPS: "+to_string(int(ceil(rawFps))));
            t_fps.setOrigin(t_fps.getLocalBounds().width, 0);
            t_fps.setPosition(window.getSize().x-4, 4);
            window.draw(t_fps);
        }

        window.display();

        window.setView(lastView);

        ///Clear the key inputs
        inputCtrl.Flush();

        if(closeWindow)
        {
            window.close();
        }

        if(state.core)
        state.core->RunCallbacks();
    }
    cout<<"Main game loop exited. Shutting down..."<<endl;
}
