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
    json drum_defaults;
    
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

        BPM = song["bpm"];
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
    for(auto name : song["idle_loop"])
    {
        sb_idle_loop.push_back(sf::SoundBuffer{});
        if(!sb_idle_loop[sb_idle_loop.size()-1].loadFromFile(songPath+std::string(name)))
           SPDLOG_ERROR("Error loading 'idle_loop', file {} not found", songPath+std::string(name)); 
    }
    SPDLOG_DEBUG("Idle loop loaded. {} files found", sb_idle_loop.size());
    sc_idle_loop.max = static_cast<int>(sb_idle_loop.size());

    // prefever calm
    SPDLOG_DEBUG("Loading prefever calm");
    for(auto name : song["prefever_calm_loop"])
    {
        sb_prefever_calm_loop.push_back(sf::SoundBuffer{});
        if(!sb_prefever_calm_loop[sb_prefever_calm_loop.size()-1].loadFromFile(songPath+std::string(name)))
           SPDLOG_ERROR("Error loading 'idle_loop', file {} not found", songPath+std::string(name)); 
    }
    SPDLOG_DEBUG("Prefever calm loop loaded. {} files found", sb_prefever_calm_loop.size());
    sc_prefever_calm_loop.max = static_cast<int>(sb_prefever_calm_loop.size());
    
    // prefever intense
    SPDLOG_DEBUG("Loading prefever intense");
    for(auto name : song["prefever_intense_loop"])
    {
        sb_prefever_intense_loop.push_back(sf::SoundBuffer{});
        if(!sb_prefever_intense_loop[sb_prefever_intense_loop.size()-1].loadFromFile(songPath+std::string(name)))
           SPDLOG_ERROR("Error loading 'idle_loop', file {} not found", songPath+std::string(name)); 
    }
    SPDLOG_DEBUG("Prefever intense loop loaded. {} files found", sb_prefever_intense_loop.size());
    sc_prefever_intense_loop.max = static_cast<int>(sb_prefever_intense_loop.size());

    // fever
    SPDLOG_DEBUG("Loading fever");
    for(auto name : song["fever_loop"])
    {
        sb_fever_loop.push_back(sf::SoundBuffer{});
        if(!sb_fever_loop[sb_fever_loop.size()-1].loadFromFile(songPath+std::string(name)))
           SPDLOG_ERROR("Error loading 'idle_loop', file {} not found", songPath+std::string(name)); 
    }
    SPDLOG_DEBUG("Fever loop loaded. {} files found", sb_fever_loop.size());
    sc_fever_loop.max = static_cast<int>(sb_fever_loop.size());

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
        SPDLOG_DEBUG("Opening chant.json file, path: {}chant.json", themePath);
        std::ifstream c(themePath+"chant.json");
        c >> chant;
        c.close();

        std::string chantPath = themePath + "chant/";

        // sb_chant_fever["march"][0..x] <- for each command from commands.json
        // std::map<std::string, std::vector<sf::SoundBuffer>> sb_chant_prefever_calm; // chants for calm prefever section
        // std::map<std::string, std::vector<sf::SoundBuffer>> sb_chant_prefever_intense; // chants for intense prefever section
        // std::map<std::string, std::vector<sf::SoundBuffer>> sb_chant_fever; // chants for fever section 

        for (auto it = chant.begin(); it != chant.end(); it++)
        {
            std::string songName = it.key();
            json songType = it.value();

            SPDLOG_DEBUG("Loading chants for song type: {}", songName);
            sb_chant_prefever_calm.insert({songName, std::vector<sf::SoundBuffer>{}});
            sb_chant_prefever_intense.insert({songName, std::vector<sf::SoundBuffer>{}});
            sb_chant_fever.insert({songName, std::vector<sf::SoundBuffer>{}});

            for (auto name : songType["prefever_calm"])
            {
                sb_chant_prefever_calm[songName].push_back(sf::SoundBuffer{});
                if(!sb_chant_prefever_calm[songName][sb_chant_prefever_calm[songName].size()-1].loadFromFile(chantPath+std::string(name)))
                    SPDLOG_ERROR("Error loading 'sb_chant_prefever_calm', file {} not found", chantPath+std::string(name)); 
            }

            SPDLOG_DEBUG("Found {} prefever_calm files for song type: {}", sb_chant_prefever_calm[songName].size(), songName);
            sc_chant_prefever_calm[songName].max = static_cast<int>(sb_chant_prefever_calm[songName].size());

            for (auto name : songType["prefever_intense"])
            {
                sb_chant_prefever_intense[songName].push_back(sf::SoundBuffer{});
                if(!sb_chant_prefever_intense[songName][sb_chant_prefever_intense[songName].size()-1].loadFromFile(chantPath+std::string(name)))
                    SPDLOG_ERROR("Error loading 'sb_chant_prefever_intense', file {} not found", chantPath+std::string(name)); 
            }

            SPDLOG_DEBUG("Found {} prefever_intense files for song type: {}", sb_chant_prefever_intense[songName].size(), songName);
            sc_chant_prefever_intense[songName].max = static_cast<int>(sb_chant_prefever_intense[songName].size());

            for (auto name : songType["fever"])
            {
                sb_chant_fever[songName].push_back(sf::SoundBuffer{});
                if(!sb_chant_fever[songName][sb_chant_fever[songName].size()-1].loadFromFile(chantPath+std::string(name)))
                    SPDLOG_ERROR("Error loading 'sb_chant_fever', file {} not found", chantPath+std::string(name)); 
            }

            SPDLOG_DEBUG("Found {} fever files for song type: {}", sb_chant_fever[songName].size(), songName);
            sc_chant_fever[songName].max = static_cast<int>(sb_chant_fever[songName].size());
        }

        SPDLOG_DEBUG("All chants successfully loaded");
    }

    // ////////////////// //
    //       Drums        //
    ////////////////////////

    //First, we load up the default drums. Then we overwrite the default drums with custom drums, if they exist

    SPDLOG_DEBUG("Loading default drums from drum_defaults.json.");

    std::ifstream dd("resources/sfx/drum_defaults.json");
    dd >> drum_defaults;
    dd.close();

    std::string drumPath = "resources/sfx/drums/";
    
    for (auto x : drumTable)
    {
        for (auto y : drumQualityTable)
        {
            for (auto z : drumTypeTable)
            {
                DrumType drumType = z.second;
                std::map<DrumType, sf::SoundBuffer> zz;
                zz.insert({drumType, sf::SoundBuffer{}});

                DrumQuality drumQuality = y.second;
                std::map<DrumQuality, std::map<DrumType, sf::SoundBuffer>> yy;
                yy.insert({drumQuality, zz});

                Drums drumName = x.second;
                sb_drum.insert({drumName, yy});

                std::string name = drum_defaults[x.first][y.first][z.first];

                if(!sb_drum[drumName][drumQuality][drumType].loadFromFile(drumPath + name))
                {
                    SPDLOG_ERROR("Couldn't load drum file {} from defaults. Something is wrong with the game installation or the drum_defaults.json file.", drumPath + name);
                }
            }
        }
    }

    SPDLOG_DEBUG("Default drums loaded.");

    // now we search for the custom drums

    if(!std::filesystem::exists(themePath + "drum.json"))
    {
        SPDLOG_DEBUG("drums.json doesn't exist, default drums will be kept.");
    }
    else
    {
        std::ifstream d(themePath + "drum.json");
        d >> drum;
        d.close();

        drumPath = themePath + "drum/";

        for (auto it = drum.begin(); it != drum.end(); it++)
        {
            std::string drumName = it.key();
            Drums key_drum = drumToEnum(drumName);

            json drumQuality = it.value();

            for (auto it2 = drumQuality.begin(); it2 != drumQuality.end(); it2++)
            {
                std::string qualityName = it2.key();
                DrumQuality key_quality = drumQualityToEnum(qualityName);

                json drumType = it2.value();

                for (auto it3 = drumType.begin(); it3 != drumType.end(); it3++)
                {
                    std::string drumTypeName = it3.key();
                    DrumType key_type = drumTypeToEnum(drumTypeName);

                    std::string drumTypeFile = it3.value();

                    if(!sb_drum[key_drum][key_quality][key_type].loadFromFile(drumPath+drumTypeFile))
                        SPDLOG_ERROR("Error loading drum {} quality {} type {}, file {} not found. Will keep the defaults.", drumName, qualityName, drumTypeName, drumPath+drumTypeFile);
                    else
                        SPDLOG_DEBUG("Custom drum {} quality {} type {}, file {} successfully loaded.", drumName, qualityName, drumTypeName, drumPath+drumTypeFile);
                }
            }
        }
    }
}

