#ifndef V4CORE_H
#define V4CORE_H

#include <SFML/Graphics.hpp>

#include <random>

#include "Config.h"
#include "Graphics/Menu.h"
#include "Menu/MainMenu.h"
#include "Mission/Background.h"
#include "Mission/Camera.h"
#include "Mission/MissionController.h"
#include "Rhythm/Rhythm.h"
#include "SaveReader.h"
#include "StringRepository.h"
#include "TipsUtil.h"
#include "ResourceManager.h"
#include "TextureManager.h"

#include "Input/InputController.h"

#include "Mission/Units/AnimatedObject.h"
#include "Mission/Units/Object.h"

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
    bool left_key = false, right_key = false, up_key = false, down_key = false;
    std::string hero_version = PATAFOUR_VERSION;
    sf::RenderWindow window;

    std::mt19937::result_type seed;
    std::mt19937 gen;

    /// Okay so this right here will cache the animations
    /// So we can make more efficient loading times and less work
    /// It's purpose is to store everything that is drawable, so Objects and Spritesheets
    /// NOTE FROM 31.08.2021 /owocek
    /// I've changed the location of AnimationCache to here so places like Barracks can also use it without having to use an inactive MissionController for that
    struct AnimationCache {
        shared_ptr<vector<vector<sf::Image>>> swaps;
        shared_ptr<vector<AnimatedObject::Animation>> spritesheet;
        shared_ptr<vector<Object>> objects;
    };

    std::map<int, std::shared_ptr<AnimationCache>> animation_cache;
    std::map<int, bool> isCached; ///Check if entities have been cached already, so we can make automatic caching inside spawnEntity function

    //Config config;
    SaveReader saveReader;
    V4Core();
    void cacheEntity(int entityID, shared_ptr<vector<vector<sf::Image>>> swaps, shared_ptr<vector<AnimatedObject::Animation>> spritesheet, shared_ptr<vector<Object>> objects);
    void changeRichPresence(string title, string bg_image, string sm_image);
    std::vector<Menu*> menus;
    bool close_window = false;
    float getFPS();
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
