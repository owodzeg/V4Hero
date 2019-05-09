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

void Background::Load(string bg_name)
{
    cout<<"Background loaded: "<<bg_name;
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

            for(int i=0; i<v_vxparams.size(); i++)
            {
                vector<string> tmp = Func::Split(v_vxparams[i],',');

                sf::Vector2f tmp_vector;
                sf::Color tmp_color;

                tmp_vector.x = 0;
                tmp_vector.y = atof(tmp[0].c_str());

                tmp_color.r = atoi(tmp[1].c_str());
                tmp_color.g = atoi(tmp[2].c_str());
                tmp_color.b = atoi(tmp[3].c_str());

                sf::Vector2f tmp_vector2;

                tmp_vector2.x = 1280;
                tmp_vector2.y = atof(tmp[0].c_str());

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
            cout << "Loading texture..." << endl;

            vector<string> v_params = Func::Split(buff,',');

            sf::Texture t_temp;
            t_temp.loadFromFile("resources/graphics/bg/"+bg_name+"/"+v_params[0]);
            //t_temp.setSmooth(true);
            t_temp.setRepeated(true);

            sf::Sprite s_temp;
            s_temp.setTextureRect(sf::IntRect(0,0,500000,t_temp.getSize().y)); ///affect later with ratio
            s_temp.setTexture(t_temp);
            s_temp.setOrigin(10000,s_temp.getLocalBounds().height);
            s_temp.setColor(sf::Color(atoi(v_params[3].c_str()),atoi(v_params[4].c_str()),atoi(v_params[5].c_str()),255));
            s_temp.setPosition(-1000,610);

            t_background.push_back(t_temp);
            s_background.push_back(s_temp);
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

    for(int i=0; i<t_background.size(); i++)
    {
        s_background[i].setTexture(t_background[i]);

        s_background[i].setPosition(-(background_xspeed[i]*camera.camera_x),610);
        window.draw(s_background[i]);
    }
}
