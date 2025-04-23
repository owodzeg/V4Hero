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
    Config* config = CoreManager::getInstance().getConfig();

    std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    std::vector<sf::String> a = {"error_tryagain", "error_tryprevious", "error_mainmenu", "error_exit"};

    PataDialogBox db;
    db.Create(font, "error_message", a, config->GetInt("textureQuality"), 2);
    db.id = 0;
    dialogboxes.push_back(db);

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