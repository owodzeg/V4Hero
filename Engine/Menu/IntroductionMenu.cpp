#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "IntroductionMenu.h"
#include "../V4Core.h"
#include "../CoreManager.h"

IntroductionMenu::IntroductionMenu()
{
    is_active = false;
}

void IntroductionMenu::Initialize()
{
    SPDLOG_DEBUG("Initialize Introduction menu");

    msgcloud.Create(45, sf::Vector2f(640, 480), sf::Color::White, true, CoreManager::getInstance().getConfig()->GetInt("textureQuality"), CoreManager::getInstance().getConfig()->fontPath);
    msgcloud.AddDialog(Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("newgame_intro_1")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("newgame_intro_2")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("newgame_intro_3")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("newgame_intro_4")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("newgame_intro_5")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("newgame_intro_6")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("newgame_intro_7")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("newgame_intro_8")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("newgame_intro_9")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("newgame_intro_10")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("newgame_intro_11")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("newgame_intro_12")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("newgame_intro_13")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("newgame_intro_14")), true);

    SPDLOG_DEBUG("Introduction menu initialized.");
}

void IntroductionMenu::EventFired(sf::Event event)
{
}

void IntroductionMenu::Update()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    float fps = CoreManager::getInstance().getCore()->getFPS();
    InputController* inputCtrl = CoreManager::getInstance().getInputController();

    if (timeout.getElapsedTime().asSeconds() > 1)
    {
        if (!msgcloud.done)
            msgcloud.Show();
    }

    msgcloud.Draw();

    if (!msgcloud.done)
        timeout2.restart();

    if (timeout2.getElapsedTime().asSeconds() > 3)
    {
        StateManager::getInstance().setState(StateManager::MISSIONCONTROLLER);

        //TO-DO: change state in statemanager

        /*
        sf::Thread loadingThreadInstance(&V4Core::loadingThread, parentMenu->v4Core);
        parentMenu->v4Core->continue_loading = true;
        parentMenu->v4Core->window.setActive(false);
        loadingThreadInstance.launch();

        parentMenu->Hide();
        parentMenu->is_active = false;
        Hide();
        is_active = false;

        v4Core->currentController.Initialise(*thisConfig, thisConfig->GetString("mission1Background"), *v4Core);
        v4Core->currentController.StartMission("mis1_0.p4m", false, 1);
        */
    }
}

IntroductionMenu::~IntroductionMenu()
{
}
