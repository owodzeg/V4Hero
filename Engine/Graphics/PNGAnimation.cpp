#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "PNGAnimation.h"
#include "ResourceManager.h"
#include "CoreManager.h"
#include "../Func.h"
#include <filesystem>
#include <sstream>
#include <spdlog/spdlog.h>
#include <exception>
#include <fstream>
#include <format>
#include <future>  // for std::async and std::future

using namespace libzippp;

namespace fs = std::filesystem;


bool isValidCacheEntry(const std::string& filename, const std::string& quality, const std::string& model_name, const std::string& anim_shortName)
{
    // Construct expected prefix pattern based on quality, model, and animation short name
    std::string prefix = quality + "@" + model_name + "@" + anim_shortName + "_spr_";

    // Ensure filename starts with the expected prefix
    if (filename.compare(0, prefix.size(), prefix) != 0)
    {
        return false;
    }

    // Ensure the file ends with ".png"
    if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".png")
    {
        return false;
    }

    // Extract the part that should be the integer
    std::string number_part = filename.substr(prefix.size(), filename.size() - prefix.size() - 4);

    // Ensure the remaining part is a valid unsigned integer
    return !number_part.empty() && std::all_of(number_part.begin(), number_part.end(), ::isdigit);
}

PNGAnimation::PNGAnimation()
{
    maxSize = sf::Texture::getMaximumSize();
}

sf::Image& PNGAnimation::getAnimationImage(const std::string& anim_path, const std::string& image_path, ZipArchive& zip_handle)
{
    std::lock_guard<std::mutex> guard(resource_mutex);

    SPDLOG_DEBUG("Providing animation file for {}, img: {}, zipped?: {}", anim_path, image_path, zip_handle.isOpen());

    if(zip_handle.isOpen())
    {
        char* data = (char*) zip_handle.readEntry(image_path, true);
        ZipEntry thumb_entry = zip_handle.getEntry(image_path);

        sf::Image img;
        if(!img.loadFromMemory(data, thumb_entry.getSize()))
        {
            SPDLOG_ERROR("Failed to load image from memory: {}", image_path);
            throw std::exception();
        }

        TextureManager::getInstance().loadImageFromMemory(image_path, img, false);

        if(TextureManager::getInstance().getRatio() != 1)
            TextureManager::getInstance().scaleTexture(image_path, TextureManager::getInstance().getRatio(), false);

        return TextureManager::getInstance().getImage(image_path);
    }
    else
    {
        TextureManager::getInstance().loadImageFromFile(image_path);

        if(TextureManager::getInstance().getRatio() != 1)
            TextureManager::getInstance().scaleTexture(image_path, TextureManager::getInstance().getRatio(), false);

        return TextureManager::getInstance().getImage(image_path);
    }
}

void PNGAnimation::loadCacheFile(Animation& anim)
{
    // load animation data
    std::string anim_data;
    std::stringstream ss_anim_data;
    std::string anim_data_path = std::format("resources/graphics/.tex_cache/{}@{}@{}.anim", CoreManager::getInstance().getConfig()->GetInt("textureQuality"), model_name, anim.shortName);
    std::ifstream anim_data_file(anim_data_path);
    ss_anim_data << anim_data_file.rdbuf();
    anim_data = ss_anim_data.str();
    std::vector<std::string> args = Func::Split(anim_data, ' ');
    if(args.size() == 5)
    {
        anim.img_x = stof(args[0]);
        anim.img_y = stof(args[1]);
        anim.frames = static_cast<unsigned int>(stoi(args[2]));
        anim.maxCols = static_cast<unsigned int>(stoi(args[3]));
        anim.maxRows = static_cast<unsigned int>(stoi(args[4]));
    }
    else
    {
        SPDLOG_ERROR("Invalid data in animation data file! file={}. expected 5 args, got {}", anim_data_path, args.size());
        throw std::exception();
    }
}

