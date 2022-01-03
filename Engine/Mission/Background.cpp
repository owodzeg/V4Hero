#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "Background.h"
#include "../ResourceManager.h"

#include "../Func.h"
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>

using namespace std;

Background::Background()
{
}

void Background::setCamera(Camera newCamera)
{
    camera = newCamera;
}

void Background::Load(string bg_name, Config& thisConfigs)
{
    thisConfig = &thisConfigs;
    SPDLOG_DEBUG("Loading background: {}", bg_name);

    quality = thisConfig->GetInt("textureQuality");

    float ratioX, ratioY;
    switch (quality)
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

    float resRatioX = thisConfig->GetInt("resX") / float(1280);
    float resRatioY = thisConfig->GetInt("resY") / float(720);

    v_background.clear();
    vx_pos.clear();
    vx_color.clear();
    p_background.clear();
    s_background.clear();
    background_xspeed.clear();
    background_y.clear();

    temp_camerax = 0;
    bg_layer = 0;

    ifstream param_file("resources/graphics/bg/" + bg_name + "/param.dat");

    string buff;
    while (getline(param_file, buff))
    {
        if (buff.find("@back:") != std::string::npos)
        {
            string vx_params = buff.substr(buff.find_first_of(":") + 1);
            vector<string> v_vxparams = Func::Split(vx_params, ';');

            //float resRatioY = float(720) / float(720);

            for (int i = 0; i < v_vxparams.size(); i++)
            {
                vector<string> tmp = Func::Split(v_vxparams[i], ',');

                sf::Vector2f tmp_vector;
                sf::Color tmp_color;

                tmp_vector.x = 0;
                tmp_vector.y = atof(tmp[0].c_str()) * resRatioY;

                if (tmp[0] == "-1")
                {
                    tmp_vector.y = float(610) * resRatioY;
                }

                tmp_color.r = atoi(tmp[1].c_str());
                tmp_color.g = atoi(tmp[2].c_str());
                tmp_color.b = atoi(tmp[3].c_str());

                sf::Vector2f tmp_vector2;

                tmp_vector2.x = float(1280) * resRatioX;
                tmp_vector2.y = atof(tmp[0].c_str()) * resRatioY;

                if (tmp[0] == "-1")
                {
                    tmp_vector2.y = float(610) * resRatioY;
                }

                vx_pos.push_back(tmp_vector);
                vx_color.push_back(tmp_color);

                vx_pos.push_back(tmp_vector2);
                vx_color.push_back(tmp_color);
            }

            sf::VertexArray tmp(sf::TrianglesStrip, vx_pos.size());
            v_background = tmp;
        } else
        {
            vector<string> v_params = Func::Split(buff, ',');

            SPDLOG_DEBUG("Loading texture: {}/{}", bg_name, v_params[0]);

            s_background[bg_layer] = ResourceManager::getInstance().getSprite("resources/graphics/bg/" + bg_name + "/" + v_params[0]);
            s_background[bg_layer].setRepeated(true);
            s_background[bg_layer].setTextureRect(sf::IntRect(0, 0, 500000, s_background[bg_layer].getGlobalBounds().height));
            s_background[bg_layer].setOrigin(10000, s_background[bg_layer].getGlobalBounds().height);
            s_background[bg_layer].setColor(sf::Color(atoi(v_params[3].c_str()), atoi(v_params[4].c_str()), atoi(v_params[5].c_str()), 255));
            s_background[bg_layer].setPosition(-1000, atoi(v_params[1].c_str()));
            s_background[bg_layer].setSmooth(false);

            sf::Vector2f tmpp;

            tmpp.x = -1000;
            tmpp.y = atoi(v_params[1].c_str());

            p_background.push_back(tmpp);
            background_xspeed.push_back(atof(v_params[2].c_str()));

            bg_layer++;
        }
    }

    floor_height = float(110) * resRatioY;

    param_file.close();
}

void Background::Draw(sf::RenderWindow& window)
{
    float resRatioX = window.getSize().x / float(1280);
    float resRatioY = window.getSize().y / float(720);

    for (int i = 0; i < vx_pos.size(); i++)
    {
        v_background[i].position = vx_pos[i];
        v_background[i].color = vx_color[i];
    }

    auto lastView = window.getView();

    window.setView(window.getDefaultView());

    window.draw(v_background);

    window.setView(lastView);

    for (int i = 0; i < bg_layer; i++)
    {
        s_background[i].setPosition((-(background_xspeed[i] * camera.camera_x) - (background_xspeed[i] * camera.manual_x) - (background_xspeed[i] * camera.debug_x)) / resRatioX, p_background[i].y);
        s_background[i].draw(window);
    }

    auto lastView2 = window.getView();

    window.setView(window.getDefaultView());

    r_ground.setSize(sf::Vector2f(window.getSize().x, floor_height));
    r_ground.setFillColor(sf::Color::Black);
    r_ground.setPosition(0, window.getSize().y - floor_height);
    window.draw(r_ground);

    window.setView(lastView2);
}
