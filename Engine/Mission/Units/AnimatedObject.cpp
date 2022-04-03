#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "AnimatedObject.h"
#include "../../Binary.hpp"
#include "../../Func.h"
#include "../../Item/Item.h"
#include "../../P4A.h"
#include "../../V4Core.h"
#include "Object.h"
#include "math.h"
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <spdlog/spdlog.h>

AnimatedObject::AnimatedObject()
{
}

AnimatedObject::~AnimatedObject()
{
    SPDLOG_TRACE("AnimatedObject::~AnimatedObject(): destructor");
}

void AnimatedObject::loadAnim(std::string data, P4A handle)
{
    ///LOADANIM ver from 9.01.2020
    //cout << "[AnimatedObject] Loading animation! Data size " << data.size() << endl;

    bool cache_loaded = false;

    if (thisConfig->thisCore->isCached[entityID])
    {
        ///Load cache here
        all_swaps_img = thisConfig->thisCore->animation_cache[entityID]->swaps;
        animation_spritesheet = thisConfig->thisCore->animation_cache[entityID]->spritesheet;
        //objects = thisConfig->thisCore->currentController.animation_cache[entityID]->objects;

        cache_loaded = true;
        //cout << "[AnimatedObject] Cache loaded" << endl;
    }

    bool legit = false;
    string version = "";

    istringstream iss(data);
    string line;
    while (getline(iss, line))
    {
        ////cout << "[AnimatedObject] Line: " << line << endl;

        if (line.find("V4Mater-ver-") != std::string::npos)
        {
            version = line.substr(line.find("V4Mater-ver-") + 12);
            legit = true;

            SPDLOG_DEBUG("[AnimatedObject] Anim format legit. Version {}", version);
        }

        /**
        [READ] V4Mater-ver-1.00
        [READ] S:9.799995
        [READ] OI:button_idle.png
        [READ] F:animation_name,0.000000,0,106,502,0,24,24,1,1
        [READ] F:animation_name,8.003246,0,1055,146,180.875,24,24,1,1
        */

        if (legit)
        {
            if (version == "1.03-experimental")
            {
                ao_version = 3;

                if (line.find("S:") != std::string::npos)
                {
                    max_time = atof(line.substr(line.find_first_of(":") + 1).c_str());
                    //cout << "[AnimatedObject] Time: " << max_time << endl;
                }

                if (line.find("FR:") != std::string::npos)
                {
                    animation_framerate = atof(line.substr(line.find_first_of(":") + 1).c_str());
                    //cout << "[AnimatedObject] Animation framerate: " << animation_framerate << endl;
                }

                if (line.find("XB:") != std::string::npos)
                {
                    xBound = atoi(line.substr(line.find_first_of(":") + 1).c_str());
                    //cout << "[AnimatedObject] Animation X bound: " << xBound << endl;
                }

                if (line.find("YB:") != std::string::npos)
                {
                    yBound = atoi(line.substr(line.find_first_of(":") + 1).c_str());
                    //cout << "[AnimatedObject] Animation Y bound: " << yBound << endl;
                }

                if (line.find("AFB:") != std::string::npos)
                {
                    string animdata = line.substr(line.find_first_of(":") + 1);
                    vector<string> anim = Func::Split(animdata, ',');

                    int animID = atoi(anim[0].c_str());
                    int animFrame = atoi(anim[1].c_str());
                    int x1 = atoi(anim[2].c_str());
                    int y1 = atoi(anim[3].c_str());
                    int x2 = atoi(anim[4].c_str());
                    int y2 = atoi(anim[5].c_str());
                    int orx = 0;
                    int ory = 0;

                    if (anim.size() > 6)
                    {
                        orx = atoi(anim[6].c_str());
                        ory = atoi(anim[7].c_str());
                    }

                    sf::IntRect rect(x1, y1, x2, y2);
                    sf::Vector2f origin(orx, ory);
                    animation_bounds[animID][animFrame] = rect;
                    animation_origins[animID][animFrame] = origin;

                    //cout << "Creating AnimationFrameBound... " << endl;

                    AnimationFrameBound tmp;
                    if (animFrame == 0) ///load only the first one
                    {
                        tmp.image.create(rect.width, rect.height, sf::Color::Transparent);
                        tmp.image.copy((*animation_spritesheet)[animID].spritesheet, 0, 0, rect);
                    }
                    tmp.rect = rect;
                    tmp.origin = origin;

                    afb[animID][animFrame] = tmp;

                    //cout << "created." << endl;

                    //cout << "Animation " << animation_names[animID] << " frame " << animFrame << " bounds: " << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
                }

                if (line.find("OI:") != std::string::npos)
                {
                    //auto start = std::chrono::high_resolution_clock::now();

                    string tex_file = line.substr(line.find_first_of(":") + 1);

                    int parent = 0; ///unused yet

                    objects.get()->emplace_back();
                    (*objects)[objects.get()->size() - 1].object_name = tex_file;

                    //auto elapsed = std::chrono::high_resolution_clock::now() - start;
                    //cout << "Pushed to vector, took " << std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count() << endl;

                    //cout << "Added new object from " << tex_file << endl;
                }

                if ((line.find("F:") != std::string::npos) && (line[0] == 'F'))
                {
                    string framedata = line.substr(line.find_first_of(":") + 1);
                    vector<string> frame = Func::Split(framedata, ',');

                    float time = atof(frame[0].c_str());
                    int objectID = atoi(frame[1].c_str());

                    float pos_x = atof(frame[2].c_str());
                    float pos_y = atof(frame[3].c_str());
                    float rotation = atof(frame[4].c_str());
                    float or_x = atof(frame[5].c_str());
                    float or_y = atof(frame[6].c_str());
                    float scale_x = atof(frame[7].c_str());
                    float scale_y = atof(frame[8].c_str());

                    if (objectID == 0)
                        (*objects)[objectID].SetCustomFrame(time, pos_x, pos_y, or_x, or_y, rotation, scale_x, scale_y);
                    else
                        (*objects)[objectID].SetCustomFrame(time, pos_x, pos_y, 0, 0, rotation, scale_x, scale_y);

                    //(*objects)[objectID*2+1].SetCustomFrame(time,pos_x,pos_y,1,1,rotation,scale_x,scale_y);
                }

                if (!cache_loaded)
                {
                    if (line.find("A:") != std::string::npos)
                    {
                        string animdata = line.substr(line.find_first_of(":") + 1);
                        vector<string> anim = Func::Split(animdata, ',');

                        /**Animation tmp;

                        tmp.a_begin = atof(anim[0].c_str());
                        tmp.a_end = atof(anim[1].c_str());
                        tmp.a_name = anim[2];

                        animations.push_back(tmp);*/
                        animation_begin.push_back(atof(anim[0].c_str()));
                        animation_end.push_back(atof(anim[1].c_str()));
                        animation_names.push_back(anim[2]);
                        animation_index[anim[2]] = animation_names.size() - 1;
                        float alength = atof(anim[1].c_str()) - atof(anim[0].c_str());
                        animation_length.push_back(alength);

                        //cout << "[AnimatedObject] Animation length: " << alength << endl;

                        ///Go-to anim implementation
                        if (anim.size() >= 4)
                        {
                            animation_goto.push_back(anim[3]);
                            animation_loop.push_back(atoi(anim[4].c_str()));
                        } else
                        {
                            animation_goto.push_back("");
                            animation_loop.push_back(1);
                        }

                        ///spritesheet implementation

                        //cout << "Loading the spritesheet" << endl;

                        /// New AnimationObject loading scheme (30.05.2021) by Owocek
                        /// Have all the models (spritesheets and pixelswaps) preloaded within some easily accessible class like MissionController.
                        /// Instead of loading the same models over and over again, grab the objects and spritesheets from MissionController
                        /// Best way to use some pointer if possible, because it absolutely minimizes both RAM and CPU usage, and makes mission loading faster
                        /// So instead of loading huge kirajin animations 5 times, I load it once in MissionController and take it from there.

                        //cout << "[AnimatedObject] Checking for cache: entityID = " << entityID << endl;

                        //cout << "[AnimatedObject] result: " << !thisConfig->thisCore->currentController.isCached[entityID] << endl;

                        //if (!thisConfig->thisCore->isCached[entityID])
                        if (true)
                        {
                            Animation tmp;
                            sf::Texture spr;
                            //cout << "Loading " << anim[2] << ".png" << endl;
                            vector<unsigned char> h = handle.ReadToMemoryChar(anim[2] + ".png");
                            tmp.spritesheet.loadFromMemory(&h[0], h.size());
                            //tmp.spritesheet.loadFromMemory(&mem[0], mem.size());
                            tmp.name = anim[2];
                            animation_spritesheet.get()->push_back(tmp);

                            //cout << "Loaded spritesheet " << anim[2] << endl;

                            //cout << "Loading the PixelSwap file" << endl;

                            vector<unsigned char> pxswap = handle.ReadToMemoryChar(anim[2] + ".px");
                            int offset = 0;
                            int checks = 0;

                            int length = pxswap.size();

                            //cout << "[PixelSwap] Length: " << length << " bytes" << endl;

                            if (length > 0)
                            {
                                vector<Object::Pixel> one_swap;            ///one frame swap
                                vector<vector<Object::Pixel>> animation_swaps; ///one full animation

                                //cout << "[PixelSwap] Reading file..." << endl;

                                while (offset < length)
                                {
                                    uint16_t check = Binary::get_uint16(pxswap, offset);

                                    //cout << "offset: " << std::hex << offset << std::dec << endl;

                                    //if((offset >= 130900) && (offset <= 131000))
                                    //cout << "offset " << offset << ": " << std::hex << check << std::dec << endl;

                                    if (int(check) == 52445)
                                    {
                                        //cout << "Check found @ 0x" << std::hex << offset << std::dec << endl;

                                        if (checks >= 1)
                                        {
                                            animation_swaps.push_back(one_swap);
                                            one_swap.clear();

                                            //cout << "Swap " << animation_swaps.size()-1 << " saved. Offset: 0x" << std::hex << offset << std::dec << endl;
                                        }

                                        checks++;

                                        offset += 8;

                                        //cout << "Check " << checks << " has been executed (" << animation_swaps.size() << ")" << endl;
                                    } else
                                    {
                                        uint16_t x = Binary::get_uint16(pxswap, offset);
                                        uint16_t y = Binary::get_uint16(pxswap, offset + 2);

                                        uint32_t c = Binary::get_uint32_r(pxswap, offset + 4);

                                        Object::Pixel px;
                                        px.x = x;
                                        px.y = y;
                                        px.color = sf::Color(c);

                                        one_swap.push_back(px);

                                        //cout << "Added a new swap " << int(x) << " " << int(y) << " " << int(r) << " " << int(g) << " " << int(b) << " " << int(a) << " (" << one_swap.size() << ")" << endl;

                                        offset += 8;
                                    }
                                }

                                animation_swaps.push_back(one_swap);
                                //cout << "Swap " << animation_swaps.size()-1 << " saved. Offset: 0x" << std::hex << offset << std::dec << endl;

                                /// New code potentially dangerous
                                // Idea: Create all swaps in loading function, and store finished images
                                // Downside: more RAM usage | Upside: less CPU usage
                                // all_swaps animIndex frame
                                // all_swaps[index][curFrame-1]

                                vector<sf::Image> frames;
                                int i_frames = 0;

                                for (int a = 0; a < animation_swaps.size(); a++)
                                {
                                    std::string img_key = anim_path + "_" + anim[2] + "_" + to_string(i_frames);
                                    SPDLOG_TRACE("IMG_KEY: {}", img_key);

                                    i_frames++;

                                    if (!TextureManager::getInstance().checkImageExists(img_key))
                                    {
                                        sf::Image nw = tmp.spritesheet;
                                        for (int i = 0; i < animation_swaps[a].size(); i++)
                                        {
                                            nw.setPixel(animation_swaps[a][i].x, animation_swaps[a][i].y, animation_swaps[a][i].color);
                                        }

                                        TextureManager::getInstance().loadImageFromMemory(img_key, nw);
                                    }
                                    //cout << "frame " << frames.size() - 1 << " for animation " << a << " created" << endl;
                                }

                                animation_length_frames.push_back(i_frames);

                                //all_swaps.push_back(animation_swaps);

                                //all_swaps_img.get()->push_back(frames);
                            }

                            //cout << "[AnimatedObject] PixelSwap animation loaded: " << animation_swaps.size() << " swaps (animation no. " << all_swaps_img.get()->size() << ")" << endl;
                        }
                    }
                } else
                {
                    if (line.find("A:") != std::string::npos)
                    {
                        string animdata = line.substr(line.find_first_of(":") + 1);
                        vector<string> anim = Func::Split(animdata, ',');

                        /**Animation tmp;

                        tmp.a_begin = atof(anim[0].c_str());
                        tmp.a_end = atof(anim[1].c_str());
                        tmp.a_name = anim[2];

                        animations.push_back(tmp);*/
                        animation_begin.push_back(atof(anim[0].c_str()));
                        animation_end.push_back(atof(anim[1].c_str()));
                        animation_names.push_back(anim[2]);
                        animation_index[anim[2]] = animation_names.size() - 1;
                        float alength = atof(anim[1].c_str()) - atof(anim[0].c_str());
                        animation_length.push_back(alength);

                        //cout << "[AnimatedObject] Animation length: " << alength << endl;

                        ///Go-to anim implementation
                        if (anim.size() >= 4)
                        {
                            animation_goto.push_back(anim[3]);
                            animation_loop.push_back(atoi(anim[4].c_str()));
                        } else
                        {
                            animation_goto.push_back("");
                            animation_loop.push_back(1);
                        }
                    }
                }

                if (line.find("HB:") != std::string::npos)
                {
                    string hitboxdata = line.substr(line.find_first_of(":") + 1);
                    vector<string> hitbox = Func::Split(hitboxdata, ',');

                    if (hitbox[0] == "rectangle")
                    {
                        Hitbox tmp;
                        hitboxes.push_back(tmp);

                        //cout << "[AnimatedObject] Added new hitbox, type " << hitbox[0] << endl;
                    }
                }

                if (line.find("HBF:") != std::string::npos)
                {
                    string hbframedata = line.substr(line.find_first_of(":") + 1);
                    vector<string> hbframe = Func::Split(hbframedata, ',');

                    float time = atof(hbframe[0].c_str());
                    int hitboxID = atoi(hbframe[1].c_str());

                    float g_x = atof(hbframe[2].c_str());
                    float g_y = atof(hbframe[3].c_str());

                    float width = atof(hbframe[6].c_str());
                    float height = atof(hbframe[7].c_str());
                    float x = atof(hbframe[4].c_str()) + width / 2;
                    float y = atof(hbframe[5].c_str()) + height / 2;
                    float rotation = 0; ///3.14159265358/6; /// this needs to be loaded later IN RADIANS
                    hitboxes[hitboxID].SetCustomFrame(time, g_x, g_y, x, y, rotation, width, height);

                    hitboxes[hitboxID].o_x = atof(hbframe[4].c_str());
                    hitboxes[hitboxID].o_y = atof(hbframe[5].c_str());
                    hitboxes[hitboxID].o_width = width;
                    hitboxes[hitboxID].o_height = height;
                }
            } else if (version == "1.02-experimental")
            {
                if (line.find("S:") != std::string::npos)
                {
                    max_time = atof(line.substr(line.find_first_of(":") + 1).c_str());
                    cout << "Time: " << max_time << endl;
                }

                if (line.find("FR:") != std::string::npos)
                {
                    animation_framerate = atof(line.substr(line.find_first_of(":") + 1).c_str());
                    cout << "Animation framerate: " << animation_framerate << endl;
                }

                if (line.find("XB:") != std::string::npos)
                {
                    xBound = atoi(line.substr(line.find_first_of(":") + 1).c_str());
                    cout << "Animation X bound: " << xBound << endl;
                }

                if (line.find("YB:") != std::string::npos)
                {
                    yBound = atoi(line.substr(line.find_first_of(":") + 1).c_str());
                    cout << "Animation Y bound: " << yBound << endl;
                }

                if (line.find("AFB:") != std::string::npos)
                {
                    string animdata = line.substr(line.find_first_of(":") + 1);
                    vector<string> anim = Func::Split(animdata, ',');

                    int animID = atoi(anim[0].c_str());
                    int animFrame = atoi(anim[1].c_str());
                    int x1 = atoi(anim[2].c_str());
                    int y1 = atoi(anim[3].c_str());
                    int x2 = atoi(anim[4].c_str());
                    int y2 = atoi(anim[5].c_str());
                    int orx = 0;
                    int ory = 0;

                    if (anim.size() > 6)
                    {
                        orx = atoi(anim[6].c_str());
                        ory = atoi(anim[7].c_str());
                    }

                    sf::IntRect rect(x1, y1, x2, y2);
                    sf::Vector2f origin(orx, ory);
                    animation_bounds[animID][animFrame] = rect;
                    animation_origins[animID][animFrame] = origin;

                    //cout << "Creating AnimationFrameBound... " << endl;

                    AnimationFrameBound tmp;
                    tmp.image.create(rect.width, rect.height, sf::Color::Transparent);
                    tmp.image.copy((*animation_spritesheet)[animID].spritesheet, 0, 0, rect);
                    tmp.rect = rect;
                    tmp.origin = origin;

                    afb[animID][animFrame] = tmp;

                    //cout << "created." << endl;

                    //cout << "Animation " << animation_names[animID] << " frame " << animFrame << " bounds: " << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
                }

                if (line.find("A:") != std::string::npos)
                {
                    string animdata = line.substr(line.find_first_of(":") + 1);
                    vector<string> anim = Func::Split(animdata, ',');

                    /**Animation tmp;

                    tmp.a_begin = atof(anim[0].c_str());
                    tmp.a_end = atof(anim[1].c_str());
                    tmp.a_name = anim[2];

                    animations.push_back(tmp);*/
                    animation_begin.push_back(atof(anim[0].c_str()));
                    animation_end.push_back(atof(anim[1].c_str()));
                    animation_names.push_back(anim[2]);
                    animation_index[anim[2]] = animation_names.size() - 1;

                    ///Go-to anim implementation
                    if (anim.size() >= 4)
                    {
                        animation_goto.push_back(anim[3]);
                        animation_loop.push_back(atoi(anim[4].c_str()));
                    } else
                    {
                        animation_goto.push_back("");
                        animation_loop.push_back(1);
                    }

                    ///spritesheet implementation

                    cout << "Loading the spritesheet" << endl;

                    Animation tmp;
                    sf::Texture spr;
                    //cout << "Loading " << anim[2] << ".png" << endl;
                    vector<unsigned char> h = handle.ReadToMemoryChar(anim[2] + ".png");
                    tmp.spritesheet.loadFromMemory(&h[0], h.size());
                    //tmp.spritesheet.loadFromMemory(&mem[0], mem.size());
                    tmp.name = anim[2];
                    animation_spritesheet.get()->push_back(tmp);

                    cout << "Loaded spritesheet " << anim[2] << endl;
                }

                if (line.find("OI:") != std::string::npos)
                {
                    //auto start = std::chrono::high_resolution_clock::now();

                    string tex_file = line.substr(line.find_first_of(":") + 1);

                    int parent = 0; ///unused yet

                    objects.get()->emplace_back();
                    (*objects)[objects.get()->size() - 1].object_name = tex_file;

                    //auto elapsed = std::chrono::high_resolution_clock::now() - start;
                    //cout << "Pushed to vector, took " << std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count() << endl;

                    //cout << "Added new object from " << tex_file << endl;
                }

                if (line.find("HB:") != std::string::npos)
                {
                    string hitboxdata = line.substr(line.find_first_of(":") + 1);
                    vector<string> hitbox = Func::Split(hitboxdata, ',');

                    if (hitbox[0] == "rectangle")
                    {
                        Hitbox tmp;
                        hitboxes.push_back(tmp);

                        cout << "Added new hitbox, type " << hitbox[0] << endl;
                    }
                }

                if ((line.find("F:") != std::string::npos) && (line[0] == 'F'))
                {
                    string framedata = line.substr(line.find_first_of(":") + 1);
                    vector<string> frame = Func::Split(framedata, ',');

                    float time = atof(frame[0].c_str());
                    int objectID = atoi(frame[1].c_str());

                    float pos_x = atof(frame[2].c_str());
                    float pos_y = atof(frame[3].c_str());
                    float rotation = atof(frame[4].c_str());
                    float or_x = atof(frame[5].c_str());
                    float or_y = atof(frame[6].c_str());
                    float scale_x = atof(frame[7].c_str());
                    float scale_y = atof(frame[8].c_str());
                    (*objects)[objectID].SetCustomFrame(time, pos_x, pos_y, or_x, or_y, rotation, scale_x, scale_y);
                    //(*objects)[objectID*2+1].SetCustomFrame(time,pos_x,pos_y,1,1,rotation,scale_x,scale_y);
                }

                if (line.find("HBF:") != std::string::npos)
                {
                    string hbframedata = line.substr(line.find_first_of(":") + 1);
                    vector<string> hbframe = Func::Split(hbframedata, ',');

                    float time = atof(hbframe[0].c_str());
                    int hitboxID = atoi(hbframe[1].c_str());

                    float g_x = atof(hbframe[2].c_str());
                    float g_y = atof(hbframe[3].c_str());

                    float width = atof(hbframe[6].c_str());
                    float height = atof(hbframe[7].c_str());
                    float x = atof(hbframe[4].c_str()) + width / 2;
                    float y = atof(hbframe[5].c_str()) + height / 2;
                    float rotation = 0; ///3.14159265358/6; /// this needs to be loaded later IN RADIANS
                    hitboxes[hitboxID].SetCustomFrame(time, g_x, g_y, x, y, rotation, width, height);

                    hitboxes[hitboxID].o_x = x;
                    hitboxes[hitboxID].o_y = y;
                    hitboxes[hitboxID].o_width = width;
                    hitboxes[hitboxID].o_height = height;
                }
            } else if (version == "1.01")
            {
                //cout << "[READ " << version << "]: " << line << endl;

                if (line.find("S:") != std::string::npos)
                {
                    max_time = atof(line.substr(line.find_first_of(":") + 1).c_str());
                    cout << "Time: " << max_time << endl;
                }

                if (line.find("A:") != std::string::npos)
                {
                    string animdata = line.substr(line.find_first_of(":") + 1);
                    vector<string> anim = Func::Split(animdata, ',');

                    /**Animation tmp;

                    tmp.a_begin = atof(anim[0].c_str());
                    tmp.a_end = atof(anim[1].c_str());
                    tmp.a_name = anim[2];

                    animations.push_back(tmp);*/
                    animation_begin.push_back(atof(anim[0].c_str()));
                    animation_end.push_back(atof(anim[1].c_str()));
                    animation_names.push_back(anim[2]);
                    animation_index[anim[2]] = animation_names.size() - 1;

                    ///Go-to anim implementation
                    if (anim.size() >= 4)
                        animation_goto.push_back(anim[3]);
                    else
                        animation_goto.push_back("");
                }

                if (line.find("OI:") != std::string::npos)
                {
                    //auto start = std::chrono::high_resolution_clock::now();

                    string tex_file = line.substr(line.find_first_of(":") + 1);

                    int parent = 0; ///unused yet

                    objects.get()->emplace_back(handle.ReadToMemory(tex_file), 0, 0, objects.get()->size(), parent);

                    //auto elapsed = std::chrono::high_resolution_clock::now() - start;
                    //cout << "Pushed to vector, took " << std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count() << endl;

                    //cout << "Added new object from " << tex_file << endl;
                }

                if (line.find("HB:") != std::string::npos)
                {
                    string hitboxdata = line.substr(line.find_first_of(":") + 1);
                    vector<string> hitbox = Func::Split(hitboxdata, ',');

                    if (hitbox[0] == "rectangle")
                    {
                        Hitbox tmp;
                        hitboxes.push_back(tmp);

                        cout << "Added new hitbox, type " << hitbox[0] << endl;
                    }
                }

                if ((line.find("F:") != std::string::npos) && (line[0] == 'F'))
                {
                    string framedata = line.substr(line.find_first_of(":") + 1);
                    vector<string> frame = Func::Split(framedata, ',');

                    float time = atof(frame[0].c_str());
                    int objectID = atoi(frame[1].c_str());

                    float pos_x = atof(frame[2].c_str());
                    float pos_y = atof(frame[3].c_str());
                    float rotation = atof(frame[4].c_str());
                    float or_x = atof(frame[5].c_str());
                    float or_y = atof(frame[6].c_str());
                    float scale_x = atof(frame[7].c_str());
                    float scale_y = atof(frame[8].c_str());
                    (*objects)[objectID].SetCustomFrame(time, pos_x, pos_y, or_x, or_y, rotation, scale_x, scale_y);
                    //(*objects)[objectID*2+1].SetCustomFrame(time,pos_x,pos_y,1,1,rotation,scale_x,scale_y);
                }

                if (line.find("HBF:") != std::string::npos)
                {
                    string hbframedata = line.substr(line.find_first_of(":") + 1);
                    vector<string> hbframe = Func::Split(hbframedata, ',');

                    float time = atof(hbframe[0].c_str());
                    int hitboxID = atoi(hbframe[1].c_str());

                    float g_x = atof(hbframe[2].c_str());
                    float g_y = atof(hbframe[3].c_str());

                    float width = atof(hbframe[6].c_str());
                    float height = atof(hbframe[7].c_str());
                    float x = atof(hbframe[4].c_str()) + width / 2;
                    float y = atof(hbframe[5].c_str()) + height / 2;
                    float rotation = 0; ///3.14159265358/6; /// this needs to be loaded later IN RADIANS
                    hitboxes[hitboxID].SetCustomFrame(time, g_x, g_y, x, y, rotation, width, height);
                }
            } else if (version == "1.00")
            {
                cout << "[READ " << version << "]: " << line << endl;

                if (line.find("S:") != std::string::npos)
                {
                    max_time = atof(line.substr(line.find_first_of(":") + 1).c_str());
                    cout << "Time: " << max_time << endl;
                }

                if (line.find("OI:") != std::string::npos)
                {
                    string objectdata = line.substr(line.find_first_of(":") + 1);
                    vector<string> object = Func::Split(objectdata, ',');

                    string tex_file = object[0];
                    string texture_data = handle.ReadToMemory(tex_file);

                    //cout << "texture data: " << texture_data.size() << endl;

                    sf::Texture obj_texture;
                    obj_texture.loadFromMemory(texture_data.data(), texture_data.size());
                    obj_texture.setSmooth(true);

                    int parent = 0; ///unused yet

                    Object tmp;
                    tmp.Load(obj_texture, 0, 0);
                    tmp.layer = objects.get()->size();
                    tmp.parent = parent;
                    objects.get()->push_back(tmp);

                    //cout << "Added new object from " << tex_file << endl;
                }

                if (line.find("F:") != std::string::npos)
                {
                    string framedata = line.substr(line.find_first_of(":") + 1);
                    vector<string> frame = Func::Split(framedata, ',');

                    string animation_name = frame[0];

                    float time = atof(frame[1].c_str());
                    int objectID = atoi(frame[2].c_str());

                    float pos_x = atof(frame[3].c_str());
                    float pos_y = atof(frame[4].c_str());
                    float rotation = atof(frame[5].c_str());
                    float or_x = atof(frame[6].c_str());
                    float or_y = atof(frame[7].c_str());
                    float scale_x = atof(frame[8].c_str());
                    float scale_y = atof(frame[9].c_str());
                    (*objects)[objectID].SetCustomFrame(time, pos_x, pos_y, or_x, or_y, rotation, scale_x, scale_y);
                    //(*objects)[objectID*2+1].SetCustomFrame(time,pos_x,pos_y,1,1,rotation,scale_x,scale_y);
                }
            }
        }
    }

    ///init first animation (change later to an option "default animation")
    //setAnimationSegment(animation_names[0]);

    /// find a segment called "idle" and start with it, if theres no such, start with first anim
    bool found = false;

    for (int i = 0; i < animation_names.size(); i++)
    {
        SPDLOG_TRACE("animation_names[{}] = {}", i, animation_names[i]);

        if (animation_names[i] == "idle")
        {
            setAnimationSegment(animation_names[i]);

            found = true;
            break;
        }
    }

    if (!found)
        setAnimationSegment(animation_names[0]);

    SPDLOG_DEBUG("Starting with animation segment {}", getAnimationSegment());

    ///add some randomness to the animations so it looks more natural
    int anim_size = floor((anim_end - anim_begin) * 100000);
    int rand_pos = (rand() << 15 | rand()) % anim_size;

    cur_pos = anim_begin + (float(rand_pos) / 100000);
    SPDLOG_DEBUG("Random cur_pos: anim_size {} rand_pos {} cur_pos {}", anim_size, rand_pos, cur_pos);
    //setPositions(0);
}

