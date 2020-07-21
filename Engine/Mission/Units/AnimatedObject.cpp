#include "AnimatedObject.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include "../../Func.h"
#include "../../P4A.h"
#include "Object.h"
#include <sstream>
#include <cstdlib>
#include <chrono>

AnimatedObject::AnimatedObject()
{

}

void AnimatedObject::loadAnim(std::string data, P4A handle)
{
    ///LOADANIM ver from 9.01.2020
    cout << "Loading animation! Data size " << data.size() << endl;

    objects.clear();

    bool legit = false;
    string version = "";

    istringstream iss(data);
    string line;
    while(getline(iss, line))
    {
        if(line.find("V4Mater-ver-") != std::string::npos)
        {
            version = line.substr(line.find("V4Mater-ver-")+12);
            legit = true;

            cout << "Anim format legit. Version " << version << endl;
        }

        /**
        [READ] V4Mater-ver-1.00
        [READ] S:9.799995
        [READ] OI:button_idle.png
        [READ] F:animation_name,0.000000,0,106,502,0,24,24,1,1
        [READ] F:animation_name,8.003246,0,1055,146,180.875,24,24,1,1
        */

        if(legit)
        {
            if(version == "1.02-experimental")
            {
                if(line.find("S:") != std::string::npos)
                {
                    max_time = atof(line.substr(line.find_first_of(":")+1).c_str());
                    cout << "Time: " << max_time << endl;
                }

                if(line.find("FR:") != std::string::npos)
                {
                    animation_framerate = atof(line.substr(line.find_first_of(":")+1).c_str());
                    cout << "Animation framerate: " << animation_framerate << endl;
                }

                if(line.find("XB:") != std::string::npos)
                {
                    xBound = atoi(line.substr(line.find_first_of(":")+1).c_str());
                    cout << "Animation X bound: " << xBound << endl;
                }

                if(line.find("YB:") != std::string::npos)
                {
                    yBound = atoi(line.substr(line.find_first_of(":")+1).c_str());
                    cout << "Animation Y bound: " << yBound << endl;
                }

                if(line.find("AFB:") != std::string::npos)
                {
                    string animdata = line.substr(line.find_first_of(":")+1);
                    vector<string> anim = Func::Split(animdata,',');

                    int animID = atoi(anim[0].c_str());
                    int animFrame = atoi(anim[1].c_str());
                    int x1 = atoi(anim[2].c_str());
                    int y1 = atoi(anim[3].c_str());
                    int x2 = atoi(anim[4].c_str());
                    int y2 = atoi(anim[5].c_str());
                    int orx = 0;
                    int ory = 0;

                    if(anim.size() > 6)
                    {
                        orx = atoi(anim[6].c_str());
                        ory = atoi(anim[7].c_str());
                    }

                    sf::IntRect rect(x1,y1,x2,y2);
                    sf::Vector2f origin(orx, ory);
                    animation_bounds[animID][animFrame] = rect;
                    animation_origins[animID][animFrame] = origin;

                    //cout << "Creating AnimationFrameBound... " << endl;

                    AnimationFrameBound tmp;
                    tmp.image.create(rect.width, rect.height, sf::Color::Transparent);
                    tmp.image.copy(animation_spritesheet[animID].spritesheet, 0, 0, rect);
                    tmp.rect = rect;
                    tmp.origin = origin;

                    afb[animID][animFrame] = tmp;

                    //cout << "created." << endl;

                    //cout << "Animation " << animation_names[animID] << " frame " << animFrame << " bounds: " << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
                }

                if(line.find("A:") != std::string::npos)
                {
                    string animdata = line.substr(line.find_first_of(":")+1);
                    vector<string> anim = Func::Split(animdata,',');

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
                    if(anim.size() >= 4)
                    {
                        animation_goto.push_back(anim[3]);
                        animation_loop.push_back(atoi(anim[4].c_str()));
                    }
                    else
                    {
                        animation_goto.push_back("");
                        animation_loop.push_back(1);
                    }

                    ///spritesheet implementation

                    cout << "Loading the spritesheet" << endl;

                    Animation tmp;
                    sf::Texture spr;
                    //cout << "Loading " << anim[2] << ".png" << endl;
                    vector<char> h = handle.ReadToMemoryChar(anim[2]+".png");
                    tmp.spritesheet.loadFromMemory(&h[0], h.size());
                    //tmp.spritesheet.loadFromMemory(&mem[0], mem.size());
                    tmp.name = anim[2];
                    animation_spritesheet.push_back(tmp);

                    cout << "Loaded spritesheet " << anim[2] << endl;
                }

                if(line.find("OI:") != std::string::npos)
                {
                    //auto start = std::chrono::high_resolution_clock::now();

                    string tex_file = line.substr(line.find_first_of(":")+1);

                    int parent = 0; ///unused yet

                    objects.emplace_back();

                    //auto elapsed = std::chrono::high_resolution_clock::now() - start;
                    //cout << "Pushed to vector, took " << std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count() << endl;

                    //cout << "Added new object from " << tex_file << endl;
                }

                if(line.find("HB:") != std::string::npos)
                {
                    string hitboxdata = line.substr(line.find_first_of(":")+1);
                    vector<string> hitbox = Func::Split(hitboxdata,',');

                    if(hitbox[0] == "rectangle")
                    {
                        Hitbox tmp;
                        hitboxes.push_back(tmp);

                        cout << "Added new hitbox, type " << hitbox[0] << endl;
                    }
                }

                if((line.find("F:") != std::string::npos) && (line[0] == 'F'))
                {
                    string framedata = line.substr(line.find_first_of(":")+1);
                    vector<string> frame = Func::Split(framedata,',');

                    float time = atof(frame[0].c_str());
                    int objectID = atoi(frame[1].c_str());

                    float pos_x = atof(frame[2].c_str());
                    float pos_y = atof(frame[3].c_str());
                    float rotation = atof(frame[4].c_str());
                    float or_x = atof(frame[5].c_str());
                    float or_y = atof(frame[6].c_str());
                    float scale_x = atof(frame[7].c_str());
                    float scale_y = atof(frame[8].c_str());
                    objects[objectID].SetCustomFrame(time,pos_x,pos_y,or_x,or_y,rotation,scale_x,scale_y);
                    //objects[objectID*2+1].SetCustomFrame(time,pos_x,pos_y,1,1,rotation,scale_x,scale_y);
                }

                if(line.find("HBF:") != std::string::npos)
                {
                    string hbframedata = line.substr(line.find_first_of(":")+1);
                    vector<string> hbframe = Func::Split(hbframedata,',');

                    float time = atof(hbframe[0].c_str());
                    int hitboxID = atoi(hbframe[1].c_str());

                    float g_x = atof(hbframe[2].c_str());
                    float g_y = atof(hbframe[3].c_str());

                    float width = atof(hbframe[6].c_str());
                    float height = atof(hbframe[7].c_str());
                    float x = atof(hbframe[4].c_str())+width/2;
                    float y = atof(hbframe[5].c_str())+height/2;
                    float rotation = 0;///3.14159265358/6; /// this needs to be loaded later IN RADIANS
                    hitboxes[hitboxID].SetCustomFrame(time,g_x,g_y,x,y,rotation,width,height);
                }
            }
            else if(version == "1.01")
            {
                //cout << "[READ " << version << "]: " << line << endl;

                if(line.find("S:") != std::string::npos)
                {
                    max_time = atof(line.substr(line.find_first_of(":")+1).c_str());
                    cout << "Time: " << max_time << endl;
                }

                if(line.find("A:") != std::string::npos)
                {
                    string animdata = line.substr(line.find_first_of(":")+1);
                    vector<string> anim = Func::Split(animdata,',');

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
                    if(anim.size() >= 4)
                    animation_goto.push_back(anim[3]);
                    else
                    animation_goto.push_back("");
                }

                if(line.find("OI:") != std::string::npos)
                {
                    //auto start = std::chrono::high_resolution_clock::now();

                    string tex_file = line.substr(line.find_first_of(":")+1);

                    int parent = 0; ///unused yet

                    objects.emplace_back(handle.ReadToMemory(tex_file),0,0,objects.size(),parent);

                    //auto elapsed = std::chrono::high_resolution_clock::now() - start;
                    //cout << "Pushed to vector, took " << std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count() << endl;

                    //cout << "Added new object from " << tex_file << endl;
                }

                if(line.find("HB:") != std::string::npos)
                {
                    string hitboxdata = line.substr(line.find_first_of(":")+1);
                    vector<string> hitbox = Func::Split(hitboxdata,',');

                    if(hitbox[0] == "rectangle")
                    {
                        Hitbox tmp;
                        hitboxes.push_back(tmp);

                        cout << "Added new hitbox, type " << hitbox[0] << endl;
                    }
                }

                if((line.find("F:") != std::string::npos) && (line[0] == 'F'))
                {
                    string framedata = line.substr(line.find_first_of(":")+1);
                    vector<string> frame = Func::Split(framedata,',');

                    float time = atof(frame[0].c_str());
                    int objectID = atoi(frame[1].c_str());

                    float pos_x = atof(frame[2].c_str());
                    float pos_y = atof(frame[3].c_str());
                    float rotation = atof(frame[4].c_str());
                    float or_x = atof(frame[5].c_str());
                    float or_y = atof(frame[6].c_str());
                    float scale_x = atof(frame[7].c_str());
                    float scale_y = atof(frame[8].c_str());
                    objects[objectID].SetCustomFrame(time,pos_x,pos_y,or_x,or_y,rotation,scale_x,scale_y);
                    //objects[objectID*2+1].SetCustomFrame(time,pos_x,pos_y,1,1,rotation,scale_x,scale_y);
                }

                if(line.find("HBF:") != std::string::npos)
                {
                    string hbframedata = line.substr(line.find_first_of(":")+1);
                    vector<string> hbframe = Func::Split(hbframedata,',');

                    float time = atof(hbframe[0].c_str());
                    int hitboxID = atoi(hbframe[1].c_str());

                    float g_x = atof(hbframe[2].c_str());
                    float g_y = atof(hbframe[3].c_str());

                    float width = atof(hbframe[6].c_str());
                    float height = atof(hbframe[7].c_str());
                    float x = atof(hbframe[4].c_str())+width/2;
                    float y = atof(hbframe[5].c_str())+height/2;
                    float rotation = 0;///3.14159265358/6; /// this needs to be loaded later IN RADIANS
                    hitboxes[hitboxID].SetCustomFrame(time,g_x,g_y,x,y,rotation,width,height);
                }
            }
            else if(version == "1.00")
            {
                cout << "[READ " << version << "]: " << line << endl;

                if(line.find("S:") != std::string::npos)
                {
                    max_time = atof(line.substr(line.find_first_of(":")+1).c_str());
                    cout << "Time: " << max_time << endl;
                }

                if(line.find("OI:") != std::string::npos)
                {
                    string objectdata = line.substr(line.find_first_of(":")+1);
                    vector<string> object = Func::Split(objectdata,',');

                    string tex_file = object[0];
                    string texture_data = handle.ReadToMemory(tex_file);

                    //cout << "texture data: " << texture_data.size() << endl;

                    sf::Texture obj_texture;
                    obj_texture.loadFromMemory(texture_data.data(),texture_data.size());
                    obj_texture.setSmooth(true);

                    int parent = 0; ///unused yet

                    Object tmp;
                    tmp.Load(obj_texture,0,0);
                    tmp.layer = objects.size();
                    tmp.parent = parent;
                    objects.push_back(tmp);

                    //cout << "Added new object from " << tex_file << endl;
                }

                if(line.find("F:") != std::string::npos)
                {
                    string framedata = line.substr(line.find_first_of(":")+1);
                    vector<string> frame = Func::Split(framedata,',');

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
                    objects[objectID].SetCustomFrame(time,pos_x,pos_y,or_x,or_y,rotation,scale_x,scale_y);
                    //objects[objectID*2+1].SetCustomFrame(time,pos_x,pos_y,1,1,rotation,scale_x,scale_y);
                }
            }
        }
    }

    ///init first animation (change later to an option "default animation")
    setAnimationSegment(animation_names[0]);

    ///add some randomness to the animations so it looks more natural
    int anim_size = floor((anim_end - anim_begin) * 100000);
    int rand_pos = (rand() << 15 | rand()) % anim_size;

    cur_pos = anim_begin + (float(rand_pos) / 100000);
    cout << "Random cur_pos: " << anim_size << " " << rand_pos << " " << cur_pos << endl;
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

     if(new_segment_name != getAnimationSegment())
    {
        //cout << "Changing spritesheet to " << new_segment_name << endl;
        //objects[0].tex_obj.loadFromMemory(&animation_spritesheet[getSegmentIndex(new_segment_name)].spritesheet[0], animation_spritesheet[getSegmentIndex(new_segment_name)].spritesheet.size());
        //objects[0].swapSpritesheet(getSegmentIndex(new_segment_name));
        //cout << "Swapped spritesheet. Took " << c.restart().asMicroseconds() << "microseconds" << endl;
    }

    if(animation_begin.size() > 1)
    {
        anim_begin = animation_begin[distance(animation_names.begin(), find(animation_names.begin(), animation_names.end(), new_segment_name))];
        anim_end = animation_end[distance(animation_names.begin(), find(animation_names.begin(), animation_names.end(), new_segment_name))];

        current_animation = new_segment_name;
    }
    else
    {
        anim_begin = animation_begin[0];
        anim_end = animation_end[0];

        current_animation = new_segment_name;
    }
}

