#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <spdlog/spdlog.h>
#include <variant>
#include <thread>
#include <vector>

#include "Menu/MainMenu.h"
#include "Menu/OptionsMenu.h"
#include "Menu/LoadingTip.h"
#include "Menu/Patapolis.h"
#include "Menu/Altar.h"
#include "Menu/Barracks.h"
#include "Mission/MissionController.h"

// Class for holding the pointers to commonly used important game contents
class StateManager
{
public:
    enum GameState
    {
        ENTRY = 0,
        NEWGAMEMENU = 1,
        MAINMENU = 2,
        OPTIONSMENU = 3,
        INTRODUCTION = 4,
        TIPS = 5,
        PATAPOLIS = 6,
        PATAPOLIS_ALTAR = 7,
        BARRACKS = 8,
        OBELISK = 9,
        MISSIONCONTROLLER = 10
    };

    static StateManager& getInstance();
    void updateCurrentState();
    void parseCurrentStateEvents(sf::Event& event);
    void initState(int state);
    void initStateMT(int state);
    void setState(int state);
    int getState();

private:
    StateManager();
    ~StateManager();

    int currentGameState = 0;
    int afterTipState = 0;

    MainMenu* mainMenuPtr;
    OptionsMenu* optionsMenuPtr;
    MissionController* missionControllerPtr;
    LoadingTip* loadingTipPtr;
    PatapolisMenu* patapolisPtr;
    AltarMenu* altarPtr;
    Barracks* barracksPtr;
    ObeliskMenu* obeliskPtr;

    std::vector<std::thread> loadingThreads;
};

#endif