int AnimatedObject::getSegmentIndex(std::string segment_name)
{
    /**
	std::vector<string>::iterator it = std::find(animation_names.begin(), animation_names.end(), segment_name);

	if (it != animation_names.end())
	{
		return std::distance(animation_names.begin(), it);
	}
	else
	{
		return -1;
	}
	**/

    return animation_index[segment_name];
}

void AnimatedObject::setAnimationSegment(std::string new_segment_name)
{
    //cout << "AnimatedObject::setAnimationSegment(" << new_segment_name << ");" << endl;
    //cout << new_segment_name << " vs " << getAnimationSegment() << endl;

    sf::Clock c;

    if (new_segment_name != getAnimationSegment())
    {
        //cout << "Changing spritesheet to " << new_segment_name << endl;
        //(*objects)[0].tex_obj.loadFromMemory(&animation_spritesheet[getSegmentIndex(new_segment_name)].spritesheet[0], animation_spritesheet[getSegmentIndex(new_segment_name)].spritesheet.size());
        //(*objects)[0].swapSpritesheet(getSegmentIndex(new_segment_name));
        //cout << "Swapped spritesheet. Took " << c.restart().asMicroseconds() << "microseconds" << endl;
    }

    if (animation_begin.size() > 1)
    {
        anim_begin = animation_begin[distance(animation_names.begin(), find(animation_names.begin(), animation_names.end(), new_segment_name))];
        anim_end = animation_end[distance(animation_names.begin(), find(animation_names.begin(), animation_names.end(), new_segment_name))];

        current_animation = new_segment_name;
    } else if (animation_begin.size() == 1)
    {
        anim_begin = animation_begin[0];
        anim_end = animation_end[0];

        current_animation = new_segment_name;
    } else
    {
        SPDLOG_ERROR("There's an object with 0 animations. Did it load properly? Or maybe you forgot to add them? File {} Animation segment {}", anim_path, new_segment_name);
    }
}