void AnimatedObject::setAnimationSegment(std::string new_segment_name, bool force_start)
{
    //cout << "AnimatedObject::setAnimationSegment(" << new_segment_name << ");" << endl;
    //cout << new_segment_name << " vs " << getAnimationSegment() << endl;

    sf::Clock c;

    if(new_segment_name != getAnimationSegment())
    {
        //cout << "Changing spritesheet to " << new_segment_name << endl;

        //objects[0].tex_obj.loadFromMemory(&animation_spritesheet[getSegmentIndex(new_segment_name)].spritesheet[0], animation_spritesheet[getSegmentIndex(new_segment_name)].spritesheet.size());
        //objects[0].swapSpritesheet(getSegmentIndex(new_segment_name));
        //cout << "Swapped spritesheet. Took " << c.restart().asMicroseconds() << "microseconds" << endl;
    }

    if(animation_begin.size() > 1)
    {
        anim_begin = animation_begin[distance(animation_names.begin(), find(animation_names.begin(), animation_names.end(), new_segment_name))];
        anim_end = animation_end[distance(animation_names.begin(), find(animation_names.begin(), animation_names.end(), new_segment_name))];

        current_animation = new_segment_name;
    }
    else
    {
        anim_begin = animation_begin[0];
        anim_end = animation_end[0];

        current_animation = new_segment_name;
    }

    if(force_start)
    cur_pos = anim_begin;
}

