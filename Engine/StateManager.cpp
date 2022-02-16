#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "StateManager.h"
#include "CoreManager.h"

StateManager::StateManager()
{
}

StateManager::~StateManager()
{
}

StateManager& StateManager::getInstance()
{
    static StateManager instance;
    return instance;
}

std::variant<NewGameMenu*, MainMenu*, MissionController*> StateManager::getCurrentState()
{
    switch (currentGameState)
    {
        case 1: {
            return newGameMenuPtr;
            break;
        }

        case 2: {
            if (mainMenuPtr == nullptr)
                mainMenuPtr = new MainMenu;

            return mainMenuPtr;
            break;
        }

        case 3: {
            return missionControllerPtr;
            break;
        }
    }
} 

void StateManager::updateCurrentState()
{
    switch (currentGameState)
    {
        case NEWGAMEMENU: {
            //newGameMenuPtr->Update();
            break;
        }

        case MAINMENU: {
            if (mainMenuPtr == nullptr)
            {
                mainMenuPtr = new MainMenu;
                mainMenuPtr->Initialise(CoreManager::getInstance().getConfig(), CoreManager::getInstance().getCore());
            }

            mainMenuPtr->Update();
            break;
        }

        case MISSIONCONTROLLER: {
            //missionControllerPtr->Update();
            break;
        }
    }
}

void StateManager::parseCurrentStateEvents(sf::Event& event)
{
    switch (currentGameState)
    {
        case NEWGAMEMENU: {
            break;
        }

        case MAINMENU: {
            if (mainMenuPtr == nullptr)
            {
                mainMenuPtr = new MainMenu;
                mainMenuPtr->Initialise(CoreManager::getInstance().getConfig(), CoreManager::getInstance().getCore());
            }

            mainMenuPtr->EventFired(event);
            break;
        }

        case MISSIONCONTROLLER: {
            break;
        }
    }
}

void StateManager::setState(int state)
{
    currentGameState = state;
}