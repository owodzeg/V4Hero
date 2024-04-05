#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "PNGAnimation.h"
#include "ResourceManager.h"
#include "CoreManager.h"
#include "../../Func.h"
#include <filesystem>
#include <sstream>
#include <spdlog/spdlog.h>
#include <exception>
#include <fstream>
#include <regex>

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

        //Step 0: Check config for downscaling information
        int quality = CoreManager::getInstance().getConfig()->GetInt("textureQuality");
        int ratio = 1;

        switch (quality)
        {
            case 0: {
                ratio = 6;
                break;
            }

            case 1: {
                ratio = 3;
                break;
            }

            case 2: {
                ratio = 2;
                break;
            }
        }

        //Step 1: Load animation names and frame paths
        for (const auto& entry : fs::directory_iterator(path))
        {
            // make sure we are reading only directories, we don't want to catch config files as animations :)
            if (fs::is_directory(entry))
            {
                Animation tmp;

                SPDLOG_DEBUG("Animation found: {}", entry.path().string());
                tmp.name = entry.path().string();
                tmp.shortName = tmp.name.substr(tmp.name.find_last_of("\\/")+1);

                // check if animation is already in cache - it was loaded earlier
                auto cache_path = fs::path("resources/graphics/.tex_cache");
                bool cache_found = false;

                // cache folder does not exist - create it
                if(!std::filesystem::exists(cache_path))
                {
                    std::filesystem::create_directory(cache_path);
                }

                // go through cache to find already generated spritesheets
                for (const auto& cache_entry : fs::directory_iterator(cache_path))
                {
                    std::string cache_entry_name = cache_entry.path().string();
                    std::regex cache_regex(tmp.shortName+"_spr_(\\d*)\\.png");
                    std::smatch matches;

                    if (std::regex_search(cache_entry_name, matches, cache_regex))
                    {
                        // cached sheet found
                        SPDLOG_INFO("Animation {} found in .tex_cache: sheet file {}. No need to regenerate spritesheets.", tmp.shortName, cache_entry_name);
                        tmp.cached = true;
                        tmp.spritesheet_paths.push_back(cache_entry_name);
                        cache_found = true;
                    }
                }

                // we found cache, so we can pack it up and quit here
                if(cache_found)
                {
                    // sort sheets in proper order
                    std::sort(tmp.spritesheet_paths.begin(), tmp.spritesheet_paths.end());

                    // load animation data
                    std::string anim_data;
                    std::stringstream ss_anim_data;
                    std::string anim_data_path = std::format("resources/graphics/.tex_cache/{}.anim", tmp.shortName);
                    std::ifstream anim_data_file(anim_data_path);
                    ss_anim_data << anim_data_file.rdbuf();
                    anim_data = ss_anim_data.str();
                    std::vector<std::string> args = Func::Split(anim_data, ' ');
                    if(args.size() == 5)
                    {
                        tmp.img_x = atoi(args[0].c_str());
                        tmp.img_y = atoi(args[1].c_str());
                        tmp.frames = atoi(args[2].c_str());
                        tmp.maxCols = atoi(args[3].c_str());
                        tmp.maxRows = atoi(args[4].c_str());
                    }
                    else
                    {
                        SPDLOG_ERROR("Invalid data in animation data file! file={}. expected 5 args, got {}", anim_data_path, args.size());
                        throw std::exception();
                    }

                    animations.push_back(tmp);
                    continue; // no need to seek for frames if we got the spritesheet ready.
                }

                // animation not found in cache - load frames
                for (const auto& frame : fs::directory_iterator(entry.path()))
                {
                    SPDLOG_DEBUG("Animation frame found: {}", frame.path().string());
                    tmp.frame_paths.push_back(frame.path().string());
                }

                std::sort(tmp.frame_paths.begin(), tmp.frame_paths.end());

                animations.push_back(tmp);
            }
        }

        //Step 2: Compose frames into a spritesheet for each animation
        unsigned int maxSize = 1024;//sf::Texture::getMaximumSize();
        SPDLOG_DEBUG("maximum texture size: {}", maxSize);

        for (Animation &x : animations)
        {
            if(!x.cached) // skip spritesheet generation if animation cache found
            {
                // load a sample image for spritesheet calculation
                TextureManager::getInstance().loadImageFromFile(x.frame_paths[0]);
                TextureManager::getInstance().scaleTexture(x.frame_paths[0], ratio, false);
                sf::Image thumb = TextureManager::getInstance().getImage(x.frame_paths[0]);
                //thumb.loadFromFile(x.frame_paths[0]);

                int img_x = thumb.getSize().x;
                int img_y = thumb.getSize().y;

                TextureManager::getInstance().unloadImage(x.frame_paths[0]);
                TextureManager::getInstance().unloadTexture(x.frame_paths[0]);

                int frames = x.frame_paths.size();

                int x_size = img_x * frames;
                int rows = ceil(float(x_size) / float(maxSize));

                int maxCols = floor(float(maxSize) / float(img_x));
                int maxRows = floor(float(maxSize) / float(img_y));
                int sheetsNeeded = ceil(float(rows) / float(maxRows));

                int maxFramesPerSheet = maxCols * maxRows;

                SPDLOG_INFO("Animation info dump: name {} img_x {} img_y {} frames {} x_size {} rows {} maxCols {} maxRows {} sheetsNeeded {} maxFramesPerSheet {}", x.name, img_x, img_y, frames, x_size, rows, maxCols, maxRows, sheetsNeeded, maxFramesPerSheet);
                // push all the important info to the animation
                x.img_x = img_x;
                x.img_y = img_y;
                x.frames = frames;
                x.maxCols = maxCols;
                x.maxRows = maxRows;

                // save the data to a file in cache so the information can be fetched later without having to go over the frames
                std::string anim_data = std::format("{} {} {} {} {}", img_x, img_y, frames, maxCols, maxRows);
                std::string anim_data_path = std::format("resources/graphics/.tex_cache/{}.anim", x.shortName);

                SPDLOG_DEBUG("Writing animation data into {}", anim_data_path);

                std::ofstream anim_data_file(anim_data_path, std::ios::trunc);
                anim_data_file << anim_data;

                SPDLOG_DEBUG("Wrote: {}", anim_data);

                anim_data_file.close();

                // when we're extracting the frames out of a spritesheet,
                // keep in mind that max frames in sheet are maxCols*maxRows
                // then decrement this amount of frames as you go through the sheets
                // to figure out how many frames are on the last sheet.

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
                int framesTotal = frames;

                if (framesLeft > frames)
                    framesLeft = frames;

                for (const auto& fr : x.frame_paths)
                {
                    TextureManager::getInstance().loadImageFromFile(fr);
                    TextureManager::getInstance().scaleTexture(fr, ratio, false);
                    sf::Image f_img = TextureManager::getInstance().getImage(fr);

                    int curCol = frameBuffer % maxCols;
                    int curRow = floor(float(frameBuffer) / float(maxCols));
                    spritesheet_buffer.copy(f_img, curCol * img_x, curRow * img_y);

                    --framesLeft;
                    --framesTotal;
                    ++frameBuffer;

                    if (framesLeft <= 0)
                    {
                        framesLeft = maxFramesPerSheet;
                        frameBuffer = 0;

                        if (framesLeft > framesTotal)
                            framesLeft = framesTotal;

                        readySheets.push_back(spritesheet_buffer);
                        spritesheet_buffer.create(spritesheetXsize, spritesheetYsize, sf::Color(255, 255, 255, 0));
                    }

                    TextureManager::getInstance().unloadImage(fr);
                    TextureManager::getInstance().unloadTexture(fr);
                }

                int sheetID = 1;
                for (const auto& s : readySheets)
                {
                    std::string spr_name = std::format("resources/graphics/.tex_cache/{}_spr_{}.png", x.shortName, sheetID);
                    s.saveToFile(spr_name);
                    SPDLOG_DEBUG("Saved spritesheet to {}", spr_name);
                    x.spritesheet_paths.push_back(spr_name);
                    sheetID += 1;
                }

                std::sort(x.spritesheet_paths.begin(), x.spritesheet_paths.end());
            }
        }

        //Step 3: Load spritesheets to ResourceManager
        SPDLOG_DEBUG("Load animations");
        for (Animation &x : animations)
        {
            for (const auto& spr : x.spritesheet_paths)
            {
                ResourceManager::getInstance().loadSprite(spr, false);
            }
        }

        //Step 4: Map vector IDs to short animation names
        int tmp_id = 0;
        for (Animation &x : animations)
        {
            animationIDtoName[x.shortName] = tmp_id;
            tmp_id++;
        }
    }
}

