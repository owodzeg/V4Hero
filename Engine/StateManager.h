#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <spdlog/spdlog.h>
#include <variant>

#include "Menu/MainMenu.h"
#include "Menu/NewGameMenu.h"
#include "Mission/MissionController.h"

// Class for holding the pointers to commonly used important game contents
class StateManager
{
public:
    enum GameState
    {
        NEWGAMEMENU = 1,
        MAINMENU = 2,
        MISSIONCONTROLLER = 3
    };

    static StateManager& getInstance();
    std::variant<NewGameMenu*, MainMenu*, MissionController*> getCurrentState();
    void updateCurrentState();
    void setState(int state);

private:
    StateManager();
    ~StateManager();

    int currentGameState = 0;

    NewGameMenu* newGameMenuPtr;
    MainMenu* mainMenuPtr;
    MissionController* missionControllerPtr;
};

#endif