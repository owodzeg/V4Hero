#include "AnimatedObject.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include "../../Func.h"
#include "../../P4A.h"
#include "Object.h"
#include <sstream>
#include <cstdlib>

AnimatedObject::AnimatedObject()
{

}

void AnimatedObject::loadAnim(std::string data, P4A handle)
{
    ///LOADANIM ver from 9.01.2020

    objects.clear();

    bool legit = false;
    string version = "";

    istringstream iss(data);
    string line;
    while(getline(iss, line))
    {
        if(line.find("V4Mater-ver-") != std::string::npos)
        {
            version = line.substr(line.find_last_of("-")+1);
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
            if(version == "1.01")
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

void AnimatedObject::setAnimationSegment(std::string new_segment_name)
{
    //cout << "AnimatedObject::setAnimationSegment(" << new_segment_name << ");" << endl;

    anim_begin = animation_begin[distance(animation_names.begin(), find(animation_names.begin(), animation_names.end(), new_segment_name))];
    anim_end = animation_end[distance(animation_names.begin(), find(animation_names.begin(), animation_names.end(), new_segment_name))];


    current_animation = new_segment_name;
}

std::string AnimatedObject::getAnimationSegment()
{
    return current_animation;
}

void AnimatedObject::setGlobalPosition(sf::Vector2f pos)
{
    global_x = pos.x;
    global_y = pos.y;
}

sf::Vector2f AnimatedObject::getGlobalPosition()
{
    return sf::Vector2f(global_x,global_y);
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

void AnimatedObject::LoadConfig(Config *thisConfigs, std::string unitParamPath)
{
    P4A handle;

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
}
void AnimatedObject::Draw(sf::RenderWindow& window)
{
    if(cur_pos < anim_begin)
    cur_pos = anim_begin;

    cur_pos += 1 / float(fps);

    if(cur_pos > anim_end)
    {
        if(loopable)
        cur_pos -= (anim_end - anim_begin);
        else
        cur_pos = anim_end;
    }

    ///SUPER TEMPORARY WORKAROUND FOR FEVER WORM, add one time animations that transfer to another immediately
    if(getAnimationSegment() == "transform")
    {
        if(cur_pos == anim_end)
        {
            setAnimationSegment("fever");
            setLoop(true);
            worm_fever = true;
        }
    }

    for(int i=0; i<hitboxes.size(); i++)
    {
        hitboxes[i].SetPos(cur_pos);
    }

    for(int i=0; i<objects.size(); i++)
    {
        objects[i].g_x = global_x;
        objects[i].g_y = global_y;
        objects[i].g_sx = scaleX;
        objects[i].g_sy = scaleY;

        objects[i].SetPos(cur_pos);
        objects[i].Draw(window);
    }
}
