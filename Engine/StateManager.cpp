#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "StateManager.h"
#include "CoreManager.h"

StateManager::StateManager()
{
}

StateManager::~StateManager()
{
    for (auto& t : loadingThreads)
    {
        t.detach();
    }
}

StateManager& StateManager::getInstance()
{
    static StateManager instance;
    return instance;
}

void StateManager::updateCurrentState()
{
    switch (currentGameState)
    {
        case ENTRY: {
            if (mainMenuPtr != nullptr && optionsMenuPtr != nullptr)
            {
                if (mainMenuPtr->initialized && optionsMenuPtr->initialized)
                {
                    setState(MAINMENU);
                }
            }
            break;
        }

        case NEWGAMEMENU: {
            //newGameMenuPtr->Update();
            break;
        }

        case MAINMENU: {
            if (mainMenuPtr == nullptr)
            {
                mainMenuPtr = new MainMenu;
            }

            mainMenuPtr->Update();
            break;
        }

        case OPTIONSMENU: {
            if (optionsMenuPtr == nullptr)
            {
                optionsMenuPtr = new OptionsMenu;
            }

            optionsMenuPtr->Update();
            break;
        }

        case TIPS: {

            if (loadingTipPtr == nullptr)
            {
                loadingTipPtr = new LoadingTip;
            }

            switch (afterTipState)
            {
                case PATAPOLIS: {

                    if (patapolisPtr != nullptr)
                    {
                        if (patapolisPtr->initialized)
                        {
                            if (loadingTipPtr != nullptr)
                            {
                                loadingTipPtr->pressAnyKey = true;

                                if (loadingTipPtr->tipFinished)
                                {
                                    setState(afterTipState);
                                }
                            }
                        }
                    }

                    break;
                }
            }

            loadingTipPtr->Draw();

            break;
        }

        case PATAPOLIS: {
        
            if (patapolisPtr == nullptr)
            {
                patapolisPtr = new PatapolisMenu;
            }

            patapolisPtr->Update();

            break;
        }

        case MISSIONCONTROLLER: {
            //missionControllerPtr->Update();
            break;
        }
    }
}

void StateManager::initState(int state)
{
    switch (state)
    {
        case NEWGAMEMENU: {

            break;
        }

        case MAINMENU: {

            // For initializing MainMenu, we want to initialize OptionsMenu as well, so the transition is seamless
            if (mainMenuPtr == nullptr)
            {
                mainMenuPtr = new MainMenu;
            }

            if (optionsMenuPtr == nullptr)
            {
                optionsMenuPtr = new OptionsMenu;
            }

            break;
        }

        case OPTIONSMENU: {
       
            if (optionsMenuPtr == nullptr)
            {
                optionsMenuPtr = new OptionsMenu;
            }
            
            break;
        }

        case TIPS: {

            if (loadingTipPtr == nullptr)
            {
                loadingTipPtr = new LoadingTip;
            }

            break;
        }

        case PATAPOLIS: {
        
            if (patapolisPtr == nullptr)
            {
                patapolisPtr = new PatapolisMenu;
            }
        }

        case MISSIONCONTROLLER: {

            break;
        }
    }
}

void StateManager::initStateMT(int state)
{
    loadingThreads.push_back(std::thread(&StateManager::initState, this, state));
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
    // Here is a good place to put specific events that always happen when changing states
    
    if (currentGameState == OPTIONSMENU && state == MAINMENU) //return from options to main
    {
        if (mainMenuPtr != nullptr)
        {
            mainMenuPtr->screenFade.Create(0, 512);
        }
    }

    if (currentGameState == MAINMENU && state == OPTIONSMENU) //go from main to options
    {
        if (optionsMenuPtr != nullptr)
        {
            optionsMenuPtr->state = 0;
            optionsMenuPtr->sel = 0;
            optionsMenuPtr->screenFade.Create(0, 512);
        }
    }

    if (currentGameState == MAINMENU && state == PATAPOLIS) //go from main to patapolis (forward through tips)
    {
        if (loadingTipPtr == nullptr)
        {
            loadingTipPtr = new LoadingTip;
        } else
        {
            delete loadingTipPtr;
            loadingTipPtr = new LoadingTip;
        }

        state = TIPS;
        afterTipState = PATAPOLIS;

        initStateMT(afterTipState);
    }

    // Change the state
    SPDLOG_DEBUG("Changing state to {}", state);
    currentGameState = state;
}