sf::SoundBuffer& SongController::getDrum(Drums drum, DrumQuality drumQuality, DrumType drumType)
{
    return sb_drum[drum][drumQuality][drumType];
}

sf::SoundBuffer& SongController::getSong(SongType songType)
{
    switch(songType)
    {
        case SongType::IDLE:
            if(sc_idle_loop.current == sc_idle_loop.max)
                sc_idle_loop.current = 0;
            return sb_idle_loop[sc_idle_loop.current++];
        break;

        case SongType::PREFEVER_CALM:
            if(sc_prefever_calm_loop.current == sc_prefever_calm_loop.max)
                sc_prefever_calm_loop.current = 0;
            return sb_prefever_calm_loop[sc_prefever_calm_loop.current++];
        break;
        
        case SongType::PREFEVER_INTENSE:
            if(sc_prefever_intense_loop.current == sc_prefever_intense_loop.max)
                sc_prefever_intense_loop.current = 0;
            return sb_prefever_intense_loop[sc_prefever_intense_loop.current++];
        break;
        
        case SongType::FEVER:
            if(sc_fever_loop.current == sc_fever_loop.max)
                sc_fever_loop.current = 0;
            return sb_fever_loop[sc_fever_loop.current++];
        break;
        
        case SongType::START:
            return sb_start;
        break;
        
        case SongType::PREFEVER_INTENSE_START:
            return sb_prefever_intense_start;
        break;
        
        case SongType::FEVER_START:
            return sb_fever_start;
        break;

        default:
            SPDLOG_ERROR("Unknown song tried to play.");
            throw SongControllerException("Unknown song tried to play.");
        break;
    }
}

float SongController::getBPM()
{
    return BPM;
}

void SongController::flushOrder()
{
    sc_idle_loop.current = 0;
    sc_prefever_calm_loop.current = 0;
    sc_prefever_intense_loop.current = 0;
    sc_fever_loop.current = 0;
}

SongController::Drums SongController::drumToEnum(std::string drum)
{

    return drumTable.at(drum);
}

SongController::DrumQuality SongController::drumQualityToEnum(std::string drumQuality)
{
    
    return drumQualityTable.at(drumQuality);
}

SongController::DrumType SongController::drumTypeToEnum(std::string drumType)
{

    return drumTypeTable.at(drumType);
}