void PNGAnimation::generateSpritesheet(Animation& anim, const std::string& anim_path, ZipArchive& zf)
{
    std::sort(anim.frame_paths.begin(), anim.frame_paths.end());

    // we need to take the largest image out of the whole animation, in case the animation has different frame dimensions...
    unsigned int img_x = 0;
    unsigned int img_y = 0;

    for (const auto& thumb_path : anim.frame_paths)
    {
        sf::Image& thumb = getAnimationImage(anim_path, thumb_path, zf);

        if(thumb.getSize().x > img_x)
            img_x = thumb.getSize().x;
        if(thumb.getSize().y > img_y)
            img_y = thumb.getSize().y;

        TextureManager::getInstance().unloadImage(thumb_path);
    }

    if(!anim.zip)
        TextureManager::getInstance().unloadTexture(anim.frame_paths[0]);

    int frames = static_cast<int>(anim.frame_paths.size());

    int x_size = img_x * frames;
    int rows = static_cast<int>(ceil(float(x_size) / float(maxSize)));

    int maxCols = static_cast<int>(floor(float(maxSize) / float(img_x)));
    int maxRows = static_cast<int>(floor(float(maxSize) / float(img_y)));
    int sheetsNeeded = static_cast<int>(ceil(float(rows) / float(maxRows)));

    int maxFramesPerSheet = maxCols * maxRows;

    SPDLOG_DEBUG("Animation info dump: name {} img_x {} img_y {} frames {} x_size {} rows {} maxCols {} maxRows {} sheetsNeeded {} maxFramesPerSheet {}", anim.name, img_x, img_y, frames, x_size, rows, maxCols, maxRows, sheetsNeeded, maxFramesPerSheet);
    // push all the important info to the animation
    anim.img_x = static_cast<float>(img_x);
    anim.img_y = static_cast<float>(img_y);
    anim.frames = frames;
    anim.maxCols = maxCols;
    anim.maxRows = maxRows;

    // save the data to a file in cache so the information can be fetched later without having to go over the frames
    std::string anim_data = std::format("{} {} {} {} {}", img_x, img_y, frames, maxCols, maxRows);
    std::string anim_data_path = std::format("resources/graphics/.tex_cache/{}@{}@{}.anim", CoreManager::getInstance().getConfig()->GetInt("textureQuality"), model_name, anim.shortName);

    SPDLOG_TRACE("Writing animation data into {}", anim_data_path);

    std::ofstream anim_data_file(anim_data_path, std::ios::trunc);
    anim_data_file << anim_data;

    SPDLOG_TRACE("Wrote: {}", anim_data);

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

    spritesheet_buffer.resize(sf::Vector2u(spritesheetXsize, spritesheetYsize), sf::Color(255, 255, 255, 0));

    std::vector<sf::Image> readySheets;

    int framesLeft = maxFramesPerSheet;
    int frameBuffer = 0;
    int framesTotal = frames;

    if (framesLeft > frames)
        framesLeft = frames;

    for (const auto& fr : anim.frame_paths)
    {
        sf::Image& f_img = getAnimationImage(anim_path, fr, zf);

        int curCol = frameBuffer % maxCols;
        int curRow = static_cast<int>(floor(float(frameBuffer) / float(maxCols)));
        if (!spritesheet_buffer.copy(f_img, sf::Vector2u(curCol * img_x, curRow * img_y)))
        {
            SPDLOG_ERROR("Failed to copy image to spritesheet buffer");
            throw std::exception();
        }

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
            spritesheet_buffer.resize(sf::Vector2u(spritesheetXsize, spritesheetYsize), sf::Color(255, 255, 255, 0));
        }

        TextureManager::getInstance().unloadImage(fr);

        if(!anim.zip)
            TextureManager::getInstance().unloadTexture(fr);
    }

    int sheetID = 1;
    for (const auto& s : readySheets)
    {
        std::string spr_name = std::format("resources/graphics/.tex_cache/{}@{}@{}_spr_{}.png", CoreManager::getInstance().getConfig()->GetInt("textureQuality"), model_name, anim.shortName, sheetID);
        if (s.saveToFile(spr_name))
        {
            SPDLOG_DEBUG("Saved spritesheet to {}", spr_name);
            anim.spritesheet_paths.push_back(spr_name);
            sheetID += 1;
        } 
        else
        {
            SPDLOG_ERROR("Failed to save spritesheet to {}", spr_name);
            throw std::exception();
        }
    }

    std::sort(anim.spritesheet_paths.begin(), anim.spritesheet_paths.end());
}

