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

    std::vector<sf::String> a = {Func::ConvertToUtf8String(strRepo->GetString("error_tryagain")), Func::ConvertToUtf8String(strRepo->GetString("error_tryprevious")), Func::ConvertToUtf8String(strRepo->GetString("error_mainmenu")), Func::ConvertToUtf8String(strRepo->GetString("error_exit"))};

    PataDialogBox db;
    db.Create(font, Func::ConvertToUtf8String(strRepo->GetString("error_message")), a, config->GetInt("textureQuality"), 2);
    db.id = 0;
    dialogboxes.push_back(db);

    b_anvil.loadFromFile("resources/sfx/drums/anvil.ogg");
    s_anvil.setBuffer(b_anvil);
    s_anvil.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("bgmVolume")) / 100.f));
    s_anvil.play();
}

void ErrorChamber::Update()
{
    for (int i = 0; i < dialogboxes.size(); i++)
    {
        dialogboxes[i].x = 1920;
        dialogboxes[i].y = 1080;
        dialogboxes[i].Draw();
    }
}

ErrorChamber::~ErrorChamber()
{

}