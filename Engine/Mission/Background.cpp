#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "Background.h"
#include "../ResourceManager.h"
#include "../CoreManager.h"
#include "../Func.h"

#include <fstream>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <exception>

using namespace nlohmann;

Background::Background()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    bgView.setSize(sf::Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));
    bgView.setCenter(sf::Vector2f(static_cast<float>(window->getSize().x)/2, static_cast<float>(window->getSize().y)/2));
}

void Background::Load(const std::string& bg_name)
{
    SPDLOG_DEBUG("Loading background: {}", bg_name);

    
    

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
            for (auto& object : sunny)
            {
                SPDLOG_INFO("Object found: {}", std::string(object["type"]));
                if(object["type"] == "skybox")
                {
                    json color_points = object["color_points"];
                    for (auto& color_point : color_points)
                    {
                        int y_pos = color_point["y_pos"];
                        sf::Color color = sf::Color(color_point["color"][0], color_point["color"][1], color_point["color"][2], 255);

                        vx_pos.push_back(sf::Vector2f(0, y_pos * CoreManager::getInstance().getCore()->resRatio));
                        vx_pos.push_back(sf::Vector2f(3840 * CoreManager::getInstance().getCore()->resRatio, y_pos * CoreManager::getInstance().getCore()->resRatio));

                        vx_color.push_back(color);
                        vx_color.push_back(color);
                    }

                    sf::VertexArray tmp(sf::PrimitiveType::TriangleStrip, vx_pos.size());
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
                    tmp.position = sf::Vector2f(0, static_cast<float>(y_pos));
                    tmp.x_speed = x_speed;

                    bg_objects.push_back(tmp);
                }
            }
        }

        SPDLOG_INFO("Real name: {}", bg_real_name);
    } catch (const std::exception& e)
    {
        SPDLOG_ERROR("[ERROR] An error occured while loading mission: resources/graphics/bg/{}. Error: {}", bg_name, std::string(e.what()));
        return;
    }

    floor_height = float(330) * CoreManager::getInstance().getCore()->resRatio;
}

void Background::Draw(Camera& work_camera)
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();

    // TODO: patch this out (search for zoom_offset in mission controller for details)
    float zoom_offset = (0.000709722222222f * CoreManager::getInstance().getWindow()->getSize().y);

    for (int i = 0; i < vx_pos.size(); i++)
    {
        v_background[i].position = vx_pos[i];
        v_background[i].color = vx_color[i];
    }

    auto lastView = window->getView();

    window->setView(window->getDefaultView());

    window->draw(v_background);

    window->setView(bgView);
    work_camera.Work(bgView);

    

    for (auto& bg_object : bg_objects)
    {
        float camPos = (work_camera.camera_x + work_camera.zoom_x + work_camera.manual_x + work_camera.debug_x);
        float xPos = (camPos - 3840) - (camPos * bg_object.x_speed) - 99999;

        bg_object.texture.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(999999, static_cast<int>(bg_object.texture.getLocalBounds().size.y))));
        bg_object.texture.setRepeated(true);
        bg_object.texture.setOrigin(0, bg_object.texture.getLocalBounds().size.y);
        bg_object.texture.setColor(bg_object.color);
        bg_object.texture.setPosition(xPos / CoreManager::getInstance().getCore()->resRatio, bg_object.position.y + work_camera.zoom_y / zoom_offset);
        bg_object.texture.draw();
    }

    window->setView(lastView);
}

void Background::DrawFloor()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();

    auto lastView = window->getView();
    window->setView(window->getDefaultView());

    r_ground.setSize(sf::Vector2f(static_cast<float>(window->getSize().x), floor_height));
    r_ground.setFillColor(sf::Color::Black);
    r_ground.setPosition(sf::Vector2f(0, window->getSize().y - floor_height));
    window->draw(r_ground);

    window->setView(lastView);
}
