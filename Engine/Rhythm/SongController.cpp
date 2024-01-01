#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "SongController.h"
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <fstream>

using json = nlohmann::json;


SongController::SongController()
{

}

void SongController::LoadTheme(std::string theme)
{
    SPDLOG_DEBUG("Loading theme {}", theme);

    json song;
    json chant;
    json drum;
    
    std::string themePath = "resources/sfx/bgm/"+theme+"/";

    if(!std::filesystem::exists(themePath + "song.json"))
        throw SongControllerException("song.json doesn't exist. Theme cannot be loaded.");

    SPDLOG_DEBUG("Opening song.json file, path: {}song.json", themePath);
    std::ifstream s(themePath+"song.json");
    s >> song;
    s.close();

    SPDLOG_DEBUG("Verifying song.json");

    std::vector<std::string> necessary_keys = {"bpm", "start", "idle_loop", "prefever_calm_loop", "prefever_intense_start", "prefever_intense_loop", "fever_start", "fever_loop"};

    for(auto x : necessary_keys)
    {
        if(song[x].empty())
            throw SongControllerException("Important key " + x + " missing in song.json. Theme will not be loaded.");
    }

    // ////////////////// //
    //        Song        //
    ////////////////////////

    std::string songPath = themePath + "song/";

    // load up individual song parts
    SPDLOG_DEBUG("Loading individual song parts");
    
    if(!sb_start.loadFromFile(songPath+std::string(song["start"])))
        SPDLOG_ERROR("Error loading 'start', file {} not found", songPath+std::string(song["start"]));

    if(!sb_prefever_intense_start.loadFromFile(songPath+std::string(song["prefever_intense_start"])))
        SPDLOG_ERROR("Error loading 'prefever_intense_start', file {} not found", songPath+std::string(song["prefever_intense_start"]));
    
    if(!sb_fever_start.loadFromFile(songPath+std::string(song["fever_start"])))
        SPDLOG_ERROR("Error loading 'fever_start', file {} not found", songPath+std::string(song["fever_start"]));

    // idle loop
    SPDLOG_DEBUG("Loading idle loop");
    for(auto x : song["idle_loop"])
    {
        sb_idle_loop.push_back(sf::SoundBuffer{});
        if(!sb_idle_loop[sb_idle_loop.size()-1].loadFromFile(songPath+std::string(x)))
           SPDLOG_ERROR("Error loading 'idle_loop', file {} not found", songPath+std::string(x)); 
    }
    SPDLOG_DEBUG("Idle loop loaded. {} files found", sb_idle_loop.size());

    // prefever calm
    SPDLOG_DEBUG("Loading prefever calm");
    for(auto x : song["prefever_calm_loop"])
    {
        sb_prefever_calm_loop.push_back(sf::SoundBuffer{});
        if(!sb_prefever_calm_loop[sb_prefever_calm_loop.size()-1].loadFromFile(songPath+std::string(x)))
           SPDLOG_ERROR("Error loading 'idle_loop', file {} not found", songPath+std::string(x)); 
    }
    SPDLOG_DEBUG("Prefever calm loop loaded. {} files found", sb_prefever_calm_loop.size());
    
    // prefever intense
    SPDLOG_DEBUG("Loading prefever intense");
    for(auto x : song["prefever_intense_loop"])
    {
        sb_prefever_intense_loop.push_back(sf::SoundBuffer{});
        if(!sb_prefever_intense_loop[sb_prefever_intense_loop.size()-1].loadFromFile(songPath+std::string(x)))
           SPDLOG_ERROR("Error loading 'idle_loop', file {} not found", songPath+std::string(x)); 
    }
    SPDLOG_DEBUG("Prefever intense loop loaded. {} files found", sb_prefever_intense_loop.size());

    // fever
    SPDLOG_DEBUG("Loading fever");
    for(auto x : song["fever_loop"])
    {
        sb_fever_loop.push_back(sf::SoundBuffer{});
        if(!sb_fever_loop[sb_fever_loop.size()-1].loadFromFile(songPath+std::string(x)))
           SPDLOG_ERROR("Error loading 'idle_loop', file {} not found", songPath+std::string(x)); 
    }
    SPDLOG_DEBUG("Fever loop loaded. {} files found", sb_fever_loop.size());

    SPDLOG_INFO("Song {} loaded, full path: {}, loading chants", theme, songPath);

    // ////////////////// //
    //       Chants       //
    ////////////////////////

    if(!std::filesystem::exists(themePath + "chant.json"))
    {
        SPDLOG_WARN("chant.json doesn't exist, theme will have no chants");
    }
    else
    {
        SPDLOG_DEBUG("Opening chant.json file, path: {}/chant.json", themePath);
        std::ifstream c(themePath+"chant.json");
        c >> chant;
        c.close();
    }

    // ////////////////// //
    //       Drums        //
    ////////////////////////

    if(!std::filesystem::exists(themePath + "drum.json"))
    {
        SPDLOG_DEBUG("drums.json doesn't exist, default drums will be loaded.");
    }
    else
    {

    }
}