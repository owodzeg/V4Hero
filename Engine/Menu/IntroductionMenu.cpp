#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "IntroductionMenu.h"
#include "../CoreManager.h"
#include "../StateManager.h"

IntroductionMenu::IntroductionMenu()
{
    is_active = false;
}

void IntroductionMenu::Initialize()
{
    SPDLOG_DEBUG("Initialize Introduction menu");

    msgcloud.Create(45, sf::Vector2f(1920, 1440), sf::Color::White);
    msgcloud.AddDialog("newgame_intro_1", true);
    msgcloud.AddDialog("newgame_intro_2", true);
    msgcloud.AddDialog("newgame_intro_3", true);
    msgcloud.AddDialog("newgame_intro_4", true);
    msgcloud.AddDialog("newgame_intro_5", true);
    msgcloud.AddDialog("newgame_intro_6", true);
    msgcloud.AddDialog("newgame_intro_7", true);
    msgcloud.AddDialog("newgame_intro_8", true);
    msgcloud.AddDialog("newgame_intro_9", true);
    msgcloud.AddDialog("newgame_intro_10", true);
    msgcloud.AddDialog("newgame_intro_11", true);
    msgcloud.AddDialog("newgame_intro_12", true);
    msgcloud.AddDialog("newgame_intro_13", true);
    msgcloud.AddDialog("newgame_intro_14", true);

    SPDLOG_DEBUG("Introduction menu initialized.");
    initialized = true;
}

void IntroductionMenu::Update()
{
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
        CoreManager::getInstance().getCore()->mission_file = "resources/missions/mis1_0.json";
        CoreManager::getInstance().getCore()->mission_id = 0;
        CoreManager::getInstance().getCore()->mission_multiplier = 1;

        StateManager::getInstance().setState(StateManager::MISSIONCONTROLLER);

        //TO-DO: change state in statemanager
    }
}

IntroductionMenu::~IntroductionMenu()
{
}
