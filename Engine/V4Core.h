#ifndef V4CORE_H
#define V4CORE_H

#include <SFML/Graphics.hpp>
#include <random>
#include <discord.h>

class V4Core
{
private:
    sf::Font f_font;
    sf::Text t_version;
    sf::Text t_fps;

    float fps = 60;
    sf::Clock fpsclock;

public:
    std::string hero_version = PATAFOUR_VERSION;

    std::mt19937::result_type seed;
    std::mt19937 gen;

    std::string mission_file = "mis1_0.p4m";
    int mission_id = 0;
    float mission_multiplier = 1;

    int framerate_limit = 60;
    std::vector<float> frame_times;

    std::string rpc_details;
    std::string rpc_current;

    discord::Core* core{};

    V4Core();
    void changeRichPresence(std::string title, std::string bg_image, std::string sm_image);
    bool close_window = false;
    float getFPS();
    void init();
};
#endif // V4CORE_H