void AnimatedObject::setAnimationSegment(std::string new_segment_name, bool force_start)
{
    //cout << "AnimatedObject::setAnimationSegment(" << new_segment_name << ");" << endl;
    //cout << new_segment_name << " vs " << getAnimationSegment() << endl;

    sf::Clock c;

    if (new_segment_name != getAnimationSegment())
    {
        //cout << "Changing spritesheet to " << new_segment_name << endl;

        //(*objects)[0].tex_obj.loadFromMemory(&animation_spritesheet[getSegmentIndex(new_segment_name)].spritesheet[0], animation_spritesheet[getSegmentIndex(new_segment_name)].spritesheet.size());
        //(*objects)[0].swapSpritesheet(getSegmentIndex(new_segment_name));
        //cout << "Swapped spritesheet. Took " << c.restart().asMicroseconds() << "microseconds" << endl;
    }

    if (animation_begin.size() > 1)
    {
        anim_begin = animation_begin[distance(animation_names.begin(), find(animation_names.begin(), animation_names.end(), new_segment_name))];
        anim_end = animation_end[distance(animation_names.begin(), find(animation_names.begin(), animation_names.end(), new_segment_name))];

        current_animation = new_segment_name;
    } else
    {
        anim_begin = animation_begin[0];
        anim_end = animation_end[0];

        current_animation = new_segment_name;
    }

    if (force_start)
        cur_pos = anim_begin;
}