int PNGAnimation::getIDfromShortName(const std::string& shortName)
{
    if(animationIDtoName.find(shortName) != animationIDtoName.end())
    {
        return animationIDtoName.at(shortName);
    }
    else
    {
        SPDLOG_ERROR("Invalid animation name found!");
        throw std::exception(); // TODO: replace with a proper exception.
    }
}

void PNGAnimation::Draw()
{
    InputController* inputController = CoreManager::getInstance().getInputController();
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();

    if(inputController->isKeyPressed(Input::Keys::LEFT))
    {
        if(currentAnimation > 0)
            --currentAnimation;

        currentFrame = 0;
    }

    if(inputController->isKeyPressed(Input::Keys::RIGHT))
    {
        if (currentAnimation < animations.size()-1)
            ++currentAnimation;

        currentFrame = 0;
    }

    Animation& curAnim = animations[currentAnimation];

    currentFrame += animationSpeed / CoreManager::getInstance().getCore()->getFPS();

    if(currentFrame >= curAnim.frames)
        currentFrame = 0;

    auto currentFrameInt = static_cast<unsigned int>(floor(currentFrame));
    unsigned int maxFramesPerSheet = curAnim.maxCols * curAnim.maxRows;

    int currentSpritesheet = floor(currentFrameInt / maxFramesPerSheet);
    int currentSpritesheetFrame = currentFrameInt % maxFramesPerSheet;

    int currentRow = currentSpritesheetFrame / curAnim.maxCols;
    int currentCol = currentSpritesheetFrame % curAnim.maxCols;

    //SPDLOG_DEBUG("currentAnimation {} {}, currentFrame {}, currentFrameInt {}, maxFrames {}, animationSpeed {}, maxFramesPerSheet {}, currentSpritesheet {}, currentSpritesheetFrame {}, row {}, col {}", currentAnimation, curAnim.shortName, currentFrame, currentFrameInt, curAnim.frames, animationSpeed, maxFramesPerSheet, currentSpritesheet, currentSpritesheetFrame, currentRow, currentCol);

    int x_start = currentCol * curAnim.img_x;
    int y_start = currentRow * curAnim.img_y;

    sf::IntRect textureRect = {x_start, y_start, static_cast<int>(curAnim.img_x), static_cast<int>(curAnim.img_y)};
    //SPDLOG_DEBUG("setting texture rect to {} {} {} {}", textureRect.left, textureRect.top, textureRect.width, textureRect.height);

    auto& texture = ResourceManager::getInstance().getSprite(curAnim.spritesheet_paths[currentSpritesheet]);
    texture.setTextureRect(textureRect);

    //sf::RectangleShape rect(sf::Vector2f(curAnim.img_x, curAnim.img_y));
    //rect.setFillColor(sf::Color(255,0,0,100));
    //rect.setPosition(x_start, y_start);

    texture.draw();
    //window->draw(rect);
}