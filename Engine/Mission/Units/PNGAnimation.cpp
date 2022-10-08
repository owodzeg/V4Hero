#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "PNGAnimation.h"
#include <filesystem>
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

PNGAnimation::PNGAnimation()
{

}

void PNGAnimation::Load(const std::string& path)
{
    SPDLOG_INFO("Loading PNGAnimation model from {}", path);

    auto f = fs::path(path);
    
    if (f.extension() == ".zip")
    {
        SPDLOG_INFO(".zip was provided, we need to extract the file");
    } else if (fs::is_directory(path))
    {
        SPDLOG_INFO("directory was provided, no need to extract the file");

        //Step 1: Load animation names and frame paths
        for (const auto& entry : fs::directory_iterator(path))
        {
            Animation tmp;

            SPDLOG_DEBUG("Animation found: {}", entry.path().string());
            tmp.name = entry.path().string();

            for (const auto& frame : fs::directory_iterator(entry.path()))
            {
                SPDLOG_DEBUG("Animation frame found: {}", frame.path().string());
                tmp.frame_paths.push_back(frame.path().string());
            }

            animations.push_back(tmp);
        }

        //Step 2: Compose frames into a spritesheet for each animation

        //Step 3: Load spritesheets to ResourceManager
    }
}

void PNGAnimation::Draw()
{

}