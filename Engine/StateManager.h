#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <spdlog/spdlog.h>
#include <variant>
#include <thread>
#include <vector>

#include "Menu/MainMenu.h"
#include "Menu/NewGameMenu.h"
#include "Menu/OptionsMenu.h"
#include "Menu/LoadingTip.h"
#include "Menu/Patapolis.h"
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
        MISSIONCONTROLLER = 10
    };

    static StateManager& getInstance();
    void updateCurrentState();
    void parseCurrentStateEvents(sf::Event& event);
    void initState(int state);
    void initStateMT(int state);
    void setState(int state);

private:
    StateManager();
    ~StateManager();

    int currentGameState = 0;
    int afterTipState = 0;

    NewGameMenu* newGameMenuPtr;
    MainMenu* mainMenuPtr;
    OptionsMenu* optionsMenuPtr;
    MissionController* missionControllerPtr;
    LoadingTip* loadingTipPtr;
    PatapolisMenu* patapolisPtr;

    std::vector<std::thread> loadingThreads;
};

#endif