std::string AnimatedObject::getAnimationSegment()
{
    return current_animation;
}

float AnimatedObject::getAnimationLength(std::string segment_name)
{
    //cout << "AnimatedObject::getAnimationLength(" << segment_name << "): " << animation_length[getSegmentIndex(segment_name)] << endl;
    if (animation_length.size() > 0)
    {
        if (animation_begin.size() > 1)
        {
            return animation_length[getSegmentIndex(segment_name)];
        } else
        {
            return animation_length[0];
        }
    } else
    {
        float b, e;

        if (animation_begin.size() > 1)
        {
            b = animation_begin[distance(animation_names.begin(), find(animation_names.begin(), animation_names.end(), segment_name))];
            e = animation_end[distance(animation_names.begin(), find(animation_names.begin(), animation_names.end(), segment_name))];
        } else
        {
            b = animation_begin[0];
            e = animation_end[0];
        }

        return e - b;
    }
}

float AnimatedObject::getAnimationPos()
{
    float e;

    if (animation_begin.size() > 1)
    {
        //e = animation_end[distance(animation_names.begin(), find(animation_names.begin(), animation_names.end(), current_animation))];
        e = animation_end[getSegmentIndex(getAnimationSegment())];
    } else
    {
        e = animation_end[0];
    }

    float r = getAnimationLength(current_animation) - (e - cur_pos);
    if (r < 0)
        r = 0;

    return r;
}

