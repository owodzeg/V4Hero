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

void Background::Load(const std::string& bg_name)
{
    SPDLOG_DEBUG("Loading background: {}", bg_name);

    float resRatioX = CoreManager::getInstance().getConfig()->GetInt("resX") / float(2160);
    float resRatioY = CoreManager::getInstance().getConfig()->GetInt("resY") / float(2160);

    std::ifstream bg(std::format("resources/graphics/bg/{}/background.json", bg_name));
    SPDLOG_INFO("Attempting to read a background from resources/graphics/bg/{}", bg_name);
    
    json bg_json;
    bg >> bg_json;

    bg.close();

    try
    {
        std::string bg_real_name = bg_json["name"];

        if(!bg_json.contains("sunny"))
        {
            SPDLOG_ERROR("no default sunny weather parameters.");
            throw std::exception();
        }
        else
        {
            json sunny = bg_json["sunny"];
            for(auto object : sunny)
            {
                SPDLOG_INFO("Object found: {}", object["type"]);
                if(object["type"] == "skybox")
                {
                    json color_points = object["color_points"];
                    for(auto color_point : color_points)
                    {
                        int y_pos = color_point["y_pos"];
                        sf::Color color = sf::Color(color_point["color"][0], color_point["color"][1], color_point["color"][2], 255);

                        vx_pos.push_back(sf::Vector2f(0, y_pos * resRatioY));
                        vx_pos.push_back(sf::Vector2f(2160 * resRatioX, y_pos * resRatioY));

                        vx_color.push_back(color);
                        vx_color.push_back(color);
                    }

                    sf::VertexArray tmp(sf::TrianglesStrip, vx_pos.size());
                    v_background = tmp;
                }
                else if(object["type"] == "image")
                {
                    std::string tex_name = object["texture"];
                    int y_pos = object["y_pos"];
                    float x_speed = object["x_speed"];
                    int layer = object["layer"];

                    BGObject tmp;

                    tmp.color = sf::Color::White;
                    if(object.contains("color"))
                    {
                        sf::Color color = sf::Color(object["color"][0], object["color"][1], object["color"][2], 255);
                        tmp.color = color;
                    }

                    tmp.texture.load(std::format("resources/graphics/bg/{}/{}", bg_name, tex_name));
                    tmp.position = sf::Vector2f(4000, y_pos);
                    tmp.x_speed = x_speed;

                    bg_objects.push_back(tmp);
                }
            }
        }

        SPDLOG_INFO("Real name: {}", bg_real_name);
    } catch (const exception& e)
    {
        SPDLOG_ERROR("[ERROR] An error occured while loading mission: resources/graphics/bg/{}. Error: {}", bg_name, e.what());
        return;
    }


    std::ifstream param_file("resources/graphics/bg/" + bg_name + "/param.dat");

    std::string buff;
    while (getline(param_file, buff))
    {
        if (buff.find("@back:") != std::string::npos)
        {

        } else
        {

        }
    }

    floor_height = float(110) * resRatioY;

    param_file.close();
}

void Background::Draw(Camera& camera)
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

    //for (int i=0; i<bg_objects.size(); i++)
    for (auto bg_object : bg_objects)
    {
        bg_object.texture.setTextureRect(sf::IntRect(0, 0, 500000, bg_object.texture.getGlobalBounds().height));
        bg_object.texture.setRepeated(true);
        bg_object.texture.setOrigin(0, bg_object.texture.getGlobalBounds().height);
        bg_object.texture.setColor(bg_object.color);
        //bg_objects[i].texture.setPosition((-(bg_objects[i].x_speed * camera.camera_x) - (bg_objects[i].x_speed * camera.manual_x) - (bg_objects[i].x_speed * camera.debug_x) - 1000), bg_objects[i].position.y);
        bg_object.texture.setPosition(-(-(camera.camera_x - camera.manual_x - camera.debug_x) + (bg_object.position.x + camera.manual_x)) * bg_object.x_speed - 1000, bg_object.position.y);
        bg_object.texture.draw();
    }

    auto lastView2 = window->getView();

    window->setView(window->getDefaultView());

    r_ground.setSize(sf::Vector2f(window->getSize().x, floor_height));
    r_ground.setFillColor(sf::Color::Black);
    r_ground.setPosition(0, window->getSize().y - floor_height);
    window->draw(r_ground);

    window->setView(lastView2);
}
