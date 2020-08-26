#include "IntroductionMenu.h"
#include "../V4Core.h"

IntroductionMenu::IntroductionMenu()
{
    isActive=false;
}

void IntroductionMenu::Initialise(Config *thisConfig, V4Core *parent, Menu *curParentMenu)
{
    parent->SaveToDebugLog("Initializing Introduction menu...");
    cout << "Initialize Introduction menu" << endl;
    Scene::Initialise(thisConfig,parent);
    //buttonList.Initialise(&m_font,*thisConfig,keymap,&(v4core->currentController),this);
    parentMenu = curParentMenu;
    cout << "Initial values loaded, loading assets" << endl;

    msgcloud.Create(45, sf::Vector2f(640,480), thisConfig->GetInt("textureQuality"));
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"newgame_intro_1")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"newgame_intro_2")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"newgame_intro_3")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"newgame_intro_4")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"newgame_intro_5")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"newgame_intro_6")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"newgame_intro_7")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"newgame_intro_8")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"newgame_intro_9")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"newgame_intro_10")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"newgame_intro_11")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"newgame_intro_12")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"newgame_intro_13")), true);
    msgcloud.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"newgame_intro_14")), true);

    timeout.restart();

    parent->SaveToDebugLog("Introduction menu initialized.");
}

void IntroductionMenu::EventFired(sf::Event event)
{

}

void IntroductionMenu::Update(sf::RenderWindow& window, float fps, InputController& inputCtrl)
{
    if(timeout.getElapsedTime().asSeconds() > 1)
    {
        if(!msgcloud.done)
        msgcloud.Show();
    }

    msgcloud.Draw(window, fps, inputCtrl);

    if(!msgcloud.done)
    timeout2.restart();

    if(timeout2.getElapsedTime().asSeconds() > 3)
    {
        sf::Thread loadingThreadInstance(parentMenu->v4core->LoadingThread,parentMenu->v4core);
        parentMenu->v4core->continueLoading=true;
        parentMenu->v4core->window.setActive(false);
        loadingThreadInstance.launch();

        parentMenu->Hide();
        parentMenu->isActive = false;
        Hide();
        isActive = false;

        v4core->currentController.Initialise(*thisConfig,thisConfig->GetString("mission1Background"),*v4core);
        v4core->currentController.StartMission("mis1.p4m",false,1);
    }
}

IntroductionMenu::~IntroductionMenu()
{

}