std::string AnimatedObject::getAnimationSegment()
{
    return current_animation;
}

float AnimatedObject::getAnimationLength(std::string segment_name)
{
    float b,e;

    if(animation_begin.size() > 1)
    {
        b = animation_begin[distance(animation_names.begin(), find(animation_names.begin(), animation_names.end(), segment_name))];
        e = animation_end[distance(animation_names.begin(), find(animation_names.begin(), animation_names.end(), segment_name))];
    }
    else
    {
        b = animation_begin[0];
        e = animation_end[0];
    }

    return e-b;
}

float AnimatedObject::getAnimationPos()
{
    float e;

    if(animation_begin.size() > 1)
    {
        e = animation_end[distance(animation_names.begin(), find(animation_names.begin(), animation_names.end(), current_animation))];
    }
    else
    {
        e = animation_end[0];
    }

    return getAnimationLength(current_animation)-(e-cur_pos);
}

void AnimatedObject::setGlobalPosition(sf::Vector2f pos)
{
    global_x = pos.x;
    global_y = pos.y;
}

sf::Vector2f AnimatedObject::getGlobalPosition()
{
    return sf::Vector2f(global_x+local_x,global_y+local_y);
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

void AnimatedObject::LoadConfig(Config *thisConfigs, std::string unitParamPath)
{
    P4A handle;
    anim_path = unitParamPath;

    if(unitParamPath != "")
    {
        handle.ReadDictionary(unitParamPath);
        string animdata = handle.ReadToMemory("data.anim");

        if(animdata == "")
        {
            cout << "Invalid animation file!" << endl;
        }

        loadAnim(animdata,handle);
    }

    thisConfig = thisConfigs;
}

void AnimatedObject::Draw(sf::RenderWindow& window)
{
    if(!manual_mode) ///manual mode is restricted only to unique entities, never intended for use since it disables the whole animation
    {
        cur_pos += framerate / float(fps);

        if(cur_pos < anim_begin)
        cur_pos = anim_begin;

        if(cur_pos > anim_end)
        {
            if(loopable)
            {
                cur_pos -= (anim_end - anim_begin);
            }
            else
            {
                if(animation_goto[getSegmentIndex(current_animation)] != "")
                {
                    cout << "Animation go to: " << animation_goto[getSegmentIndex(current_animation)] << " SegmentIndex: " << getSegmentIndex(current_animation) << " current animation: " << current_animation << endl;
                    setAnimationSegment(animation_goto[getSegmentIndex(current_animation)], true);
                }
                else
                {
                    cur_pos = anim_end;
                }
            }
        }

        setLoop(animation_loop[getSegmentIndex(current_animation)]);
    }

    ///calculate current position on the spritesheet
    curFrame = floor(getAnimationPos() * animation_framerate);
    index = getSegmentIndex(getAnimationSegment());

    float bound = floor(getAnimationLength(getAnimationSegment()) * animation_framerate) - 1;

    if(!manual_mode) ///manual mode disables animation length limitations
    {
        if(curFrame > bound)
        curFrame = bound;
    }

    //cout << "file " << anim_path << " animation " << getAnimationSegment() << " frame " << curFrame << "/" << floor(getAnimationLength(getAnimationSegment()) * animation_framerate)-1 << " bounds: " << animation_bounds[index][curFrame].left << " " << animation_bounds[index][curFrame].top << " " << animation_bounds[index][curFrame].width << " " << animation_bounds[index][curFrame].height << endl;

    if(!offbounds)
    {
        for(int i=0; i<hitboxes.size(); i++)
        {
            hitboxes[i].SetPos(cur_pos);
        }

        for(int i=0; i<objects.size(); i++)
        {
            objects[i].g_x = global_x;
            objects[i].g_y = global_y;
            objects[i].gl_x = local_x;
            objects[i].gl_y = local_y;
            objects[i].g_sx = scaleX;
            objects[i].g_sy = scaleY;
            objects[i].color = color;
            objects[i].g_r = rotation;

            objects[i].SetPos(cur_pos);

            if(force_origin_null) ///nullify the origin when the object is inanimate or you set a custom origin
            {
                objects[i].or_x = 0;
                objects[i].or_y = 0;
            }

            //cout << "Displaying animation " << getAnimationSegment() << ", time: " << getAnimationPos() << ":" << cur_pos << "/" << anim_end << " frame: " << curFrame+1 << "/" << (getAnimationLength(getAnimationSegment()) * animation_framerate) << " " << getAnimationLength(getAnimationSegment()) << endl;
            if(!manual_spritesheet)
            {
                if(curFrame != lastFrame)
                objects[i].swapTexture(afb[index][curFrame].image);

                if(force_origin_null) ///nullify the origin when the object is inanimate or you set a custom origin
                {
                    objects[i].Draw(window, 0, 0);
                }
                else
                {
                    objects[i].Draw(window, afb[index][curFrame].origin.x, afb[index][curFrame].origin.y);
                }
            }
            else
            {
                objects[i].Draw(window, animation_bounds[index][curFrame].left, animation_bounds[index][curFrame].top, animation_bounds[index][curFrame].width, animation_bounds[index][curFrame].height, animation_origins[index][curFrame].x, animation_origins[index][curFrame].y);
            }
        }
    }

    lastFrame = curFrame;
}