bool PNGAnimation::getAnimationCache(Animation& anim)
{
    // check if animation is already in cache - it was loaded earlier
    auto cache_path = fs::path("resources/graphics/.tex_cache");
    bool cache_found = false;

    // cache folder does not exist - create it
    if (!fs::exists(cache_path))
    {
        fs::create_directory(cache_path);
    }

    // Get texture quality from the config
    std::string textureQuality = std::to_string(CoreManager::getInstance().getConfig()->GetInt("textureQuality"));

    // Iterate through the cache folder to find already generated spritesheets
    for (const auto& cache_entry : fs::directory_iterator(cache_path))
    {
        std::string cache_entry_name = cache_entry.path().filename().string();

        if (isValidCacheEntry(cache_entry_name, textureQuality, model_name, anim.shortName))
        {
            // cached sheet found
            SPDLOG_DEBUG("Animation {} found in .tex_cache: sheet file {}. No need to regenerate spritesheets.", anim.shortName, cache_entry_name);
            anim.cached = true;
            anim.spritesheet_paths.push_back(cache_entry.path().string());
            cache_found = true;
        }
    }

    return cache_found;
}

void PNGAnimation::Load(const std::string& path)
{
    SPDLOG_INFO("Loading PNGAnimation model from {}", path);

    auto f = fs::path(path);
    model_name = f.stem().string();

    std::vector<std::string> animation_names;
    std::vector<std::string> frame_names;
    bool zip = false;

    ZipArchive zf(f.string());

    // Step 1 - get all paths. Process varies between .zip and folder animations
    if (f.extension() == ".zip")
    {
        SPDLOG_INFO(".zip was provided, we need to extract the file");
        zip = true;

        zf.open(ZipArchive::ReadOnly);
        std::vector<ZipEntry> entries = zf.getEntries();

        SPDLOG_TRACE("Zip entries: {}", entries.size());

        // first detect all the names
        for (auto& entry : entries)
        {
            std::string name = entry.getName();

            SPDLOG_TRACE("ZipEntry: {}", name);

            if(((name.ends_with("\\") && !name.ends_with(".json")) || (name.ends_with("/") && !name.ends_with(".json"))))
            {
                SPDLOG_TRACE("Animation detected: {}", name);
                animation_names.push_back(name);
            }
            else
            {
                SPDLOG_TRACE("Frame detected: {}", name);
                frame_names.push_back(name);
            }
        }
    } else if (fs::is_directory(path))
    {
        SPDLOG_INFO("Directory was provided, no need to extract the file");

        for (const auto& entry : fs::directory_iterator(path))
        {
            // make sure we are reading only directories, we don't want to catch config files as animations :)
            if (fs::is_directory(entry))
            {
                SPDLOG_TRACE("Animation found: {}", entry.path().string());
                animation_names.push_back(entry.path().string());

                for (const auto& frame : fs::directory_iterator(entry.path()))
                {
                    SPDLOG_TRACE("[FD] Animation frame found: {}", frame.path().string());
                    frame_names.push_back(frame.path().string());
                }
            }
        }
    } else
    {
        throw PNGAnimationException("Invalid file detected. PNGAnimation supports only .png and .zip.");
    }

    // Step 2 - create animations, then assign frames to animations
    for (auto& anim_name : animation_names)
    {
        Animation tmp;

        tmp.name = anim_name;

        if(!zip)
            tmp.shortName = tmp.name.substr(tmp.name.find_last_of("\\/")+1); //folder-based name
        else
            tmp.shortName = tmp.name.substr(0, tmp.name.find_last_of("\\/")); //zip-based name

        tmp.zip = zip;
        SPDLOG_DEBUG("Animation found: {} {}, zip: {}", tmp.name, tmp.shortName, tmp.zip);

        // check if animation is already in cache - it was loaded earlier
        bool cache_found = getAnimationCache(tmp);

        // we found cache, so we can pack it up and quit here
        if(cache_found)
        {
            // sort sheets in proper order
            std::sort(tmp.spritesheet_paths.begin(), tmp.spritesheet_paths.end());

            loadCacheFile(tmp);
        }

        // we continue to seek after files anyways, because we'll perform a quick checksum check later

        for(auto& frame_name : frame_names)
        {
            if (frame_name.find(anim_name) != std::string::npos)
            {
                SPDLOG_TRACE("[FP] Animation frame found: {}", frame_name);
                tmp.frame_paths.push_back(frame_name);
            }
        }

        animations.push_back(tmp);
        animationIDtoName[tmp.shortName] = static_cast<int>(animations.size()-1);
    }

    // Step 2.5 - Checksum calculation. Invalidate cache if checksum is different or doesn't exist
    for(auto& a : animations)
    {
        std::vector<std::string> full_paths;

        unsigned int new_checksum = 0;
        unsigned int old_checksum = 0;

        new_checksum = Func::calculateTotalChecksum(a.frame_paths, zf);

        std::string cs_data_path = std::format("resources/graphics/.tex_cache/{}@{}@{}.hash", CoreManager::getInstance().getConfig()->GetInt("textureQuality"), model_name, a.shortName);

        std::ifstream cs_file(cs_data_path);
        if(cs_file.good())
        {
            std::string buffer;
            std::getline(cs_file, buffer);
            old_checksum = stoul(buffer);
        }
        cs_file.close();

        if(new_checksum != old_checksum)
        {
            a.cached = false;
            std::ofstream cs_file_out(cs_data_path);
            cs_file_out << std::to_string(new_checksum);
            cs_file_out.close();
        }
    }

    // Step 3 - Compose the spritesheets
    SPDLOG_DEBUG("Maximum texture size: {}", maxSize);

    std::vector<std::future<void>> futures;

    for(auto& x : animations)
    {
        if(!x.cached) // skip spritesheet generation if animation cache found
        {
            // Launch async task for each spritesheet generation
            futures.push_back(std::async(std::launch::async, [this, &x, &path, &zf]() {
                generateSpritesheet(x, path, zf);
            }));
        }
    }

    for(auto& fu : futures)
    {
        fu.get();
    }

    //Step 4 - Load spritesheets to ResourceManager
    SPDLOG_DEBUG("Loading spritesheets...");
    for (Animation &x : animations)
    {
        for (const auto& spr : x.spritesheet_paths)
        {
            ResourceManager::getInstance().loadSprite(spr, false);
        }
    }

    //Step 5 - Fetch animation.json and read parameters
    json animation;

    if(zip)
    {
        ZipEntry entry = zf.getEntry("animation.json");

        if(!entry.isNull())
        {
            animation = json::parse(entry.readAsText());

            SPDLOG_DEBUG("Reading animation.json.");
        }
    }
    else
    {
        std::ifstream anim(model_name+"/animation.json");
        if(anim.good())
        {
            anim >> animation;
        }
        else
        {
            SPDLOG_ERROR("No animation.json file found. Will use defaults which may be incorrect.");
        }
    }

    // animation.json is either loaded or not loaded here, so before we continue we can load the defaults
    // Step 6: Defaults + JSON config
    for(auto& a : animations)
    {
        a.origin_x = a.img_x / 2;
        a.origin_y = a.img_y / 2;
    }

    animationSpeed = 30;

    if(animation.contains("main"))
    {
        if(animation["main"].contains("framerate"))
        {
            animationSpeed = animation["main"]["framerate"];
        }
    }

    for (auto& s : animation["switchTo"])
    {
        std::string from = s[0].get<std::string>();
        std::string to = s[1].get<std::string>();

        SPDLOG_TRACE("switchTo {} {}", from, to);
        std::pair<int, int> st;
        st.first = getIDfromShortName(from);
        st.second = getIDfromShortName(to);
        animationSwitchTo.push_back(st);
    }

    for (auto& s : animation["center"].items())
    {
        std::string key = s.key();

        if(animation["center"][key].size() == 2)
        {
            int o_x = animation["center"][key][0].get<int>();
            int o_y = animation["center"][key][1].get<int>();

            SPDLOG_TRACE("set origin for {} {}", o_x, o_y);
            int id = getIDfromShortName(key);

            SPDLOG_TRACE("setting custom origin for {}: {} {}", key, o_x, o_y);
            animations[id].origin_x = static_cast<float>(o_x);
            animations[id].origin_y = static_cast<float>(o_y);

            animations[id].customOrigin = true;
        }
    }

    if(animation.contains("hitbox"))
    {
        SPDLOG_DEBUG("Loading hitboxes");
        for (auto& s : animation["hitbox"].items())
        {
            std::string key = s.key();
            SPDLOG_TRACE("Hitbox key: {}", key);

            if (animation["hitbox"][key].is_array() && !animation["hitbox"][key].empty() && animation["hitbox"][key].at(0).is_number()) {
                SPDLOG_TRACE("Single hitbox detected");

                int hb_x = animation["hitbox"][key][0].get<int>();
                int hb_y = animation["hitbox"][key][1].get<int>();
                int hb_width = animation["hitbox"][key][2].get<int>();
                int hb_height = animation["hitbox"][key][3].get<int>();

                sf::FloatRect h;

                h.position.x = static_cast<float>(hb_x);
                h.position.y = static_cast<float>(hb_y);
                h.size.x = static_cast<float>(hb_width);
                h.size.y = static_cast<float>(hb_height);

                if(key == "default")
                {
                    SPDLOG_TRACE("Setting default hitbox: {} {} {} {}", h.position.x, h.position.y, h.size.x, h.size.y);

                    for(auto& a : animations)
                    {
                        a.hitboxes.push_back(h);
                    }
                }
                else
                {
                    SPDLOG_TRACE("Setting animation-specific hitbox: {} {} {} {}", h.position.x, h.position.y, h.size.x, h.size.y);

                    int id = getIDfromShortName(key);
                    animations[id].hitboxes.push_back(h);
                }
            }
            else if (animation["hitbox"][key].is_array() && !animation["hitbox"][key].empty() && animation["hitbox"][key].at(0).is_array()) {
                SPDLOG_TRACE("Multi hitbox detected");
                for (auto& json_hb : animation["hitbox"][key])
                {
                    SPDLOG_TRACE("JSON: {}", json_hb.dump());
                    if(json_hb.size() == 4)
                    {
                        int hb_x = json_hb[0].get<int>();
                        int hb_y = json_hb[1].get<int>();
                        int hb_width = json_hb[2].get<int>();
                        int hb_height = json_hb[3].get<int>();

                        sf::FloatRect h;

                        h.position.x = static_cast<float>(hb_x);
                        h.position.y = static_cast<float>(hb_y);
                        h.size.x = static_cast<float>(hb_width);
                        h.size.y = static_cast<float>(hb_height);

                        if(key == "default")
                        {
                            SPDLOG_TRACE("Setting default hitbox: {} {} {} {}", h.position.x, h.position.y, h.size.x, h.size.y);

                            for(auto& a : animations)
                            {
                                a.hitboxes.push_back(h);
                            }
                        }
                        else
                        {
                            SPDLOG_TRACE("Setting animation-specific hitbox: {} {} {} {}", h.position.x, h.position.y, h.size.x, h.size.y);

                            int id = getIDfromShortName(key);
                            animations[id].hitboxes.push_back(h);
                        }
                    }
                }
            }
        }
    }
    else
    {
        // default hitbox
        for(auto& a : animations)
        {
            float ox = static_cast<float>(a.origin_x);
            float oy = static_cast<float>(a.origin_y);
            float bw = static_cast<float>(a.img_x);
            float bh = static_cast<float>(a.img_y);

            sf::FloatRect h;

            h.position.x = -ox;
            h.position.y = -oy;
            h.size.x = bw;
            h.size.y = bh;

            a.hitboxes.push_back(h);
        }
    }

    for (auto& s : animation["noRepeat"])
    {
        std::string anim = s;

        SPDLOG_TRACE("noRepeat {}", anim);
        animationPause.push_back(getIDfromShortName(anim));
    }

    // for extra, separately animated parts
    if(animation.contains("extras"))
    {
        for (auto& e : animation["extras"].items())
        {
            std::string name = e.value();

            SpriteWrapper spr;
            std::pair<std::string, SpriteWrapper> entry;
            entry.first = name;
            entry.second = spr;
            extra.push_back(entry);

            SPDLOG_TRACE("Loading extra: {}", name);

            json j_extra;
            std::string json_name = "extra_" + name + ".json";

            if(zip)
            {
                ZipArchive zzf(f.string());
                zzf.open(ZipArchive::ReadOnly);
                ZipEntry aentry = zzf.getEntry(json_name);
                if(!aentry.isNull())
                {
                    j_extra = json::parse(aentry.readAsText());

                    SPDLOG_TRACE("Reading animation.json.");
                }
            }
            else
            {
                std::ifstream ex(model_name+"/"+json_name);
                if(ex.good())
                {
                    ex >> j_extra;
                }
                else
                {
                    SPDLOG_ERROR("No animation.json file found. Will use defaults which may be incorrect.");
                }
            }

            for (auto& ff : j_extra["animationData"])
            {
                ExtraFrame ef;
                SPDLOG_TRACE("Parsing extra frame: {}", ff.dump());

                ef.x = ff["pos_x"].get<float>() * 3;
                ef.y = ff["pos_y"].get<float>() * 3;
                ef.r = ff["rotation"].get<float>();

                int an = getIDfromShortName(ff["animation"]);
                int fr = ff["frame"].get<int>();

                ex_frames[name][an][fr] = ef;
            }
        }
    }

    zf.close();
}

