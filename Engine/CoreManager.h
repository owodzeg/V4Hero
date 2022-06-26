#ifndef COREMANAGER_H
#define COREMANAGER_H

#include <filesystem>
#include <spdlog/spdlog.h>

#include "V4Core.h"
#include "Config.h"
#include "SaveReader.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "StringRepository.h"
#include "TipsUtil.h"
#include "Mission/MissionController.h"

// Class for holding the pointers to commonly used important game contents
class CoreManager
{
public:
    static CoreManager& getInstance();
    void init();
    V4Core* getCore();
    Config* getConfig();
    StringRepository* getStrRepo();
    SaveReader* getSaveReader();
    InputController* getInputController();
    TipsUtil* getTipsUtil();
    MissionController* getMissionController();
    void reinitMissionController();
    void deleteMissionController();
    sf::RenderWindow* getWindow();

private:
    CoreManager();
    ~CoreManager();
    
    // Pointer to V4Core
    V4Core* core;

    // Pointer to Config
    Config* config;

    // Pointer to String repository (for localization)
    StringRepository* strRepo;

    // Pointer to Save (and item) manager
    SaveReader* saveReader;

    // Pointer to Input controller
    InputController* inputCtrl;

    // Pointer to Tips utility
    TipsUtil* tipsUtil;

    // Pointer to MissionController (handled by StateManager)
    MissionController* missionController;

    // Pointer to current window
    sf::RenderWindow* window;
};

#endif