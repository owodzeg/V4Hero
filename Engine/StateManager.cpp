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

        case INTRODUCTION: {
            if (introductionPtr == nullptr)
            {
                introductionPtr = new IntroductionMenu;
            }

            introductionPtr->Update();
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

                    if (patapolisPtr != nullptr && altarPtr != nullptr && barracksPtr != nullptr && obeliskPtr != nullptr)
                    {
                        if (patapolisPtr->initialized && altarPtr->initialized && barracksPtr->initialized && obeliskPtr->initialized)
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

                case MISSIONCONTROLLER: {

                    if (missionControllerPtr != nullptr)
                    {
                        if (missionControllerPtr->initialized)
                        {
                            if (loadingTipPtr != nullptr)
                            {
                                loadingTipPtr->pressAnyKey = true;

                                if (loadingTipPtr->tipFinished)
                                {
                                    setState(afterTipState);
                                    missionControllerPtr->rhythm.Start();
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

        case PATAPOLIS_ALTAR: {

            if (patapolisPtr == nullptr)
            {
                patapolisPtr = new PatapolisMenu;
            }

            if (altarPtr == nullptr)
            {
                altarPtr = new AltarMenu;
            }

            patapolisPtr->Update();
            altarPtr->Update();

            break;
        }

        case BARRACKS: {
        
            if (barracksPtr == nullptr)
            {
                barracksPtr = new Barracks;
            }

            barracksPtr->Update();

            break;
        }

        case OBELISK: {
        
            if (obeliskPtr == nullptr)
            {
                obeliskPtr = new ObeliskMenu;
            }

            obeliskPtr->Update();

            break;
        }

        case MISSIONCONTROLLER: {

            if (missionControllerPtr == nullptr)
            {
                missionControllerPtr = new MissionController;
            }

            missionControllerPtr->Update();
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

        case INTRODUCTION: {

            if (introductionPtr == nullptr)
            {
                introductionPtr = new IntroductionMenu;
                introductionPtr->Initialize();
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

            if (altarPtr == nullptr)
            {
                altarPtr = new AltarMenu;
                altarPtr->save_loaded = patapolisPtr->save_loaded;
                altarPtr->reloadInventory();
                altarPtr->initialized = true;
            }

            if (barracksPtr == nullptr)
            {
                barracksPtr = new Barracks;
            }

            if (obeliskPtr == nullptr)
            {
                obeliskPtr = new ObeliskMenu;
                obeliskPtr->Reload();
                obeliskPtr->initialized = true;
            }

            break;
        }

        case MISSIONCONTROLLER: {

            if (missionControllerPtr == nullptr)
            {
                missionControllerPtr = CoreManager::getInstance().getMissionController();

                std::string mission_file = "";
                int mission_id = -1;

                if (barracksPtr != nullptr)
                {
                    mission_file = barracksPtr->mission_file;
                    mission_id = barracksPtr->mission_id;
                } else
                {
                    SPDLOG_ERROR("No load mission specified, returning to Patapolis");
                    missionControllerPtr->loadingError = true;
                    setState(PATAPOLIS);
                    break;
                }

                missionControllerPtr->StartMission(mission_file, false, mission_id, 1); ///last argument: placeholder, mission multiplier
            }

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
    
    //return from options to main
    if (currentGameState == OPTIONSMENU && state == MAINMENU) 
    {
        if (mainMenuPtr != nullptr)
        {
            mainMenuPtr->screenFade.Create(ScreenFade::FADEIN, 1024);
        }
    }

    //go from main to options
    if (currentGameState == MAINMENU && state == OPTIONSMENU) 
    {
        if (optionsMenuPtr != nullptr)
        {
            optionsMenuPtr->state = 0;
            optionsMenuPtr->sel = 0;
            optionsMenuPtr->screenFade.Create(ScreenFade::FADEIN, 1024);
        }
    }

    //go from main to patapolis (forward through tips)
    if (currentGameState == MAINMENU && state == PATAPOLIS) 
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

    // go from patapolis to altar
    if (currentGameState == PATAPOLIS && state == PATAPOLIS_ALTAR)
    {
        if (altarPtr != nullptr)
        {
            altarPtr->reloadInventory();
        }
    }

    // go from patapolis to barracks
    if (currentGameState == PATAPOLIS && state == BARRACKS)
    {
        if (barracksPtr != nullptr)
        {
            barracksPtr->screenFade.Create(ScreenFade::FADEIN, 1024);
            barracksPtr->obelisk = false;
            barracksPtr->refreshStats();
            barracksPtr->updateInputControls();
        }
    }

    // go from barracks to patapolis
    if (currentGameState == BARRACKS && state == PATAPOLIS)
    {
        if (patapolisPtr != nullptr)
        {
            patapolisPtr->screenFade.Create(ScreenFade::FADEIN, 1024);
        }
    }

    // go from patapolis to obelisk
    if (currentGameState == PATAPOLIS && state == OBELISK)
    {
        if (obeliskPtr != nullptr)
        {
            obeliskPtr->screenFade.Create(ScreenFade::FADEIN, 1024);
        }
    }

    // go from obelisk to patapolis
    if (currentGameState == OBELISK && state == PATAPOLIS)
    {
        if (patapolisPtr != nullptr)
        {
            patapolisPtr->screenFade.Create(ScreenFade::FADEIN, 1024);
        }
    }

    // go from obelisk to barracks
    if (currentGameState == OBELISK && state == BARRACKS)
    {
        if (barracksPtr != nullptr)
        {
            barracksPtr->screenFade.Create(ScreenFade::FADEIN, 1024);
            barracksPtr->obelisk = true;
            barracksPtr->mission_id = obeliskPtr->missions[obeliskPtr->sel_mission].mis_ID;
            barracksPtr->mission_file = obeliskPtr->missions[obeliskPtr->sel_mission].mission_file;
            barracksPtr->refreshStats();
            barracksPtr->updateInputControls();
        }
    }

    // go from barracks to obelisk
    if (currentGameState == BARRACKS && state == OBELISK)
    {
        if (obeliskPtr != nullptr)
        {
            obeliskPtr->screenFade.Create(ScreenFade::FADEIN, 1024);
        }
    }

    //go from barracks to missioncontroller
    if (currentGameState == BARRACKS && state == MISSIONCONTROLLER)
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
        afterTipState = MISSIONCONTROLLER;

        initStateMT(afterTipState);
    }

    //go from tips to patapolis (mission load error)
    if (missionControllerPtr != nullptr)
    {
        if (missionControllerPtr->loadingError)
        {
            if (currentGameState == TIPS && state == PATAPOLIS)
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
        }
    }

    // Change the state
    SPDLOG_DEBUG("Changing state to {}", state);
    currentGameState = state;
}

int StateManager::getState()
{
    return currentGameState;
}