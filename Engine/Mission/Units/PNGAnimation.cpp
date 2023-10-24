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
            // make sure we are reading only directories, we don't want to catch config files as animations :)
            if (fs::is_directory(entry))
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
        }

        //Step 2: Compose frames into a spritesheet for each animation
        int maxSize = sf::Texture::getMaximumSize();

        for (Animation x : animations)
        {
            // load a sample image for spritesheet calculation
            sf::Image thumb;
            thumb.loadFromFile(x.frame_paths[0]);

            int img_x = thumb.getSize().x;
            int img_y = thumb.getSize().y;
            int frames = x.frame_paths.size();

            int x_size = img_x * frames;
            int rows = ceil(float(x_size) / float(maxSize));

            int maxCols = floor(float(maxSize) / float(img_x));
            int maxRows = floor(float(maxSize) / float(img_y));
            int sheetsNeeded = ceil(float(rows) / float(maxRows));

            int maxFramesPerSheet = maxCols * maxRows;

            SPDLOG_INFO("Animation info dump: name {} img_x {} img_y {} frames {} x_size {} rows {} maxCols {} maxRows {} sheetsNeeded {} maxFramesPerSheet {}", x.name, img_x, img_y, frames, x_size, rows, maxCols, maxRows, sheetsNeeded, maxFramesPerSheet);

            sf::Image spritesheet_buffer;
            int spritesheetXsize = maxCols * img_x;
            int spritesheetYsize = maxRows * img_y;

            if (rows < maxRows)
                spritesheetYsize = rows * img_y;

            if (rows == 1)
                spritesheetXsize = frames * img_x;

            spritesheet_buffer.create(spritesheetXsize, spritesheetYsize, sf::Color(255, 255, 255, 0));

            std::vector<sf::Image> readySheets;

            int framesLeft = maxFramesPerSheet;
            int frameBuffer = 0;

            if (framesLeft > frames)
                framesLeft = frames;
            
            for (auto f : x.frame_paths)
            {
                sf::Image f_img;
                f_img.loadFromFile(f);

                int curCol = frameBuffer % maxCols;
                int curRow = floor(float(frameBuffer) / float(maxCols));
                spritesheet_buffer.copy(f_img, curCol * img_x, curRow * img_y);

                --framesLeft;
                ++frameBuffer;

                if (framesLeft <= 0)
                {
                    framesLeft = maxFramesPerSheet;
                    frameBuffer = 0;

                    if (framesLeft > frames)
                        framesLeft = frames;

                    readySheets.push_back(spritesheet_buffer);
                    spritesheet_buffer.create(spritesheetXsize, spritesheetYsize, sf::Color(255, 255, 255, 0));
                }
            }

            for (auto s : readySheets)
            {
                s.saveToFile(x.name+"_spr.png");
                SPDLOG_DEBUG("saved preview to {}", x.name + "_spr.png");
            }
        }

        //Step 3: Load spritesheets to ResourceManager
    }
}

void PNGAnimation::Draw()
{

}