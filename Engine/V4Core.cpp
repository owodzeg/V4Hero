#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "V4Core.h"
#include "CoreManager.h"
#include "StateManager.h"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <random>
#include <sys/stat.h>
#include <sys/types.h>
#include <thread>
#include <time.h>

using namespace std;

inline bool exists(const std::string& name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

V4Core::V4Core()
{
    rpc_details = "Running Patafour " + hero_version;

    const unsigned int maxSize = sf::Texture::getMaximumSize();
    SPDLOG_DEBUG("Max allowed texture size: {}", maxSize);
    sf::RenderTexture rtx;
    SPDLOG_DEBUG("Max antialiasing level: {}", rtx.getMaximumAntialiasingLevel());

    /*auto result = discord::Core::Create(712761245752623226, DiscordCreateFlags_NoRequireDiscord, &core);
    state.core.reset(core);
    if (!state.core) {
        std::cout << "Failed to instantiate discord core! (err " << static_cast<int>(result)
                  << ")\n";
    }*/

    /*if(state.core)
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
    }*/

    /** Detect when the build was compiled **/

    time_t t;

    struct stat res;
    if (stat("Patafour.exe", &res) == 0)
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
    Config* config = CoreManager::getInstance().getConfig();
    config->LoadConfig(this);

    /** Apply logging level from config **/
    switch (config->GetInt("logLevel")) //i can't get int to convert to a spdlog::set_level argument, so i'm making a switch
    {
        case 0: {
            spdlog::set_level(spdlog::level::trace);
            SPDLOG_INFO("Logging level set to TRACE");
            break;
        }
        case 1: {
            spdlog::set_level(spdlog::level::debug);
            SPDLOG_INFO("Logging level set to DEBUG");
            break;
        }
        case 2: {
            spdlog::set_level(spdlog::level::info);
            SPDLOG_INFO("Logging level set to INFO");
            break;
        }
    }

    /** Load Resource Manager **/
    ResourceManager::getInstance().getQuality();

    /** Initialize main menu **/
    SPDLOG_DEBUG("Load backgrounds from tipsUtil");
    //tipsUtil.LoadBackgrounds();
    SPDLOG_DEBUG("Load icons from tipsUtil");
    //tipsUtil.LoadIcons();

    config->configDebugID = 10;
}

void V4Core::changeRichPresence(string title, string bg_image, string sm_image)
{
    /*if(state.core)
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
    }*/
}

void V4Core::loadingWaitForKeyPress()
{
    bool biff = true;
    press_any_key = true;

    while (biff)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); ///force it 60fps

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                biff = false;
            }

            ///Cannot use input controller here because this while loop completely blocks the other event access from happening
            if ((event.type == sf::Event::KeyPressed) || (event.type == sf::Event::JoystickButtonPressed))
            {
                biff = false;
                press_any_key = false;
                continue_loading = false;
            }
        }
    }
}
void V4Core::loadingThread()
{
    changeRichPresence("Reading tips", "logo", "");

    //sf::Context context;
    window.setActive(true);
    window.draw(t_version);
    window.clear();
    window.display();

    // Seed RNG
    srand(time(NULL));
    random_device rd; // https://stackoverflow.com/questions/13445688
    seed = rd() ^ ((mt19937::result_type)
                           chrono::duration_cast<chrono::seconds>(
                                   chrono::system_clock::now().time_since_epoch())
                                   .count() +
                   (mt19937::result_type)
                           chrono::duration_cast<chrono::microseconds>(
                                   chrono::high_resolution_clock::now().time_since_epoch())
                                   .count());

    float resRatioX = window.getSize().x / float(1280);
    float resRatioY = window.getSize().y / float(720);

    sf::RectangleShape box_1, box_2;
    box_1.setSize(sf::Vector2f(1280 * resRatioX, 80 * resRatioY));
    box_2.setSize(sf::Vector2f(1280 * resRatioX, 514 * resRatioY));

    PSprite tip_logo = ResourceManager::getInstance().getSprite("resources/graphics/ui/tips/tip-logo.png");

    PSprite loading_head = ResourceManager::getInstance().getSprite("resources/graphics/ui/tips/loading_head.png");
    PSprite loading_eye1 = ResourceManager::getInstance().getSprite("resources/graphics/ui/tips/loading_eye.png");
    PSprite loading_eye2 = ResourceManager::getInstance().getSprite("resources/graphics/ui/tips/loading_eye.png");

    loading_eye1.setOrigin(loading_eye1.getLocalBounds().width * 0.85, loading_eye1.getLocalBounds().height * 0.85);
    loading_eye2.setOrigin(loading_eye2.getLocalBounds().width * 0.85, loading_eye2.getLocalBounds().height * 0.85);

    ///20 from top
    ///80 box1
    ///20 gap
    ///480 box2
    ///20 from bottom
    ///66 for floor

    box_1.setPosition(0, 20 * resRatioY);
    box_2.setPosition(0, 120 * resRatioY);

    box_1.setFillColor(sf::Color(0, 0, 0, 192));
    box_2.setFillColor(sf::Color(0, 0, 0, 192));

    int tipBackground = rand() % tipsUtil.backgroundFileNames.size();
    int tipIcon = rand() % tipsUtil.iconFileNames.size();
    int tipText = (rand() % tipsUtil.tipAmount) + 1;

    string title_key = "tip" + to_string(tipText) + "_title";
    string desc_key = "tip" + to_string(tipText) + "_desc";

    string wtitle_key(title_key.begin(), title_key.end());
    string wdesc_key(desc_key.begin(), desc_key.end());

    PText t_tipTitle;
    //t_tipTitle.createText(f_font, 48, sf::Color(255, 255, 255, 255), Func::ConvertToUtf8String(config.strRepo.GetString(wtitle_key)), config.GetInt("textureQuality"), 1);

    //sf::String str_tipText = Func::ConvertToUtf8String(config.strRepo.GetString(wdesc_key));
    //for(int t=0; t<str_tipText.size(); t++)
    //{
    //    if(str_tipText[t] == '\\')
    //    str_tipText[t] = '\n';
    //}

    PText t_tipText;
    //t_tipText.createText(f_font, 32, sf::Color(255, 255, 255, 255), str_tipText, config.GetInt("textureQuality"), 1);

    PText t_pressAnyKey;
    //t_pressAnyKey.createText(f_font, 46, sf::Color(255, 255, 255, 255), Func::ConvertToUtf8String(config.strRepo.GetString("tips_anykey")), config.GetInt("textureQuality"), 1);

    PText t_nowLoading;
    //t_nowLoading.createText(f_font, 46, sf::Color(255, 255, 255, 255), Func::ConvertToUtf8String(config.strRepo.GetString("tips_loading")), config.GetInt("textureQuality"), 1);

    std::string bg_key = "resources/graphics/ui/tips/" + tipsUtil.backgroundFileNames[tipBackground];
    PSprite s_bg = ResourceManager::getInstance().getSprite(bg_key);

    std::string icon_key = "resources/graphics/ui/tips/" + tipsUtil.iconFileNames[tipIcon];
    PSprite s_icon = ResourceManager::getInstance().getSprite(icon_key);

    float maxFps = 240;

    if (maxFps == 0)
        maxFps = 240;

    while (continue_loading)
    {
        int ms = round(1000 / maxFps);
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));

        window.clear();
        auto lastView = window.getView();
        window.setView(window.getDefaultView());

        s_bg.draw(window);

        window.draw(box_1);
        window.draw(box_2);

        tip_logo.setPosition(1060, 20);
        tip_logo.draw(window);

        s_icon.setOrigin(s_icon.getLocalBounds().width / 2, s_icon.getLocalBounds().height / 2);
        s_icon.setPosition(1040, 380);
        s_icon.draw(window);

        t_tipTitle.setPosition(24, 32);
        t_tipTitle.draw(window);

        t_tipText.setPosition(24, 130);
        t_tipText.draw(window);

        // drawing some text
        if (press_any_key)
        {
            t_pressAnyKey.setOrigin(t_pressAnyKey.getLocalBounds().width, t_pressAnyKey.getLocalBounds().height / 2);
            t_pressAnyKey.setPosition(722 + 526, 658 + 21);
            t_pressAnyKey.draw(window);
        } else
        {
            t_nowLoading.setOrigin(t_nowLoading.getLocalBounds().width, t_nowLoading.getLocalBounds().height / 2);
            t_nowLoading.setPosition(722 + 230 + 256, 658 + 26);
            t_nowLoading.draw(window);

            loading_head.setPosition(t_nowLoading.getPosition().x - t_nowLoading.getLocalBounds().width - 46, t_nowLoading.getPosition().y - 28);
            loading_eye1.setPosition(t_nowLoading.getPosition().x - t_nowLoading.getLocalBounds().width + 19 - 46, t_nowLoading.getPosition().y + 43 - 28);
            loading_eye1.setRotation(loading_eye1.angle + (5.0 / maxFps));
            loading_head.draw(window);
            loading_eye1.draw(window);

            loading_head.setPosition(t_nowLoading.getPosition().x + 12, t_nowLoading.getPosition().y - 28);
            loading_eye2.setPosition(t_nowLoading.getPosition().x + 19 + 12, t_nowLoading.getPosition().y + 43 - 28);
            loading_eye2.setRotation(loading_eye2.angle - (5.0 / maxFps));
            loading_head.draw(window);
            loading_eye2.draw(window);
        }

        window.setView(lastView);
        window.display();
    }

    window.setActive(false);
}

