#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "Background.h"
#include "../ResourceManager.h"
#include "../CoreManager.h"
#include "../Func.h"

#include <fstream>
#include <spdlog/spdlog.h>

// TO-DO: REWRITE THIS!!!!!!!!! please

Background::Background()
{
}

void Background::setCamera(Camera newCamera)
{
    camera = newCamera;
}

void Background::Load(std::string bg_name)
{
    SPDLOG_DEBUG("Loading background: {}", bg_name);

    float resRatioX = CoreManager::getInstance().getConfig()->GetInt("resX") / float(1280);
    float resRatioY = CoreManager::getInstance().getConfig()->GetInt("resY") / float(720);

    v_background.clear();
    vx_pos.clear();
    vx_color.clear();

    temp_camerax = 0;

    std::ifstream param_file("resources/graphics/bg/" + bg_name + "/param.dat");

    std::string buff;
    while (getline(param_file, buff))
    {
        if (buff.find("@back:") != std::string::npos)
        {
            std::string vx_params = buff.substr(buff.find_first_of(":") + 1);
            std::vector<std::string> v_vxparams = Func::Split(vx_params, ';');

            for (int i = 0; i < v_vxparams.size(); i++)
            {
                std::vector<std::string> tmp = Func::Split(v_vxparams[i], ',');

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
            std::vector<std::string> v_params = Func::Split(buff, ',');

            SPDLOG_DEBUG("Loading texture: {}/{}", bg_name, v_params[0]);
            std::string t_name = "resources/graphics/bg/" + bg_name + "/" + v_params[0];

            BGObject tmp;
            tmp.texture.load(t_name);
            tmp.color = sf::Color(atoi(v_params[3].c_str()), atoi(v_params[4].c_str()), atoi(v_params[5].c_str()), 255);
            tmp.position = sf::Vector2f(-1000, atoi(v_params[1].c_str()));
            tmp.x_speed = atof(v_params[2].c_str());
            bg_objects.push_back(tmp);
        }
    }

    floor_height = float(110) * resRatioY;

    param_file.close();
}

void Background::Draw()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();

    for (int i = 0; i < vx_pos.size(); i++)
    {
        v_background[i].position = vx_pos[i];
        v_background[i].color = vx_color[i];
    }

    auto lastView = window->getView();

    window->setView(window->getDefaultView());

    window->draw(v_background);

    window->setView(lastView);

    for (int i=0; i<bg_objects.size(); i++)
    {
        bg_objects[i].texture.setTextureRect(sf::IntRect(0, 0, 500000, bg_objects[i].texture.getGlobalBounds().height));
        bg_objects[i].texture.setRepeated(true);
        bg_objects[i].texture.setOrigin(0, bg_objects[i].texture.getGlobalBounds().height);
        bg_objects[i].texture.setColor(bg_objects[i].color);
        bg_objects[i].texture.setPosition((-(bg_objects[i].x_speed * camera.camera_x) - (bg_objects[i].x_speed * camera.manual_x) - (bg_objects[i].x_speed * camera.debug_x) - 1000), bg_objects[i].position.y);
        bg_objects[i].texture.draw();
    }

    auto lastView2 = window->getView();

    window->setView(window->getDefaultView());

    r_ground.setSize(sf::Vector2f(window->getSize().x, floor_height));
    r_ground.setFillColor(sf::Color::Black);
    r_ground.setPosition(0, window->getSize().y - floor_height);
    window->draw(r_ground);

    window->setView(lastView2);
}
