#include "OptionsButtonList.h"
#include "string"
#include "../MenuButton.h"
#include<iostream>
#include "../../Mission/MissionController.h"
#include "../MainMenu.h"
#include <algorithm>
#include "../../Func.h"
#include "../../V4Core.h"
using namespace std;
OptionsButtonList::OptionsButtonList()
{

}
void OptionsButtonList::Initialise(sf::Font *font,Config &newConfig,  std::map<int,bool> *thisKeymap,MissionController *controller,OptionsMenu *mainMenu)
{
    ButtonList::Initialise(font,newConfig,thisKeymap,controller,mainMenu);
    optionMenu = mainMenu;
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_1280x720"));
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_1366x768"));
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_1366x700"));
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_1600x900"));
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_1600x800"));
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_1920x1080"));
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_1920x980"));
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_2560x1440"));
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_2560x1240"));
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_3840x2160"));
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_3840x1960"));

    texture_quality.push_back(config->strRepo.GetUnicodeString(L"texture_quality_high"));
    texture_quality.push_back(config->strRepo.GetUnicodeString(L"texture_quality_medium"));
    texture_quality.push_back(config->strRepo.GetUnicodeString(L"texture_quality_low"));

    fps_options.push_back(config->strRepo.GetUnicodeString(L"fps_cap_60"));
    fps_options.push_back(config->strRepo.GetUnicodeString(L"fps_cap_120"));
    fps_options.push_back(config->strRepo.GetUnicodeString(L"fps_cap_144"));
    fps_options.push_back(config->strRepo.GetUnicodeString(L"fps_cap_240"));
    fps_options.push_back(config->strRepo.GetUnicodeString(L"fps_cap_0")+L" "+config->strRepo.GetUnicodeString(L"fps_cap_unlimited"));

    show_debug_options.push_back(config->strRepo.GetUnicodeString(L"option_hide_debug_info"));
    show_debug_options.push_back(config->strRepo.GetUnicodeString(L"option_show_debug_info"));

    curShowDebugPos = config->GetInt("showDebugMsg");
    curTextureQualPos = config->GetInt("textureQuality")-1;

    string fpsCapStr = config->GetString("framerateLimit");
    if (fpsCapStr!="0"){
        std::wstring resws;
        resws.assign(fpsCapStr.begin(), fpsCapStr.end());
        std::vector<wstring>::iterator rit = std::find(fps_options.begin(), fps_options.end(), resws);
        if (rit == fps_options.end())
        {
            /// Element not in list. Push fps onto the end and then get the position of the current
            fps_options.push_back(resws);
        }

        std::vector<wstring>::iterator ritt = std::find(fps_options.begin(), fps_options.end(), resws);
        int rindex = std::distance(fps_options.begin(), ritt);
        curFpsCapPos = rindex;
    } else {
        curFpsCapPos = 4;
    }

    string resolution = config->GetString("resX")+"x"+config->GetString("resY");
    std::wstring ws;
    ws.assign(resolution.begin(), resolution.end());
    std::vector<wstring>::iterator it = std::find(resolutions.begin(), resolutions.end(), ws);
    if (it == resolutions.end())
    {
        /// Element not in list. Push resolution onto the end and then get the position of the current
        resolutions.push_back(ws);
    }
    std::vector<wstring>::iterator itt = std::find(resolutions.begin(), resolutions.end(), ws);
    int index = std::distance(resolutions.begin(), itt);
    curResPos = index;
    std::wstring lngbtn = L"menu_options_button_language";
    std::wstring rbtn = L"menu_options_button_resolution";
    std::wstring txbtn = L"menu_options_button_texture_quality";
    std::wstring fpsbtn = L"menu_options_button_fps_cap";
    std::wstring sdbgbtn = L"menu_options_button_show_debug_info";
    std::wstring bkbtn = L"menu_button_back";
    MenuButton* languageButton = new MenuButton(lngbtn,font,42,200,this,0);
    MenuButton* resolutionButton = new MenuButton(rbtn,font,42,250,this,1);
    MenuButton* textureQualityButton = new MenuButton(txbtn,font,42,300,this,2);
    MenuButton* fpsCapButton = new MenuButton(fpsbtn,font,42,350,this,3);
    MenuButton* showDebugInfo = new MenuButton(sdbgbtn,font,42,400,this,4);
    MenuButton* backButton = new MenuButton(bkbtn,font,42,500,this,5);
    languageButton->AddEndString(config->GetLanguageName());
    resolutionButton->AddEndString(resolutions[curResPos]);
    textureQualityButton->AddEndString(texture_quality[curTextureQualPos]);
    fpsCapButton->AddEndString(fps_options[curFpsCapPos]);
    showDebugInfo->AddEndString(show_debug_options[curShowDebugPos]);
    buttons.push_back(*languageButton);
    buttons.push_back(*resolutionButton);
    buttons.push_back(*textureQualityButton);
    buttons.push_back(*fpsCapButton);
    buttons.push_back(*showDebugInfo);
    buttons.push_back(*backButton);
    buttons[0].SetSelected(true);
}
void OptionsButtonList::Show(){
    resolutions.clear();
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_1280x720"));
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_1366x768"));
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_1366x700"));
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_1600x900"));
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_1600x800"));
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_1920x1080"));
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_1920x980"));
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_2560x1440"));
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_2560x1240"));
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_3840x2160"));
    resolutions.push_back(config->strRepo.GetUnicodeString(L"resolution_3840x1960"));

    texture_quality.clear();
    texture_quality.push_back(config->strRepo.GetUnicodeString(L"texture_quality_high"));
    texture_quality.push_back(config->strRepo.GetUnicodeString(L"texture_quality_medium"));
    texture_quality.push_back(config->strRepo.GetUnicodeString(L"texture_quality_low"));

    fps_options.clear();
    fps_options.push_back(config->strRepo.GetUnicodeString(L"fps_cap_60"));
    fps_options.push_back(config->strRepo.GetUnicodeString(L"fps_cap_120"));
    fps_options.push_back(config->strRepo.GetUnicodeString(L"fps_cap_144"));
    fps_options.push_back(config->strRepo.GetUnicodeString(L"fps_cap_240"));
    fps_options.push_back(config->strRepo.GetUnicodeString(L"fps_cap_0")+L" "+config->strRepo.GetUnicodeString(L"fps_cap_unlimited"));

    show_debug_options.clear();
    show_debug_options.push_back(config->strRepo.GetUnicodeString(L"option_hide_debug_info"));
    show_debug_options.push_back(config->strRepo.GetUnicodeString(L"option_show_debug_info"));

    curShowDebugPos = config->GetInt("showDebugMsg");
    curTextureQualPos = config->GetInt("textureQuality")-1;

    string fpsCapStr = config->GetString("framerateLimit");
    if (fpsCapStr!="0"){
        std::wstring resws;
        resws.assign(fpsCapStr.begin(), fpsCapStr.end());
        std::vector<wstring>::iterator rit = std::find(fps_options.begin(), fps_options.end(), resws);
        if (rit == fps_options.end())
        {
            /// Element not in list. Push fps onto the end and then get the position of the current
            fps_options.push_back(resws);
        }

        std::vector<wstring>::iterator ritt = std::find(fps_options.begin(), fps_options.end(), resws);
        int rindex = std::distance(fps_options.begin(), ritt);
        curFpsCapPos = rindex;
    } else {
        curFpsCapPos = 4;
    }

    string resolution = config->GetString("resX")+"x"+config->GetString("resY");
    std::wstring ws;
    ws.assign(resolution.begin(), resolution.end());
    std::vector<wstring>::iterator it = std::find(resolutions.begin(), resolutions.end(), ws);
    if (it == resolutions.end())
    {
        /// Element not in list. Push resolution onto the end and then get the position of the current
        resolutions.push_back(ws);
    }
    std::vector<wstring>::iterator itt = std::find(resolutions.begin(), resolutions.end(), ws);
    int index = std::distance(resolutions.begin(), itt);
    curResPos = index;

    buttons[0].AddEndString(config->GetLanguageName());
    buttons[1].AddEndString(resolutions[curResPos]);
    buttons[2].AddEndString(texture_quality[curTextureQualPos]);
    buttons[3].AddEndString(fps_options[curFpsCapPos]);
    buttons[4].AddEndString(show_debug_options[curShowDebugPos]);
    optionMenu->madeChanges=false;
}
void OptionsButtonList::SelectButton(int index){
    currentIndex = index;
    int curLang = config->GetInt("lang");
    switch (currentIndex){
            case 0:
                /// language button
                {
                if (curLang<maxLang){
                    int moreLang = curLang+1;
                    std::string text = "";
                    text += std::to_string(moreLang);
                    config->SetString("lang",text);
                } else {
                    config->SetString("lang","1");
                }
                config->changedLang=true;
                optionMenu->madeChanges=true;
                buttons[currentIndex].AddEndString(config->GetLanguageName());
                break;
                }
            case 1:
                /// resolution
                if (curResPos<resolutions.size()-1){
                    curResPos++;
                    std::wstring resXtext = Func::Split(resolutions[curResPos],'x')[0];
                    std::string sx( resXtext.begin(), resXtext.end() );
                    std::wstring resYtext = Func::Split(resolutions[curResPos],'x')[1];
                    std::string sy( resYtext.begin(), resYtext.end() );
                    config->SetString("resX",sx);
                    config->SetString("resY",sy);
                } else {
                    curResPos = 0;
                    config->SetString("resX","1280");
                    config->SetString("resY","720");
                }
                buttons[currentIndex].AddEndString(resolutions[curResPos]);
                optionMenu->madeChanges=true;
                break;
            case 2:
                /// texture quality
                if (curTextureQualPos<texture_quality.size()-1){
                    curTextureQualPos++;
                    std::string text = "";
                    text += std::to_string(curTextureQualPos+1);
                    config->SetString("textureQuality",text);
                } else {
                    curTextureQualPos = 0;
                    config->SetString("textureQuality","1");
                }
                buttons[currentIndex].AddEndString(texture_quality[curTextureQualPos]);
                break;
            case 3:
                /// fps cap
                if (curFpsCapPos<fps_options.size()-1){
                    curFpsCapPos++;
                    if (curFpsCapPos==4){
                        config->SetString("framerateLimit","0");
                    } else {
                        std::wstring ws = fps_options[curFpsCapPos];
                        std::string s( ws.begin(), ws.end() );
                        config->SetString("framerateLimit",s);

                    }
                } else {
                    curFpsCapPos = 0;
                    std::wstring ws = fps_options[curFpsCapPos];
                    std::string s( ws.begin(), ws.end() );
                    config->SetString("framerateLimit",s);
                }
                buttons[currentIndex].AddEndString(fps_options[curFpsCapPos]);
                break;
            case 4:
                /// show debug info
                if (curShowDebugPos<show_debug_options.size()-1){
                    curShowDebugPos++;
                    std::string text = "";
                    text += std::to_string(curShowDebugPos);
                    config->SetString("showDebugMsg",text);
                } else {
                    curShowDebugPos = 0;
                    config->SetString("showDebugMsg","0");
                }
                buttons[currentIndex].AddEndString(show_debug_options[curShowDebugPos]);
                break;
            case 5:
                /// back to main menu
                config->SaveConfig();
                config->ReloadLanguages();
                for (int i=0;i<parentMenu->v4core->menus.size();i++){
                    Menu* currentMenu = parentMenu->v4core->menus[i];
                    currentMenu->UpdateButtons();
                }
                parentMenu->Back();
                break;
        }
}
OptionsButtonList::~OptionsButtonList()
{

}