void V4Core::showTip()
{
    //loadingThreadInstance = sf::Thread(LoadingThread);
    //loadingThreadInstance.launch();
    continue_loading = true;
}

void V4Core::cacheEntity(int entityID, shared_ptr<vector<vector<sf::Image>>> swaps, shared_ptr<vector<AnimatedObject::Animation>> spritesheet, shared_ptr<vector<Object>> objects)
{
    SPDLOG_DEBUG("Caching entity with id {}", entityID);

    isCached[entityID] = true;

    animation_cache[entityID] = make_shared<AnimationCache>();
    animation_cache[entityID].get()->swaps = swaps;
    animation_cache[entityID].get()->spritesheet = spritesheet;
    //animation_cache[entityID].get()->objects = objects;

    SPDLOG_DEBUG("Cache created");
}

float V4Core::getFPS()
{
    return fps;
}

// Core function that runs the entirety of Patafour
void V4Core::init()
{
    /// turned off because it doesn't work for owocek
    // DisableProcessWindowsGhosting();
    
    // Create a random seed for all of the randomization 
    srand(time(NULL));
    random_device rd; // https://stackoverflow.com/questions/13445688
    seed = rd() ^ ((mt19937::result_type)
                           chrono::duration_cast<chrono::seconds>(
                                   chrono::system_clock::now().time_since_epoch())
                                   .count() +
                   (mt19937::result_type)
                           chrono::duration_cast<chrono::microseconds>(
                                   chrono::high_resolution_clock::now().time_since_epoch())
                                   .count());
    mt19937 tmp(seed);
    gen = tmp;

    // Set antialiasing level (hardcoded, apply from config)
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;

    // Create the game window
    SPDLOG_INFO("Creating window");
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    
    // Gather the Config pointer so we can get values from there
    Config* config = CoreManager::getInstance().getConfig();

    if (config->GetInt("enableFullscreen")) // open in fullscreen
        window->create(sf::VideoMode(config->GetInt("resX"), config->GetInt("resY")), "Patafour", sf::Style::Fullscreen, settings);
    else if (config->GetInt("enableBorderlessWindow")) // open as borderless window
        window->create(sf::VideoMode(config->GetInt("resX"), config->GetInt("resY")), "Patafour", sf::Style::None, settings);
    else // open as a regular window
        window->create(sf::VideoMode(config->GetInt("resX"), config->GetInt("resY")), "Patafour", sf::Style::Titlebar | sf::Style::Close, settings);

    // Get current framerate limit
    framerate_limit = config->GetInt("framerateLimit");
    
    // Despite having an "Unlimited" framerate option, we limit it to 500.
    if (framerate_limit == 0)
        framerate_limit = 500;
    if (framerate_limit > 500)
        framerate_limit = 500;

    // Apply window settings (fps limit, vsync)
    SPDLOG_INFO("Applying window settings");
    window->setFramerateLimit(framerate_limit);
    window->setKeyRepeatEnabled(false);
    window->setVerticalSyncEnabled(config->GetInt("verticalSync"));

    // Load language data and appropriate font
    SPDLOG_DEBUG("Loading language data");
    StringRepository* strRepo = CoreManager::getInstance().getStrRepo();
    strRepo->LoadLanguageFiles(config->GetInt("lang"));

    config->fontPath = "resources/fonts/" + strRepo->langFonts[config->GetInt("lang") - 1];

    /** "Alpha release" text **/
    SPDLOG_DEBUG("Creating text for version and FPS");
    f_font.loadFromFile(config->fontPath);

    t_version.setFont(f_font);
    t_version.setCharacterSize(24);
    t_version.setFillColor(sf::Color(255, 255, 255, 32));
    t_version.setString("V4Hero Client " + hero_version);

    t_fps.setFont(f_font);
    t_fps.setCharacterSize(24);
    t_fps.setFillColor(sf::Color(255, 255, 255, 96));
    t_fps.setOutlineColor(sf::Color(0, 0, 0, 96));
    t_fps.setOutlineThickness(1);
    t_fps.setString("FPS: ");

    // Load item registry
    SPDLOG_DEBUG("Loading item registry");
    SaveReader* saveReader = CoreManager::getInstance().getSaveReader();
    saveReader->itemReg.readItemFiles();

    // Get Input controller
    InputController* inputCtrl = CoreManager::getInstance().getInputController();
    inputCtrl->LoadKeybinds();

    StateManager::getInstance().setState(StateManager::ENTRY);

    //StateManager& instance = StateManager::getInstance();
    StateManager::getInstance().initStateMT(StateManager::MAINMENU);

    // Execute the main game loop
    while (window->isOpen())
    {
        // Check for window events
        sf::Event event;
        while (window->pollEvent(event))
        {
            // Close the window when cross is clicked
            if (event.type == sf::Event::Closed)
                window->close();

            // Forward events to InputController for keyboard and controller usage
            inputCtrl->parseEvents(event);

            // Forward events to currently enabled state
            StateManager::getInstance().parseCurrentStateEvents(event);
        }

        // Calculate framerate per second
        fps = float(1000000) / fpsclock.getElapsedTime().asMicroseconds();
        float rawFps = fps;
        frame_times.push_back(fps);
        fpsclock.restart();

        auto n = frame_times.size();
        float average = 0.0f;
        if (n != 0)
        {
            average = accumulate(frame_times.begin(), frame_times.end(), 0.0) / (n - 1);
        }

        if (fps <= 1)
            fps = average;
        else
            fps = rawFps;

        while (frame_times.size() > framerate_limit)
            frame_times.erase(frame_times.begin());

        //cout << fps << endl;

        window->clear();

        //mainMenu.Update(window, fps, inputCtrl);

        StateManager::getInstance().updateCurrentState();

        // Reset view for static GUI
        auto lastView = window->getView();
        window->setView(window->getDefaultView());

        // Draw version number
        t_version.setPosition(4, 4);
        window->draw(t_version);

        // If FPS counter is enabled, draw it
        if (config->GetInt("showFPS"))
        {
            t_fps.setString("FPS: " + to_string(int(ceil(rawFps))));
            t_fps.setOrigin(t_fps.getLocalBounds().width, 0);
            t_fps.setPosition(window->getSize().x - 4, 4);
            window->draw(t_fps);
        }

        // Display everything in the window
        window->display();

        // Return to last view
        window->setView(lastView);

        // Clear the key inputs
        inputCtrl->Flush();

        // Close the window through in-game means
        if (close_window)
        {
            window->close();
        }

        // Discord rich presence callbacks
        //if(state.core)
        //state.core->RunCallbacks();
    }

    SPDLOG_INFO("Main game loop exited. Shutting down...");
}
