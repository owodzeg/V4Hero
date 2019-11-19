#ifndef OPTIONSBUTTONLIST_H
#define OPTIONSBUTTONLIST_H

#include <string>
#include <SFML/Graphics.hpp>
#include "../MenuButton.h"
#include "../../Config.h"
#include "../../Mission/MissionController.h"
#include "../../Graphics/Menu.h"
#include "../ButtonList.h"
class Menu;
class OptionsMenu;
class OptionsButtonList : public ButtonList
{
    public:
        const int maxLang = 5;
        int curResPos;
        int curTextureQualPos;
        int curFpsCapPos;
        int curShowDebugPos;
        OptionsMenu* optionMenu;
        std::vector<std::wstring> resolutions;
        std::vector<std::wstring> texture_quality;
        std::vector<std::wstring> fps_options;
        std::vector<std::wstring> show_debug_options;
        void Initialise(sf::Font *font,Config &newConfig, std::map<int,bool> *keymap,MissionController *controller,OptionsMenu *parentMenu);
        void SelectButton(int index);
        void Show();
        OptionsButtonList();
        ~OptionsButtonList();

};


#endif // OPTIONSBUTTONLIST_H
