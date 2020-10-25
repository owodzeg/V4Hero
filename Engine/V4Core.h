#ifndef V4CORE_H
#define V4CORE_H

#include <SFML/Graphics.hpp>

#include "Rhythm/Rhythm.h"
#include "Mission/Background.h"
#include "Mission/Camera.h"
#include "Config.h"
#include "StringRepository.h"
#include "Mission/MissionController.h"
#include "Menu/MainMenu.h"
#include "Graphics/Menu.h"
#include "SaveReader.h"
#include "Menu/NewGameMenu.h"
#include "TipsUtil.h"
#include "../discord-cpp/discord.h"

#include "Input/InputController.h"

class V4Core
{
    private:
    //MissionController currentController;
        std::vector<float> tot_fps;
        sf::Thread* loadingThreadInstance;

        sf::Font f_font;
        sf::Text t_debug;
        sf::Text t_version;
        sf::Text t_fps;
        float fps = 60;
        sf::Clock fpsclock;

    bool leftkey=false,rightkey=false,upkey=false,downkey=false;
    public:
        std::string hero_version = "v1.1.1";
        sf::RenderWindow window;

        NewGameMenu newGameMenu;
        MainMenu mainMenu;
        Config config;
        TipsUtil tipsUtil;
        SaveReader savereader;
        MissionController currentController;
        V4Core();
        void SaveToDebugLog(string data);
        void ChangeRichPresence(string title, string bg_image, string sm_image);
        std::vector<Menu*> menus;
        bool closeWindow=false;
        void Init();
        void ShowTip();
        void LoadingThread();
        void LoadingWaitForKeyPress();
        bool continueLoading;
        bool pressAnyKey = false;
        int framerateLimit = 60;
        vector<float> frameTimes;

        InputController inputCtrl;

        struct DiscordState {
            discord::User currentUser;

            std::unique_ptr<discord::Core> core;
        };

        string rpc_details;
        string rpc_current;

        DiscordState state{};
        discord::Core* core{};
};
#endif // V4CORE_H
