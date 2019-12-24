#include "AnimatedObject.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include "../../Func.h"
#include "../../P4A.h"
#include "Object.h"
#include <sstream>
AnimatedObject::AnimatedObject()
{

}

void AnimatedObject::loadAnim(std::string data, P4A handle)
{
    ///LOADANIM ver from 6.10.2019

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

        if(legit)
        {
            if(version == "1.00")
            {
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

                    cout << "texture data: " << texture_data.size() << endl;

                    sf::Texture obj_texture;
                    obj_texture.loadFromMemory(texture_data.data(),texture_data.size());
                    obj_texture.setSmooth(true);

                    int parent = 0; ///unused yet

                    Object tmp;
                    tmp.Load(obj_texture,0,0);
                    tmp.layer = objects.size();
                    tmp.parent = parent;
                    objects.push_back(tmp);

                    cout << "Added new object from " << tex_file << endl;

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
                }
            }
        }
    }

    //setPositions(0);
}

void AnimatedObject::LoadConfig(Config *thisConfigs, std::string unitParamPath)
{
    P4A handle;

    /*if(unitParamPath != "")
    {
        handle.ReadDictionary(unitParamPath);
        string animdata = handle.ReadToMemory("data.anim");

        if(animdata == "")
        {
            cout << "Invalid animation file!" << endl;
        }

        loadAnim(animdata,handle);
    }*/
 thisConfig = thisConfigs;
 ifstream param(unitParamPath+"param.dat");//"resources/graphics/units/wall/param.dat");

    string buff;
    getline(param,buff);
    string name = buff.substr(0,buff.find_first_of(":"));
    string coords = buff.substr(buff.find_first_of(":")+1);

    std::vector<std::string> results = Func::Split(coords,',');


 float ratioX, ratioY;
    switch(thisConfigs->GetInt("textureQuality"))
    {
        case 0: ///low
        {
            ratioX = thisConfig->GetInt("resX") / float(640);
            ratioY = thisConfig->GetInt("resY") / float(360);
            break;
        }

        case 1: ///med
        {
            ratioX = thisConfig->GetInt("resX") / float(1280);
            ratioY = thisConfig->GetInt("resY") / float(720);
            break;
        }

        case 2: ///high
        {
            ratioX = thisConfig->GetInt("resX") / float(1920);
            ratioY = thisConfig->GetInt("resY") / float(1080);
            break;
        }

        case 3: ///ultra
        {
            ratioX = thisConfig->GetInt("resX") / float(3840);
            ratioY = thisConfig->GetInt("resY") / float(2160);
            break;
        }
    }
    animation_name.clear();
    animation_frames.clear();
    cout<<unitParamPath+"param.dat | "<<"RESULTS: "<<results.size()<<'\n';
    hitBox =  sf::Rect<float>(atof(results[0].c_str())*ratioX,atof(results[1].c_str())*ratioY,atof(results[2].c_str())*ratioX,atof(results[3].c_str())*ratioY);
    while(getline(param,buff))
    {
        string name = buff.substr(0,buff.find_first_of(","));
        string frames = buff.substr(buff.find_first_of(",")+1);

        animation_name.push_back(name);
        animation_frames.push_back(atoi(frames.c_str()));
    }

    param.close();

    for(int i=0; i<animation_name.size(); i++)
    {
        thisConfig->debugOut->DebugMessage("Loading animation '" + animation_name[i] + "'...\n");

        for(int a=1; a<=animation_frames[i]; a++)
        {
            sf::Texture temp;
            temp.loadFromFile(unitParamPath+animation_name[i]+"/"+to_string(a)+".png");
            temp.setSmooth(true);

            animation_textures[animation_name[i]].push_back(temp);
        }
    }
}
void AnimatedObject::Draw(sf::RenderWindow& window)
{
    /*cur_pos += 1 / float(fps);

    if(cur_pos > max_time)
    {
        cur_pos = 0;
    }

    for(int i=0; i<objects.size(); i++)
    {
        objects[i].SetPos(cur_pos);
        objects[i].Draw(window);
    }
*/
float ratioX, ratioY;
    switch(thisConfig->GetInt("textureQuality"))
    {
        case 0: ///low
        {
            ratioX = thisConfig->GetInt("resX") / float(640);
            ratioY = thisConfig->GetInt("resY") / float(360);
            break;
        }

        case 1: ///med
        {
            ratioX = thisConfig->GetInt("resX") / float(1280);
            ratioY = thisConfig->GetInt("resY") / float(720);
            break;
        }

        case 2: ///high
        {
            ratioX = thisConfig->GetInt("resX") / float(1920);
            ratioY = thisConfig->GetInt("resY") / float(1080);
            break;
        }

        case 3: ///ultra
        {
            ratioX = thisConfig->GetInt("resX") / float(3840);
            ratioY = thisConfig->GetInt("resY") / float(2160);
            break;
        }
    }
    s_wall.setTexture(animation_textures[current_animation][floor(current_frame)]);
    current_frame += float(60) / fps;

    if(current_frame >= animation_textures[current_animation].size())
    {
        current_frame = 0;
    }


    s_wall.setScale(scaleX,scaleY);

    s_wall.setPosition(x,y);
    width = s_wall.getGlobalBounds().width;
    sf::Rect<float> spriteSize = s_wall.getGlobalBounds();
    //cout<< spriteSize.left<<" "<<spriteSize.top<<" "<<spriteSize.width<<" "<<spriteSize.height<<endl;
    //cout<< hitBox.left<<" "<<hitBox.top<<" "<<hitBox.width<<" "<<hitBox.height<<endl;
    //hitBox = sf::Rect<float>(x*ratioX,y,spriteSize.width,spriteSize.height);

    window.draw(s_wall);
}