int PNGAnimation::getIDfromShortName(const std::string& shortName)
{
    if(animationIDtoName.find(shortName) != animationIDtoName.end())
    {
        return animationIDtoName.at(shortName);
    }
    else
    {
        SPDLOG_ERROR("Invalid animation name found! {}", shortName);
        throw std::exception(); // TODO: replace with a proper exception.
    }
}

std::string PNGAnimation::getShortNameFromID(int ID)
{
    for(auto& x : animationIDtoName)
    {
        if(x.second == ID)
            return x.first;
    }

    SPDLOG_ERROR("Invalid animation ID found!");
    throw std::exception(); // TODO: replace with a proper exception.
}

void PNGAnimation::Draw()
{
    Animation& curAnim = animations[currentAnimation];

    if(isPlaying)
        currentFrame += animationSpeed / CoreManager::getInstance().getCore()->getFPS();

    isLooping = true;

    for(unsigned int a : animationPause)
    {
        if(a == currentAnimation)
            isLooping = false;
    }

    if(currentFrame >= curAnim.frames)
    {
        for (auto& st : animationSwitchTo)
        {
            if(st.first == currentAnimation)
            {
                currentAnimation = st.second;
                currentFrame = 0;
            }
        }

        if(isLooping)
        {
            currentFrame = 0;
        }
        else
        {
            currentFrame = static_cast<float>(curAnim.frames - 1);
        }
    }

    auto currentFrameInt = static_cast<unsigned int>(floor(currentFrame));
    unsigned int maxFramesPerSheet = curAnim.maxCols * curAnim.maxRows;

    int currentSpritesheet = static_cast<int>(floor(currentFrameInt / maxFramesPerSheet));
    int currentSpritesheetFrame = currentFrameInt % maxFramesPerSheet;

    int currentRow = currentSpritesheetFrame / curAnim.maxCols;
    int currentCol = currentSpritesheetFrame % curAnim.maxCols;

    SPDLOG_TRACE("currentAnimation {} {}, currentFrame {}, currentFrameInt {}, maxFrames {}, animationSpeed {}, maxFramesPerSheet {}, currentSpritesheet {}, currentSpritesheetFrame {}, row {}, col {}", currentAnimation, curAnim.shortName, currentFrame, currentFrameInt, curAnim.frames, animationSpeed, maxFramesPerSheet, currentSpritesheet, currentSpritesheetFrame, currentRow, currentCol);

    int x_start = static_cast<int>(currentCol * curAnim.img_x);
    int y_start = static_cast<int>(currentRow * curAnim.img_y);

    sf::IntRect textureRect = sf::IntRect(sf::Vector2i(x_start, y_start), sf::Vector2i(static_cast<int>(curAnim.img_x), static_cast<int>(curAnim.img_y)));
    SPDLOG_TRACE("setting texture rect to {} {} {} {}", textureRect.position.x, textureRect.position.y, textureRect.size.x, textureRect.size.y);

    auto& texture = ResourceManager::getInstance().getSprite(curAnim.spritesheet_paths[currentSpritesheet]);
    texture.setTextureRect(textureRect);

    //sf::RectangleShape rect(sf::Vector2f(curAnim.img_x, curAnim.img_y));
    //rect.setFillColor(sf::Color(255,0,0,100));
    //rect.setPosition(x_start, y_start);

    int qualitySetting = CoreManager::getInstance().getConfig()->GetInt("textureQuality");

    if (qualitySetting != q && qualitySetting != -1)
    {
        SPDLOG_DEBUG("Quality has changed.");

        switch (qualitySetting)
        {
            case 0: {
                qscale = 6;
                break;
            }
            case 1: {
                qscale = 3;
                break;
            }
            case 2: {
                qscale = 2;
                break;
            }
        }

        q = qualitySetting;
    }

    if(curAnim.customOrigin)
        texture.setOrigin(curAnim.origin_x / qscale, curAnim.origin_y / qscale);
    else
        texture.setOrigin(curAnim.origin_x, curAnim.origin_y);
    texture.setPosition(position.x, position.y);
    texture.setScale(scale.x, scale.y);
    texture.setRotation(rotation);
    texture.setColor(color);

    //draw extras
    for(auto& e : extra)
    {
        auto& name = e.first;
        auto& spr = e.second;

        auto& frame = ex_frames[name][currentAnimation][static_cast<unsigned int>(currentFrame)];

        //SPDLOG_INFO("Drawing extra {}, anim {} frame {}, {} {} {}", name, currentAnimation, currentFrame, frame.x, frame.y, frame.r);

        spr.setPosition(position.x + frame.x, position.y + frame.y);
        spr.setScale(scale.x, scale.y);
        spr.setRotation(frame.r);
        spr.draw();
    }

    texture.draw();
}