void AnimatedObject::setGlobalPosition(sf::Vector2f pos)
{
    global_x = pos.x;
    global_y = pos.y;
}

sf::Vector2f AnimatedObject::getGlobalPosition()
{
    return sf::Vector2f(global_x + local_x, global_y + local_y);
}

void AnimatedObject::moveGlobalPosition(sf::Vector2f pos)
{
    global_x += pos.x;
    global_y += pos.y;
}

void AnimatedObject::setLoop(bool loop)
{
    loopable = loop;
}

void AnimatedObject::setColor(sf::Color new_color)
{
    color = new_color;
}

sf::Color AnimatedObject::getColor()
{
    return color;
}

void AnimatedObject::applyEquipment(vector<int> item_id, int slot, bool offhand)
{
    std::string str_item_id = std::to_string(item_id[0]);

    for (int i = 1; i < item_id.size(); i++)
    {
        str_item_id += "," + std::to_string(item_id[i]);
    }

    SPDLOG_INFO("Applying equipment with id {}, slot: {}, offhand: {}", str_item_id, slot, offhand);

    int q = stoi(thisConfig->configMap["textureQuality"]);
    Item* equip = thisConfig->thisCore->saveReader.itemReg.getItemByID(item_id);
    string category = equip->item_category;
    string type = equip->item_type;
    string path = "resources/graphics/item/textures/" + equip->spritesheet + "/" + Func::num_padding(equip->spritesheet_id, 4) + ".png";

    /* switch (q)
    {
        case 0: {
            path += "L.png";
            break;
        }
        case 1: {
            path += "M.png";
            break;
        }
        case 2: {
            path += "H.png";
            break;
        }
        case 3: {
            path += "U.png";
            break;
        }
    }*/
    if (offhand)
    {
        q += 4; // If is in offhand, use lines 5-8 instead
    } 

    SPDLOG_INFO("Applied equipment: category: {}, type: {}, path: {}, slot: {}", category, type, path, slot);

    //(*objects)[slot + 1].tex_obj.loadFromFile(path);
    //(*objects)[slot + 1].s_obj.setTexture((*objects)[slot + 1].tex_obj);
    (*objects)[slot + 1].s_obj.setTexture(TextureManager::getInstance().getTexture(path, q));
    (*objects)[slot + 1].s_obj.qualitySetting = q;

    (*objects)[slot + 1].object_name = "eq_" + to_string(slot);

    sf::Vector2f a;

    /// make some check here if .spr file exists, if not, then go defaults

    ifstream file("resources/graphics/item/alignment/" + equip->spritesheet + "/" + Func::num_padding(equip->spritesheet_id, 4) + ".spr");
    string buff;

    float ax, ay;

    for (int i = 0; getline(file, buff); i++)
    {
        if (i == 3)
        {
            ax = atof(buff.substr(0, buff.find_first_of(",")).c_str());
            ay = atof(buff.substr(buff.find_first_of(",") + 1).c_str());
        }
    }

    switch (q)
    {
        case 0: {
            ax = ax / 6.f;
            ay = ay / 6.f;
            break;
        }
        
        case 1: {
            ax = ax / 3.f;
            ay = ay / 3.f;
            break;
        }
        
        case 2: {
            ax = ax / 2.f;
            ay = ay / 2.f;
            break;
        }
    }

    a.x = ax;
    a.y = ay;

    /**if(slots_origins.size() - 1 > slot)
    {
        sf::Vector2f temp;
        for(int i = slots_origins.size(); i <= slot; i++)
        {
            slots_origins.push_back(temp);
        }
    }
    slots_origins[slot] = a;*/

    slots_origins[slot] = a;
}

