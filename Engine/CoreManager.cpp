

#include "CoreManager.h"

CoreManager::CoreManager()
{

}

CoreManager::~CoreManager()
{
    SPDLOG_DEBUG("CoreManager Destructor.");

    SPDLOG_DEBUG("Delete Window.");
    delete window;
    SPDLOG_DEBUG("Delete DialogHandler");
    delete dialogHandler;
    SPDLOG_DEBUG("Delete songController.");
    delete songController;
    SPDLOG_DEBUG("Delete rhythmController.");
    delete rhythmController;
    SPDLOG_DEBUG("Delete rhythmGUI.");
    delete rhythmGUI;
    SPDLOG_DEBUG("Delete Rhythm.");
    delete rhythm;
    SPDLOG_DEBUG("Delete missionController.");
    delete missionController;
    SPDLOG_DEBUG("Delete tips utility.");
    delete tipsUtil;
    SPDLOG_DEBUG("Delete globals.");
    delete globals;
    SPDLOG_DEBUG("Delete mouse controller.");
    delete mouseCtrl;
    SPDLOG_DEBUG("Delete text controller.");
    delete textCtrl;
    SPDLOG_DEBUG("Delete input controller.");
    delete inputCtrl;
    SPDLOG_DEBUG("Delete save reader.");
    delete saveReader;
    SPDLOG_DEBUG("Delete string repository.");
    delete strRepo;
    SPDLOG_DEBUG("Delete config.");
    delete config;
    SPDLOG_DEBUG("Delete core.");
    delete core;
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
    textCtrl = new TextInputController;
    mouseCtrl = new MouseController;

    // Window must be created beforehand so V4Core can use it.
    window = new sf::RenderWindow;

    // Load tips utility so we can have the background and icon filenames ready
    tipsUtil = new TipsUtil;

    // Prepare rhythmGUI for missions
    rhythm = new Rhythm;

    // Prepare rhythmGUI for missions
    rhythmGUI = new RhythmGUI;

    // Prepare rhythm controller for general use (missions + minigames)
    rhythmController = new RhythmController;

    // Create song controller for managing themes
    songController = new SongController;

    // Create new globals table
    globals = new Globals;

    // Create new dialog handler
    dialogHandler = new DialogHandler;

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

// Returns a pointer to TextInputController.
TextInputController* CoreManager::getTextInputController()
{
    return textCtrl;
}

// Returns a pointer to MouseController.
MouseController* CoreManager::getMouseController()
{
    return mouseCtrl;
}

// Returns a pointer to Globals table.
Globals* CoreManager::getGlobals()
{
    return globals;
}

// Returns a pointer to DialogHandler.
DialogHandler* CoreManager::getDialogHandler()
{
    return dialogHandler;
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

Rhythm* CoreManager::getRhythm()
{
    return rhythm;
}

RhythmGUI* CoreManager::getRhythmGUI()
{
    return rhythmGUI;
}

RhythmController* CoreManager::getRhythmController()
{
    return rhythmController;
}

SongController* CoreManager::getSongController()
{
    return songController;
}

void CoreManager::reinitMissionController()
{
    missionController = new MissionController;
}

void CoreManager::deleteMissionController()
{
    if (missionController != nullptr)
        delete missionController;

    missionController = nullptr;
}

void CoreManager::reinitSongController()
{
    songController = new SongController;
}

void CoreManager::deleteSongController()
{
    if (songController != nullptr)
        delete songController;

    songController = nullptr;
}

// Returns a pointer to the window game uses.
sf::RenderWindow* CoreManager::getWindow()
{
    return window;
}