void PNGAnimation::drawCopy(sf::Vector2f pos, sf::Vector2f sc)
{
    Animation& curAnim = animations[currentAnimation];

    auto currentFrameInt = static_cast<unsigned int>(floor(currentFrame));
    unsigned int maxFramesPerSheet = curAnim.maxCols * curAnim.maxRows;

    int currentSpritesheet = static_cast<int>(floor(currentFrameInt / maxFramesPerSheet));
    int currentSpritesheetFrame = currentFrameInt % maxFramesPerSheet;

    int currentRow = currentSpritesheetFrame / curAnim.maxCols;
    int currentCol = currentSpritesheetFrame % curAnim.maxCols;

    SPDLOG_TRACE("currentAnimation {} {}, currentFrame {}, currentFrameInt {}, maxFrames {}, animationSpeed {}, maxFramesPerSheet {}, currentSpritesheet {}, currentSpritesheetFrame {}, row {}, col {}", currentAnimation, curAnim.shortName, currentFrame, currentFrameInt, curAnim.frames, animationSpeed, maxFramesPerSheet, currentSpritesheet, currentSpritesheetFrame, currentRow, currentCol);

    int x_start = static_cast<int>(currentCol * curAnim.img_x);
    int y_start = static_cast<int>(currentRow * curAnim.img_y);

    sf::IntRect textureRect = sf::IntRect(sf::Vector2i(x_start, y_start), sf::Vector2i(static_cast<int>(curAnim.img_x), static_cast<int>(curAnim.img_y)));

    auto& texture = ResourceManager::getInstance().getSprite(curAnim.spritesheet_paths[currentSpritesheet]);
    texture.setTextureRect(textureRect);

    if(curAnim.customOrigin)
        texture.setOrigin(curAnim.origin_x / qscale, curAnim.origin_y / qscale);
    else
        texture.setOrigin(curAnim.origin_x, curAnim.origin_y);
    texture.setPosition(pos.x, pos.y);
    texture.setScale(sc.x, sc.y);
    texture.setRotation(rotation);
    texture.setColor(color);

    //draw extras
    for(auto& e : extra)
    {
        auto& name = e.first;
        auto& spr = e.second;

        auto& frame = ex_frames[name][currentAnimation][static_cast<unsigned int>(currentFrame)];

        //SPDLOG_INFO("Drawing extra {}, anim {} frame {}, {} {} {}", name, currentAnimation, currentFrame, frame.x, frame.y, frame.r);

        spr.setScale(sc.x, sc.y);
        spr.setPosition(pos.x + frame.x*sc.x, pos.y + frame.y*sc.y);
        spr.setRotation(frame.r);
        spr.draw();
        spr.setScale(1, 1);
    }

    texture.draw();
}

void PNGAnimation::setAnimation(const std::string& animShortName)
{
    currentAnimation = getIDfromShortName(animShortName);
}

std::string PNGAnimation::getAnimation()
{
    return getShortNameFromID(currentAnimation);
}

void PNGAnimation::addAnimationStalling(const std::string& animShortName, int frame, int duration)
{
}