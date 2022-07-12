#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "V4Core.h"
#include "CoreManager.h"
#include "StateManager.h"
#include <chrono>

V4Core::V4Core()
{
    /** Check possible graphical settings (for debugging purposes) **/
    const unsigned int maxSize = sf::Texture::getMaximumSize();
    SPDLOG_DEBUG("Max allowed texture size: {}", maxSize);
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
    SPDLOG_DEBUG("Font path is {}", config->fontPath);

    /** Version and fps text **/
    SPDLOG_DEBUG("Creating text for version and FPS");
    strRepo->font.loadFromFile(config->fontPath);

    t_version.setFont(strRepo->font);
    t_version.setCharacterSize(24);
    t_version.setFillColor(sf::Color(255, 255, 255, 32));
    t_version.setString("V4Hero Client " + hero_version);

    t_fps.setFont(strRepo->font);
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
        if(core)
        core->RunCallbacks();
    }

    SPDLOG_INFO("Main game loop exited. Shutting down...");
}
