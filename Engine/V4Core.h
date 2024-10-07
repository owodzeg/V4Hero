#ifndef V4CORE_H
#define V4CORE_H

#include <SFML/Graphics.hpp>
#include <random>
#include <discord.h>
#include <Graphics/PText.h>

// Main game class that handles the engine
class V4Core
{
private:
    // Static GUI always on screen

    PText t_version, t_fps;

    // Variables for delta time calc
    float fps = 60;
    sf::Clock fpsclock;
    unsigned int framerate_limit = 60;
    std::vector<float> frame_times;

    // Fetch current version from CMakeLists.txt
    std::string hero_version = PATAFOUR_VERSION;

    // Discord Rich Presence variables
    std::string rpc_details;
    std::string rpc_current;
    discord::Core* core{};

public:
    // Randomization purposes
    std::mt19937::result_type seed;
    std::mt19937 gen;

    // Mission variables
    std::string mission_file = "mis1_0.p4m";
    int mission_id = 0;
    float mission_multiplier = 1;

    // Misc
    bool close_window = false;

    // Functions
    V4Core();
    void changeRichPresence(std::string title, std::string bg_image, std::string sm_image);
    float getFPS();
    void init();
};
#endif // V4CORE_H
