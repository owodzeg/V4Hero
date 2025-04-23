#ifndef COREMANAGER_H
#define COREMANAGER_H

#include <filesystem>
#include <spdlog/spdlog.h>

#include "V4Core.h"
#include "Config.h"
#include "SaveReader.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "StringRepository.h"
#include "TipsUtil.h"
#include "Mission/MissionController.h"
#include "Rhythm/Rhythm.h"
#include "Rhythm/RhythmGUI.h"
#include "Rhythm/RhythmController.h"
#include "Rhythm/SongController.h"
#include "Input/TextInputController.h"
#include "Input/MouseController.h"
#include "Mechanics/Globals.h"

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
    TextInputController* getTextInputController();
    MouseController* getMouseController();
    TipsUtil* getTipsUtil();
    MissionController* getMissionController();
    Rhythm* getRhythm();
    RhythmGUI* getRhythmGUI();
    RhythmController* getRhythmController();
    SongController* getSongController();
    Globals* getGlobals();
    void reinitMissionController();
    void deleteMissionController();
    void reinitSongController();
    void deleteSongController();
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

    // Pointer to Text input controller
    TextInputController* textCtrl;

    // Pointer to Mouse controller
    MouseController* mouseCtrl;

    // Pointer to Globals
    Globals* globals;

    // Pointer to Tips utility
    TipsUtil* tipsUtil;

    // Pointer to MissionController (handled by StateManager)
    MissionController* missionController;

    // Pointer to Rhythm
    Rhythm* rhythm;

    // Pointer to RhythmGUI
    RhythmGUI* rhythmGUI;

    // Pointer to RhythmController
    RhythmController* rhythmController;

    // Pointer to SongController
    SongController* songController;

    // Pointer to current window
    sf::RenderWindow* window;
};

#endif