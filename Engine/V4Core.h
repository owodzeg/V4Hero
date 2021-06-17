#ifndef V4CORE_H
#define V4CORE_H

#include <SFML/Graphics.hpp>

#include <random>

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

#include "Input/InputController.h"

class V4Core
{
    private:
    //MissionController currentController;
        std::vector<float> tot_fps;
        sf::Thread* loading_thread_instance;

        sf::Font f_font;
        sf::Text t_debug;
        sf::Text t_version;
        sf::Text t_fps;
        float fps = 60;
        sf::Clock fpsclock;

    public:
        bool left_key=false, right_key=false, up_key=false, down_key=false;
        std::string hero_version = "v1.1.1";
        sf::RenderWindow window;

		std::mt19937::result_type seed;
		std::mt19937 gen;

        NewGameMenu newGameMenu;
        MainMenu mainMenu;
        Config config;
        TipsUtil tipsUtil;
        SaveReader saveReader;
        MissionController currentController;
        V4Core();
        void saveToDebugLog(string data);
        void changeRichPresence(string title, string bg_image, string sm_image);
        std::vector<Menu*> menus;
        bool close_window=false;
        void init();
        void showTip();
        void loadingThread();
        void loadingWaitForKeyPress();
        bool continue_loading;
        bool press_any_key = false;
        int framerate_limit = 60;
        vector<float> frame_times;

        InputController inputCtrl;

        /*struct DiscordState {
            discord::User currentUser;

            std::unique_ptr<discord::Core> core;
        };*/

        string rpc_details;
        string rpc_current;

        //DiscordState state{};
        //discord::Core* core{};
};
#endif // V4CORE_H