void AnimatedObject::LoadConfig(Config* thisConfigs, std::string unitParamPath)
{
    all_swaps_img = make_shared<vector<vector<sf::Image>>>();
    animation_spritesheet = make_shared<vector<Animation>>();
    objects = make_shared<vector<Object>>();

    P4A handle;
    anim_path = unitParamPath;

    thisConfig = thisConfigs;

    if (unitParamPath != "")
    {
        handle.ReadDictionary(unitParamPath);
        string animdata = handle.ReadToMemory("data.anim");

        if (animdata == "")
        {
            cout << "Invalid animation file!" << endl;
        }

        loadAnim(animdata, handle);
    }
}

void AnimatedObject::Draw(sf::RenderWindow& window)
{
    //cout << "[AnimatedObject::Draw] DRAW START" << endl;
    if (!manual_mode) ///manual mode is restricted only to unique entities, never intended for use since it disables the whole animation
    {
        cur_pos += framerate / float(fps);

        if (cur_pos < anim_begin)
            cur_pos = anim_begin;

        if (cur_pos > anim_end)
        {
            if (loopable)
            {
                cur_pos -= (anim_end - anim_begin);
            } else
            {
                if (animation_goto[getSegmentIndex(current_animation)] != "")
                {
                    cout << "Animation go to: " << animation_goto[getSegmentIndex(current_animation)] << " SegmentIndex: " << getSegmentIndex(current_animation) << " current animation: " << current_animation << endl;
                    setAnimationSegment(animation_goto[getSegmentIndex(current_animation)], true);
                } else
                {
                    cur_pos = anim_end;
                }
            }
        }

        setLoop(animation_loop[getSegmentIndex(current_animation)]);
    }

    ///calculate current position on the spritesheet
    //cout << "[AnimatedObject::Draw] Get current frame position" << endl;
    curFrame = floor(getAnimationPos() * animation_framerate);
    index = getSegmentIndex(getAnimationSegment());
    //cout << "entity ID " << entityID << " curFrame = " << getAnimationPos() << " " << animation_framerate << " " << curFrame << " " << index << " " << getAnimationLength(getAnimationSegment()) << endl;

    float bound = floor(getAnimationLength(getAnimationSegment()) * animation_framerate);

    int curFramePX = curFrame;

    if (!manual_mode) ///manual mode disables animation length limitations
    {
        if (curFrame > bound)
            curFrame = bound;

        curFramePX = curFrame - 1;
    }

    //if (anim_path == "resources/units/entity/kirajin.p4a")
    //cout << "file " << anim_path << " x: " << getGlobalPosition().x << " animation " << getAnimationSegment() << " frame " << curFrame << "/" << floor(getAnimationLength(getAnimationSegment()) * animation_framerate)-1 << " " << getAnimationLength(getAnimationSegment()) << " " << animation_framerate << " bounds: " << animation_bounds[index][curFrame].left << " " << animation_bounds[index][curFrame].top << " " << animation_bounds[index][curFrame].width << " " << animation_bounds[index][curFrame].height << endl;

    if (!offbounds)
    {
        //cout << "[AnimatedObject::Draw] Hitboxes" << endl;
        for (int i = 0; i < hitboxes.size(); i++)
        {
            hitboxes[i].SetPos(cur_pos);
        }

        for (int i = 0; i < objects.get()->size(); i++)
        {
            //cout << "[AnimatedObject::Draw] Object " << i << endl;

            if (qualitySetting != -1)
            (*objects)[i].s_obj.qualitySetting = qualitySetting;

            (*objects)[i].g_x = global_x;
            (*objects)[i].g_y = global_y;
            (*objects)[i].gl_x = local_x;
            (*objects)[i].gl_y = local_y;
            (*objects)[i].g_sx = scaleX;
            (*objects)[i].g_sy = scaleY;
            (*objects)[i].color = color;
            (*objects)[i].g_r = rotation;
            (*objects)[i].texture_path = current_animation;

            (*objects)[i].SetPosFrame(cur_pos, curFrame);

            if ((*objects)[i].object_name == "main")
            {
                if (force_origin_null) ///nullify the origin when the object is inanimate or you set a custom origin
                {
                    (*objects)[i].or_x = 0;
                    (*objects)[i].or_y = 0;
                }

                //cout << "Displaying animation " << getAnimationSegment() << ", time: " << getAnimationPos() << ":" << cur_pos << "/" << anim_end << " frame: " << curFrame+1 << "/" << (getAnimationLength(getAnimationSegment()) * animation_framerate) << " " << getAnimationLength(getAnimationSegment()) << endl;
                if (!manual_spritesheet)
                {
                    if (ao_version != 3)
                    {
                        //cout << "[AnimatedObject::Draw] Type 1" << endl;

                        if (curFrame != lastFrame)
                            (*objects)[i].swapTexture(afb[index][curFrame].image);

                    } else
                    {
                        if (curFrame == 0)
                        {
                            //(*objects)[i].swapTexture(animation_spritesheet[index].spritesheet);

                            if (bound == 0)
                            {
                                //cout << "[AnimatedObject::Draw] Type 2" << endl;
                                //cout << "A " << anim_path << ", swapping texture to: " << index << " " << curFrame - 1 << endl;
                                (*objects)[i].swapTexture((*all_swaps_img)[index][curFrame - 1]);
                            }
                        } else if ((curFrame != lastFrame) && (bound > 0))
                        {
                            //cout << anim_path << " Version 3 index " << index << " curFrame " << curFrame << " lastFrame " << lastFrame << " " << (*all_swaps_img)[index].size() << endl;
                            //cout << "Bounds width: " << animation_bounds[index][curFrame].width << endl;
                            //cout << "Bounds height: " << animation_bounds[index][curFrame].height << endl;

                            //if(curFrame < (*all_swaps_img)[index].size())
                            //cout << "[AnimatedObject::Draw] Type 3" << endl;
                            //cout << "[AnimatedObject::Draw] path: " << anim_path << endl;
                            //cout << "[AnimatedObject::Draw] index: " << index << endl;
                            //cout << "[AnimatedObject::Draw] curFramePX: " << curFramePX << endl;
                            //cout << "[AnimatedObject::Draw] (*all_swaps_img).size(): " << (*all_swaps_img).size() << endl;
                            //cout << "[AnimatedObject::Draw] (*all_swaps_img)[index].size(): " << (*all_swaps_img)[index].size() << endl;

                            if (animation_length_frames.size() > index)
                            {
                                if (curFramePX >= animation_length_frames[index])
                                    curFramePX = animation_length_frames[index] - 1;
                            }

                            std::string img_key = anim_path + "_" + getAnimationSegment() + "_" + to_string(curFramePX);

                            if (entityID == 5)
                                img_key = custom_img_key + to_string(curFramePX);

                            //if (curFramePX >= (*all_swaps_img)[index].size())
                                //curFramePX = (*all_swaps_img)[index].size() - 1;

                            (*objects)[i].swapTexture(img_key);
                        } else if (bound == 0)
                        {
                            //cout << "B " << anim_path << ", swapping texture to: " << index << " " << curFrame - 1 << endl;
                            //cout << "[AnimatedObject::Draw] Type 4" << endl;
                            (*objects)[i].swapTexture((*all_swaps_img)[index][curFrame - 1]);
                        } else
                        {
                            ///what?
                            //cout << "C " << anim_path << ", unknown behavior " << index << " " << curFrame - 1 << " " << bound << endl;
                        }
                    }

                    //cout << "[Object] Draw" << endl;

                    if (force_origin_null) ///nullify the origin when the object is inanimate or you set a custom origin
                    {
                        (*objects)[i].Draw(window, animation_origins[index][curFrame].x, animation_origins[index][curFrame].y);
                    } else
                    {
                        if (ao_version != 3)
                        {
                            //cout << "[AnimatedObject::Draw] Type 5" << endl;
                            (*objects)[i].Draw(window, afb[index][curFrame].origin.x, afb[index][curFrame].origin.y);
                        } else
                        {
                            //cout << "[AnimatedObject::Draw] Type 6" << endl;
                            (*objects)[i].Draw(window, (*animation_spritesheet)[index].spritesheet.getSize().x / 2, (*animation_spritesheet)[index].spritesheet.getSize().y / 2);
                        }
                    }
                } else
                {
                    //cout << "[AnimatedObject::Draw] Type 7" << endl;
                    (*objects)[i].Draw(window, animation_bounds[index][curFrame].left, animation_bounds[index][curFrame].top, animation_bounds[index][curFrame].width, animation_bounds[index][curFrame].height, animation_origins[index][curFrame].x, animation_origins[index][curFrame].y);
                }
            } else if ((*objects)[i].object_name.substr(0, 3) == "eq_")
            {
                //cout << "[AnimatedObject::Draw] Type 8" << endl;
                sf::Vector2f obj_origin = slots_origins[stoi((*objects)[i].object_name.substr((*objects)[i].object_name.find_first_of("_") + 1))];
                (*objects)[i].Draw(window, obj_origin.x, obj_origin.y);
            }
        }
    }

    //cout << "[AnimatedObject::Draw] FINISH" << endl;
    //cout << "[Object] Finished" << endl;

    lastFrame = curFrame;
}

void AnimatedObject::Update() {}
