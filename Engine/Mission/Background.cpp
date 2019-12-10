#include "Background.h"

#include <fstream>
#include "../Func.h"
#include <iostream>
using namespace std;

Background::Background()
{

}

void Background::setCamera(Camera newCamera)
{
    camera = newCamera;
}

void Background::Load(string bg_name,Config &thisConfigs)
{
    thisConfig = &thisConfigs;
    thisConfig->debugOut->DebugMessage("Background loaded: "+bg_name);

    quality = thisConfig->GetInt("textureQuality");

    float ratioX, ratioY;
    switch(quality)
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

    cout << ratioX << " " << ratioY << endl;

    v_background.clear();
    vx_pos.clear();
    vx_pos.clear();
    vx_color.clear();

    t_background.clear();
    s_background.clear();
    background_xspeed.clear();
    background_y.clear();

    temp_camerax = 0;
    ifstream param_file("resources/graphics/bg/"+bg_name+"/param.dat");

    string buff;
    while(getline(param_file,buff))
    {
        if(buff.find("@back:") != std::string::npos)
        {
            string vx_params = buff.substr(buff.find_first_of(":")+1);
            vector<string> v_vxparams = Func::Split(vx_params,';');

            //float ratioY = thisConfig->GetInt("resY") / float(720);

            for(int i=0; i<v_vxparams.size(); i++)
            {
                vector<string> tmp = Func::Split(v_vxparams[i],',');

                sf::Vector2f tmp_vector;
                sf::Color tmp_color;

                tmp_vector.x = 0;
                tmp_vector.y = atof(tmp[0].c_str()) * ratioY;

                if(tmp[0] == "-1")
                {
                    tmp_vector.y = thisConfig->GetInt("resY") - (110 * ratioY);
                }

                tmp_color.r = atoi(tmp[1].c_str());
                tmp_color.g = atoi(tmp[2].c_str());
                tmp_color.b = atoi(tmp[3].c_str());

                sf::Vector2f tmp_vector2;

                tmp_vector2.x = thisConfig->GetInt("resX");
                tmp_vector2.y = atof(tmp[0].c_str()) * ratioY;

                if(tmp[0] == "-1")
                {
                    tmp_vector2.y = thisConfig->GetInt("resY") - (110 * ratioY);
                }

                vx_pos.push_back(tmp_vector);
                vx_color.push_back(tmp_color);

                vx_pos.push_back(tmp_vector2);
                vx_color.push_back(tmp_color);
            }

            sf::VertexArray tmp(sf::TrianglesStrip,vx_pos.size());
            v_background = tmp;
        }
        else
        {
            thisConfig->debugOut->DebugMessage("Loading texture...\n");

            vector<string> v_params = Func::Split(buff,',');

            PSprite ps_temp;
            ps_temp.loadFromFile("resources/graphics/bg/"+bg_name+"/"+v_params[0],quality);
            ps_temp.setRepeated(true);
            ps_temp.setTextureRect(sf::IntRect(0,0,500000,ps_temp.t.getSize().y)); ///affect later with ratio
            ps_temp.setOrigin(10000,ps_temp.getLocalBounds().height);
            ps_temp.setColor(sf::Color(atoi(v_params[3].c_str()),atoi(v_params[4].c_str()),atoi(v_params[5].c_str()),255));
            ps_temp.setPosition(-1000,atoi(v_params[1].c_str())/ratioY);

            sf::Vector2f tmpp;

            tmpp.x = -1000;
            tmpp.y = atoi(v_params[1].c_str()) * ratioY;

            s_background.push_back(ps_temp);
            p_background.push_back(tmpp);
            background_xspeed.push_back(atof(v_params[2].c_str()));
        }
    }
}

void Background::Draw(sf::RenderWindow& window)
{
    for(int i=0; i<vx_pos.size(); i++)
    {
        v_background[i].position = vx_pos[i];
        v_background[i].color = vx_color[i];
    }

    auto lastView = window.getView();

    window.setView(window.getDefaultView());

    window.draw(v_background);

    window.setView(lastView);

    for(int i=0; i<s_background.size(); i++)
    {
        //s_background[i].setTexture(t_background[i]);

        s_background[i].setPosition(-(background_xspeed[i]*camera.camera_x)-(background_xspeed[i]*camera.manual_x)-(background_xspeed[i]*camera.debug_x),p_background[i].y);
        //cout << s_background[i].y << endl;
        s_background[i].draw(window);
    }
}
