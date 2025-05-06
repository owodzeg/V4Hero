#include "ErrorChamber.h"
#include "CoreManager.h"
#include "StringRepository.h"
#include <spdlog/spdlog.h>

ErrorChamber::ErrorChamber()
{
    Initialize();
}

void ErrorChamber::Initialize()
{
    SPDLOG_ERROR("Welcome to the Error Chamber! Something bad must have happened...");

    StringRepository* strRepo = CoreManager::getInstance().getStrRepo();

    std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    std::vector<sf::String> a = {"error_tryagain", "error_tryprevious", "error_mainmenu", "error_exit"};

    dialogboxes.emplace_back(
        Func::GetStrFromKey("error_message"),
        std::vector<PataDialogBox::Option>{
            {Func::GetStrFromKey("error_tryagain"), [this]() {
                SPDLOG_ERROR("Trying again...");
                }},
            {Func::GetStrFromKey("error_tryprevious"), [this]() {
                SPDLOG_ERROR("Trying previous state...");
                }},
            {Func::GetStrFromKey("error_mainmenu"), [this]() {
                SPDLOG_ERROR("Going to main menu...");
                }},
            {Func::GetStrFromKey("error_exit"), [this]() {
                SPDLOG_ERROR("Exiting game...");
                CoreManager::getInstance().getCore()->close_window = true;
            }}
        },
        2
    );

    SoundManager::getInstance().loadBufferFromFile("resources/sfx/drums/anvil.ogg");
    SoundManager::getInstance().playSound("resources/sfx/drums/anvil.ogg", SoundManager::SoundTag::INTERFACE);
}

void ErrorChamber::Update()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    auto view = window->getView();
    window->setView(window->getDefaultView());

    for (int i = 0; i < dialogboxes.size(); i++)
    {
        dialogboxes[i].x = 1920;
        dialogboxes[i].y = 1080;
        dialogboxes[i].Draw();
    }

    window->setView(view);
}

ErrorChamber::~ErrorChamber()
{

}