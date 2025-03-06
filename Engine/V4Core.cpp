#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "V4Core.h"
#include "CoreManager.h"
#include "StateManager.h"
#include "Constants.h"
#include <chrono>
#include <fstream>

V4Core::V4Core()
{
    /** Check possible graphical settings (for debugging purposes) **/
    //const unsigned int maxSize = sf::Texture::getMaximumSize();
    //SPDLOG_DEBUG("Max allowed texture size: {}", maxSize);
    sf::RenderTexture rtx;
    SPDLOG_DEBUG("Max antialiasing level: {}", rtx.getMaximumAntialiasingLevel());

    /** Initialize Discord Rich Presence **/
    auto result = discord::Core::Create(712761245752623226, DiscordCreateFlags_NoRequireDiscord, &core);
    rpc_details = "Running V4Hero Client " + hero_version;

    /** Check if Discord RPC works and set a status **/
    if (!core) 
    {
        SPDLOG_ERROR("Failed to initialize Discord Rich Presence (err code: {})", static_cast<int>(result));
    } else
    {
        discord::Activity activity{};
        activity.SetDetails(rpc_details.c_str());
        activity.SetState("In Main menu");
        activity.GetAssets().SetLargeImage("logo");
        activity.SetType(discord::ActivityType::Playing);
        core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
            SPDLOG_INFO("Discord Rich Presence has {} updating activity", ((result == discord::Result::Ok) ? "Succeeded" : "Failed"));
        });
    }

    /** Load config from config.cfg **/
    Config* config = CoreManager::getInstance().getConfig();
    config->LoadConfig();

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
}

V4Core::~V4Core()
{
    SPDLOG_DEBUG("V4Core Destructor.");
}

void V4Core::changeRichPresence(std::string title, std::string bg_image, std::string sm_image)
{
    if(core)
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
            core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
                SPDLOG_INFO("Discord Rich Presence has {} updating activity", ((result == discord::Result::Ok) ? "Succeeded" : "Failed"));
            });
        }
    }
}

float V4Core::getFPS()
{
    return fps;
}

// Core function that runs the entirety of Patafour
void V4Core::init()
{
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

    // Version text
    t_version.append("{outline 2 0 0 0}{color 255 255 255}{size 24}V4Hero Client ");
    t_version.append(hero_version);
    t_version.setGlobalOrigin(t_version.getGlobalBounds().width, 0);

    // Apply window settings (fps limit, vsync)
    SPDLOG_INFO("Applying window settings");
    window->setFramerateLimit(framerate_limit);
    window->setKeyRepeatEnabled(false);
    window->setVerticalSyncEnabled(config->GetInt("verticalSync"));

    // TODO: check if we launched the game via launcher, then read the launcher's lang.txt or get the lang through cmdline args
    /*std::ifstream langConfig("resources/lang.txt");
    std::string buffer;
    std::getline(langConfig, buffer);
    strRepo->SetCurrentLanguage(buffer);*/

    // Load item registry
    SPDLOG_DEBUG("Loading item registry");
    SaveReader* saveReader = CoreManager::getInstance().getSaveReader();
    saveReader->itemReg.readItemFiles();

    // Load squad registry
    SPDLOG_DEBUG("Loading squad registry");
    saveReader->squadReg.Load();

    // Get Input controller
    InputController* inputCtrl = CoreManager::getInstance().getInputController();
    MouseController* mouseCtrl = CoreManager::getInstance().getMouseController();
    inputCtrl->LoadKeybinds();

    StateManager::getInstance().setState(StateManager::ENTRY);

    switch(CoreManager::getInstance().getConfig()->GetInt("test"))
    {
        case 1: {
            SPDLOG_INFO("Test initialized. Moving to TEST CHAMBER.");
            StateManager::getInstance().setState(StateManager::TEST_CHAMBER);
            break;
        }
        case 2: {
            SPDLOG_INFO("Test initialized. Moving to MISSION CONTROLLER, mission_file = mis1_0.p4m.");
            ifstream check(SAVEFILE_PATH);
            bool exists = check.good();
            check.close();

            if (exists)
            {
                /** Load save from saveReader **/
                saveReader->Flush();
                saveReader->LoadSave();
            } else {
                /** No save found. Create a blank one for test purposes **/
                saveReader->Flush();
                saveReader->CreateBlankSave();
            }

            mission_file = "resources/missions/debug_0001.json";
            mission_id = 0;
            mission_multiplier = 1;

            StateManager::getInstance().setState(StateManager::MISSIONCONTROLLER);
            break;
        }
        case 3: {
            SPDLOG_INFO("Test initialized. CREATING BLANK SAVE.");

            saveReader->Flush();
            saveReader->CreateBlankSave();
            saveReader->Save();
            saveReader->Flush();

            break;
        }
        case 4: {
            SPDLOG_INFO("Test initialized. LOAD PATAPOLIS.");

            ifstream check(SAVEFILE_PATH);
            bool exists = check.good();
            check.close();

            if (exists)
            {
                /** Load save from saveReader **/
                saveReader->Flush();
                saveReader->LoadSave();
            } else {
                /** No save found. Create a blank one for test purposes **/
                saveReader->Flush();
                saveReader->CreateBlankSave();
            }

            StateManager::getInstance().setState(StateManager::PATAPOLIS);

            break;
        }
        default: {
            break;
        }
    }

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
            mouseCtrl->parseEvents(event);

            // Forward events to currently enabled state
            StateManager::getInstance().parseCurrentStateEvents(event);
        }

        // Calculate framerate per second (delta time)
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

        window->clear();

        // Draw whatever state is currently in use
        StateManager::getInstance().updateCurrentState();

        // Reset view for static GUI
        auto lastView = window->getView();
        window->setView(window->getDefaultView());

        auto strRepo = CoreManager::getInstance().getStrRepo();
        std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

        // Draw version number
        t_version.setGlobalPosition(1276, 0);
        t_version.draw();

        // If FPS counter is enabled, draw it
        if (config->GetInt("showFPS"))
        {
            t_fps.reset();
            t_fps.append("{outline 2 0 0 0}{color 255 255 255}FPS: ");
            t_fps.append(to_string(int(ceil(rawFps))));
            t_fps.setGlobalOrigin(t_fps.getGlobalBounds().width, 0);
            t_fps.setGlobalPosition(1276, 24);
            t_fps.draw();
        }

        // Return to last view
        window->setView(lastView);

        // Display everything in the window
        window->display();

        // Clear the key inputs
        inputCtrl->Flush();

        // Close the window through in-game means
        if (close_window)
        {
            window->close();
        }

        // Discord rich presence callbacks
        if(core)
        core->RunCallbacks();
    }

    SPDLOG_INFO("Main game loop exited. Shutting down...");
}
