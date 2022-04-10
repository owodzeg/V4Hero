#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "CoreManager.h"

CoreManager::CoreManager()
{

}

CoreManager::~CoreManager()
{

}

CoreManager& CoreManager::getInstance()
{
    static CoreManager instance;
    return instance;
}

// Initializes necessary parts of the game.
void CoreManager::init()
{
    // Config must be loaded first. V4Core uses config for initialization.
    config = new Config;

    // String repository and save reader both use config values, so we initialize it after config
    strRepo = new StringRepository;
    saveReader = new SaveReader;

    // Create InputController before window, because we take events from it
    inputCtrl = new InputController;

    // Window must be created beforehand so V4Core can use it.
    window = new sf::RenderWindow;

    // Load tips utility so we can have the background and icon filenames ready
    tipsUtil = new TipsUtil;

    // Prepare an empty mission controller for future usage
    missionController = new MissionController;

    // After we created prerequisities for V4Core, we can safely create it.
    core = new V4Core;
}

// Returns a pointer to V4Core.
V4Core* CoreManager::getCore()
{
    return core;
}

// Returns a pointer to Config.
Config* CoreManager::getConfig()
{
    return config;
}

// Returns a pointer to String repository.
StringRepository* CoreManager::getStrRepo()
{
    return strRepo;
}

// Returns a pointer to SaveReader.
SaveReader* CoreManager::getSaveReader()
{
    return saveReader;
}

// Returns a pointer to InputController.
InputController* CoreManager::getInputController()
{
    return inputCtrl;
}

// Returns a pointer to Tips utility.
TipsUtil* CoreManager::getTipsUtil()
{
    return tipsUtil;
}

MissionController* CoreManager::getMissionController()
{
    return missionController;
}

// Returns a pointer to the window game uses.
sf::RenderWindow* CoreManager::getWindow()
{
    return window;
}