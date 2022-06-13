#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "Obelisk.h"
#include "../V4Core.h"
#include "ButtonList.h"
#include "iostream"
#include "math.h"
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <spdlog/spdlog.h>
#include "../CoreManager.h"
#include "../StateManager.h"

using json = nlohmann::json;

ObeliskMenu::ObeliskMenu()
{
    SPDLOG_INFO("Initializing Obelisk...");

    font.loadFromFile(CoreManager::getInstance().getConfig()->fontPath);

    quality = CoreManager::getInstance().getConfig()->GetInt("textureQuality");

    float ratioX, ratioY;

    switch (quality)
    {
        case 0: ///low
        {
            ratioX = CoreManager::getInstance().getConfig()->GetInt("resX") / float(640);
            ratioY = CoreManager::getInstance().getConfig()->GetInt("resY") / float(360);
            break;
        }

        case 1: ///med
        {
            ratioX = CoreManager::getInstance().getConfig()->GetInt("resX") / float(1280);
            ratioY = CoreManager::getInstance().getConfig()->GetInt("resY") / float(720);
            break;
        }

        case 2: ///high
        {
            ratioX = CoreManager::getInstance().getConfig()->GetInt("resX") / float(1920);
            ratioY = CoreManager::getInstance().getConfig()->GetInt("resY") / float(1080);
            break;
        }

        case 3: ///ultra
        {
            ratioX = CoreManager::getInstance().getConfig()->GetInt("resX") / float(3840);
            ratioY = CoreManager::getInstance().getConfig()->GetInt("resY") / float(2160);
            break;
        }
    }

    PSprite i_hunt;
    i_hunt.loadFromFile("resources/graphics/ui/worldmap/hunting_icon.png", quality, 1);
    PSprite i_fortress;
    i_fortress.loadFromFile("resources/graphics/ui/worldmap/fortress_icon.png", quality, 1);

    mission_icons.push_back(i_hunt);
    mission_icons.push_back(i_fortress);

    dullpon.loadFromFile("resources/graphics/ui/worldmap/dullpon.png", quality, 1);

    ///boxes
    mainbox.load("resources/graphics/ui/worldmap/main_box.png");
    mainbox.setOrigin(sf::Vector2f(mainbox.getLocalBounds().width / 2, mainbox.getLocalBounds().height / 2));
    mainbox.setPosition(sf::Vector2f(640, 320));
    descbox.load("resources/graphics/ui/worldmap/description_box.png");
    descbox.setOrigin(sf::Vector2f(descbox.getLocalBounds().width / 2, descbox.getLocalBounds().height / 2));
    descbox.setPosition(sf::Vector2f(820, 542));
    iconbox.load("resources/graphics/ui/worldmap/icon_box.png");
    missionbox.load("resources/graphics/ui/worldmap/mission_box.png");
    missionbox.setOrigin(sf::Vector2f(missionbox.getLocalBounds().width / 2, missionbox.getLocalBounds().height / 2));
    missionbox.setPosition(sf::Vector2f(290, 542));


    missionselect.loadFromFile("resources/graphics/ui/worldmap/mission_select.png", quality, 1);

    worldmap_title.createText(font, 34, sf::Color::Black, Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("worldmap_header_1")), quality, 1);
    location_title.createText(font, 27, sf::Color::Black, Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("worldmap_location_1_title")), quality, 1);
    string desc = Func::wrap_text(CoreManager::getInstance().getStrRepo()->GetString("worldmap_location_1_description"), 800, font, 18);

    location_desc.createText(font, 19, sf::Color::Black, Func::ConvertToUtf8String(desc), quality, 1);
    select_quest.createText(font, 18, sf::Color::Black, Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("worldmap_select")), quality, 1);
    mission_title.createText(font, 18, sf::Color::Black, "Hunting Kacheek", quality, 1);
    mission_desc.createText(font, 18, sf::Color::Black, "(no translation needed)", quality, 1);

    unavailable.loadFromFile("resources/graphics/ui/worldmap/unavailable.png", quality, 1);
    location_highlight.loadFromFile("resources/graphics/ui/worldmap/location_highlight.png", quality, 1);
    location_highlight.setOrigin(location_highlight.getLocalBounds().width / 2, location_highlight.getLocalBounds().height / 2);

    mission_select.loadFromFile("resources/graphics/ui/worldmap/mission_select.png", quality, 1);

    ctrlTips.create(66, font, 20, sf::String("Left/Right: Select field      X: View missions      O: Exit to Patapolis"), quality);

    float resRatioX = CoreManager::getInstance().getConfig()->GetInt("resX") / float(1280);
    float resRatioY = CoreManager::getInstance().getConfig()->GetInt("resY") / float(720);

    SPDLOG_INFO("Initializing Obelisk finished.");
}

void ObeliskMenu::addMission(json missiondata)
{
    Mission tmp;
    try
    {
        tmp.mis_ID = missiondata["mission_id"];
    } catch (const std::exception& e)
    {
        tmp.mis_ID = 0;
    }
    try
    {
        tmp.loc_ID = missiondata["location_id"];
    } catch (const std::exception& e)
    {
        tmp.loc_ID = 1;
    }

    try
    {
        std::string title_key = missiondata["mission_title"];
        tmp.title = CoreManager::getInstance().getStrRepo()->GetString(title_key);
    } catch (const std::exception& e)
    {
        std::string title = "No Data";
        tmp.title = string(title.begin(), title.end());
    }
    try
    {
        std::string desc_key = missiondata["mission_description"];
        tmp.desc = CoreManager::getInstance().getStrRepo()->GetString(desc_key);
    } catch (const std::exception& e)
    {
        std::string desc = "No Data";
        tmp.desc = string(desc.begin(), desc.end());
    }

    try
    {
        tmp.mission_file = missiondata["mission_file"];
    } catch (const std::exception& e)
    {
        tmp.mission_file = "mis1_1.p4m";
    }

    string level = "";

    if (CoreManager::getInstance().getSaveReader()->mission_levels[tmp.mis_ID] != 0)
    {
        level = to_string(CoreManager::getInstance().getSaveReader()->mission_levels[tmp.mis_ID]);
    }

    PText tm;
    tm.createText(font, 18, sf::Color::Black, "", quality, 1);
    tm.setString(Func::ConvertToUtf8String(tmp.title) + level);
    tm.setOrigin(tm.getLocalBounds().width / 2, tm.getLocalBounds().height / 2);
    tmp.p_mis = tm;

    SPDLOG_DEBUG("[WorldMap] Making text {}", Func::ConvertToUtf8String(tmp.title).toAnsiString());

    missions.push_back(tmp);
}

void ObeliskMenu::Initialise()
{
    
}

void ObeliskMenu::Reload()
{
    location_bgs.clear();
    location_icons.clear();

    worldmap_fields.clear();
    worldmap_icons.clear();

    SPDLOG_TRACE("Location_bgs capacity: {}", location_bgs.capacity());

    ///Access the save data
    fields_unlocked = CoreManager::getInstance().getSaveReader()->locations_unlocked;
    missions_unlocked = CoreManager::getInstance().getSaveReader()->missions_unlocked;

    PSprite fld;
    fld.loadFromFile("resources/graphics/ui/worldmap/location_field.png", quality, 1);

    ///worldmap contents
    // i is location id, not an idx in fields_unlocked
    for (int i = 1; i <= *std::max_element(fields_unlocked.begin(), fields_unlocked.end()); i++)
    {
        if (std::find(fields_unlocked.begin(), fields_unlocked.end(), i) != fields_unlocked.end())
        {
            int curIdx = std::find(fields_unlocked.begin(), fields_unlocked.end(), i) - fields_unlocked.begin();

            PSprite bg;
            location_bgs.push_back(bg);
            location_bgs[location_bgs.size() - 1].loadFromFile("resources/graphics/ui/worldmap/locationbg_" + to_string(fields_unlocked[curIdx]) + ".png", quality, 1);

            PSprite loc;
            loc.loadFromFile("resources/graphics/ui/worldmap/location_" + to_string(fields_unlocked[curIdx]) + ".png", quality, 1);
            worldmap_icons.push_back(loc);
        } else
        {
            worldmap_fields.push_back(fld);
            worldmap_icons.push_back(dullpon);
        }
    }

    for (int i = 1; i <= 20; i++)
    {
        worldmap_fields.push_back(fld);

        if (i <= location_icons.size())
        {
            worldmap_icons.push_back(location_icons[i - 1]);
        } else
        {
            worldmap_icons.push_back(dullpon);
        }
    }

    //if (sel_location > 0 && sel_location < location_bgs.size())
    //{
    //    location_bg_a = location_bgs[sel_location - 1];
    //    location_bg_b = location_bgs[sel_location - 1];
    //}

    unlocked.clear();

    for (const auto& field : fields_unlocked)
    {
        unlocked.push_back(field - 1);
    }

    //vector<int> unlocked = {0};
}

void ObeliskMenu::EventFired(sf::Event event)
{
    if (is_active)
    {
        if (event.type == sf::Event::KeyPressed)
        {

        } else if (event.type == sf::Event::MouseButtonReleased)
        {
        }
    }
}

void ObeliskMenu::Update(sf::RenderWindow& window, float fps, InputController& inputCtrl)
{

}

void ObeliskMenu::Update()
{
    if (true)
    {
        InputController* inputCtrl = CoreManager::getInstance().getInputController();
        sf::RenderWindow* window = CoreManager::getInstance().getWindow();
        float fps = CoreManager::getInstance().getCore()->getFPS();

        window->setView(window->getDefaultView());

        float resRatioX = window->getSize().x / float(1280);
        float resRatioY = window->getSize().y / float(720);

        if (!displayMissions)
        {
            mainbox_destX = 640;
            mainbox_destY = 320;
        } else
        {
            mainbox_destX = 640;
            mainbox_destY = 230;
        }

        if (round(mainbox.getPosition().x) != mainbox_destX)
        {
            float speed = abs(mainbox.getPosition().x - mainbox_destX) * 10;

            if (mainbox.getPosition().x > mainbox_destX)
                mainbox.setPosition(sf::Vector2f(mainbox.getPosition().x - (speed / fps), mainbox.getPosition().y));
            else
                mainbox.setPosition(sf::Vector2f(mainbox.getPosition().x + (speed / fps), mainbox.getPosition().y));
        }

        if (round(mainbox.getPosition().y) != mainbox_destY)
        {
            float speed = abs(mainbox.getPosition().y - mainbox_destY) * 10;

            if (mainbox.getPosition().y > mainbox_destY)
                mainbox.setPosition(sf::Vector2f(mainbox.getPosition().x, mainbox.getPosition().y - (speed / fps)));
            else
                mainbox.setPosition(sf::Vector2f(mainbox.getPosition().x, mainbox.getPosition().y + (speed / fps)));
        }

        if (round(alphaA) != location_bg_a_destAlpha)
        {
            float speed = abs(alphaA - location_bg_a_destAlpha) * 8;

            if (alphaA > location_bg_a_destAlpha)
                alphaA -= speed / fps;
            else
                alphaA += speed / fps;
        }

        alphaB = 255 - alphaA;

        int renderPrev = sel_prevlocation - 1;

        if (renderPrev > location_bgs.size() - 1)
            renderPrev = location_bgs.size() - 1;

        int renderCur = sel_location - 1;

        if (renderCur > location_bgs.size() - 1)
            renderCur = location_bgs.size() - 1;

        if (!location_bgs.empty())
        {
            location_bgs[renderPrev].setColor(sf::Color(255, 255, 255, 255));
            location_bgs[renderPrev].draw(window);

            location_bgs[renderCur].setColor(sf::Color(255, 255, 255, alphaB));
            location_bgs[renderCur].draw(window);
        }

        mainbox.draw();

        worldmap_title.setOrigin(worldmap_title.getLocalBounds().width / 2, worldmap_title.getLocalBounds().height / 2);
        worldmap_title.setPosition(mainbox.getPosition().x - 303, mainbox.getPosition().y - 174);
        worldmap_title.draw(window);

        location_title.setOrigin(location_title.getLocalBounds().width / 2, location_title.getLocalBounds().height / 2);
        location_title.setPosition(mainbox.getPosition().x, mainbox.getPosition().y + 72);
        location_title.draw(window);

        location_desc.setOrigin(location_desc.getLocalBounds().width / 2, location_desc.getLocalBounds().height / 2);
        location_desc.setPosition(mainbox.getPosition().x, mainbox.getPosition().y + 142);
        location_desc.draw(window);

        if (round(mapX) != mapXdest)
        {
            float speed = abs(mapX - mapXdest) * 10;

            if (mapX > mapXdest)
                mapX -= speed / fps;
            else
                mapX += speed / fps;
        }

        if (mapX >= 0)
            mapX = 0;

        int maxBound = (worldmap_fields.size() * 123 - 1012) * (-1);

        if (mapX <= maxBound)
            mapX = maxBound;

        v_render_map.setSize(1012 * resRatioX, 720 * resRatioY);
        v_render_map.setCenter((506) * resRatioX, -360 * resRatioY);
        v_render_map.setViewport(sf::FloatRect(134.0 / 1280.0, (mainbox.getPosition().y - 115) / 720.0, 1012.0 / 1280.0, 1.f));

        window->setView(v_render_map);

        for (int i = 0; i < worldmap_fields.size(); i++)
        {
            worldmap_fields[i].setPosition(i * 123 + mapX, 0 - 720);
            worldmap_fields[i].draw(window);
        }

        location_highlight.setOrigin(location_highlight.getLocalBounds().width / 2, location_highlight.getLocalBounds().height / 2);
        location_highlight.setPosition(sel_location * 123 + mapX - 62, 78 - 720);
        location_highlight.draw(window);

        for (int i = 0; i < worldmap_fields.size(); i++)
        {
            worldmap_icons[i].setOrigin(worldmap_icons[i].getLocalBounds().width / 2, worldmap_icons[i].getLocalBounds().height);
            worldmap_icons[i].setPosition(i * 123 + mapX + 61, 155 - 720);
            worldmap_icons[i].draw(window);

            if (std::find(unlocked.begin(), unlocked.end(), i) == unlocked.end())
            {
                unavailable.setPosition(i * 123 + mapX - 1, 0 - 720);
                unavailable.draw(window);
            }
        }

        window->setView(window->getDefaultView());

        if (displayMissions)
        {
            missionbox.draw();
            descbox.draw();

            select_quest.setOrigin(0, select_quest.getLocalBounds().height / 2);
            select_quest.setPosition(missionbox.getPosition().x - 141, missionbox.getPosition().y - 74);
            select_quest.draw(window);

            mission_title.setOrigin(0, mission_title.getLocalBounds().height / 2);
            mission_title.setPosition(descbox.getPosition().x - 315, descbox.getPosition().y - 74);
            mission_title.draw(window);

            mission_desc.setOrigin(0, 0);
            mission_desc.setPosition(descbox.getPosition().x - 315, descbox.getPosition().y - 55);
            mission_desc.draw(window);

            mission_select.setPosition(135, missionbox.getPosition().y - 56 + (sel_mission * 24));
            mission_select.draw(window);

            v_render_missions_map.setSize(298 * resRatioX, 120 * resRatioY);
            v_render_missions_map.setCenter((1280 + 149) * resRatioX, (-720 + 60) * resRatioY);
            v_render_missions_map.setViewport(sf::FloatRect(143.0 / 1280.0, (missionbox.getPosition().y - 53) / 720.0, 298.0 / 1280.0, 120.0 / 720.0));

            window->setView(v_render_missions_map);

            for (int i = 0; i < missions.size(); i++)
            {
                missions[i].p_mis.setOrigin(0, 0);
                missions[i].p_mis.setPosition(1280, -720 + (i * 24));
                missions[i].p_mis.draw(window);
            }

            window->setView(window->getDefaultView());
        }

        if (screenFade.checkFinished())
        {
            if (inputCtrl->isKeyPressed(InputController::Keys::CIRCLE))
            {
                if (displayMissions)
                {
                    ctrlTips.create(66, font, 20, sf::String("Left/Right: Select field      X: View missions      O: Exit to Patapolis"), quality);

                    displayMissions = false;
                    SPDLOG_DEBUG("Exited mission selection.");
                } else
                {
                    //go back to patapolis
                    screenFade.Create(ScreenFade::FADEOUT, 1024);
                    goto_id = 0;

                    //parentMenu->screenFade.Create(thisConfig, 1, 1536);
                    //parentMenu->goto_id = 3;

                    //this->Hide();
                    //this->isActive = false;
                    SPDLOG_INFO("Exited Obelisk.");
                }
            } else if (inputCtrl->isKeyPressed(InputController::Keys::CROSS))
            {
                if (!displayMissions)
                {
                    SPDLOG_INFO("Displaying missions on Worldmap for location {}.", sel_location);

                    ///(re)load missions here
                    SPDLOG_DEBUG("Displaying missions");

                    missions.clear();

                    ifstream wmap("resources/missions/worldmap.dat", std::ios::in);
                    json wmap_data;

                    if (wmap.good())
                    {
                        wmap >> wmap_data;

                        for (const auto& missiondata : wmap_data)
                        {
                            if (missiondata["location_id"] == sel_location && CoreManager::getInstance().getSaveReader()->isMissionUnlocked(missiondata["mission_id"]))
                            {
                                addMission(missiondata);
                            }
                        }
                    }

                    wmap.close();

                    if (missions.size() > 0)
                    {
                        ctrlTips.create(66, font, 20, sf::String("Up/Down: Select mission      X: Enter mission      O: Return to field select"), quality);

                        displayMissions = true;

                        string level = "";

                        if (CoreManager::getInstance().getSaveReader()->mission_levels[missions[sel_mission].mis_ID] != 0)
                        {
                            level = to_string(CoreManager::getInstance().getSaveReader()->mission_levels[missions[sel_mission].mis_ID]);
                        }

                        mission_title.setString(Func::ConvertToUtf8String(missions[sel_mission].title) + level);
                        string desc = Func::wrap_text(missions[sel_mission].desc, 633, font, 18);
                        mission_desc.setString(Func::ConvertToUtf8String(desc));
                    }
                } else
                {
                    //go to barracks
                    screenFade.Create(ScreenFade::FADEOUT, 1024);
                    goto_id = 1;

                    //parentMenu->goto_id = 4;
                    /*
                    parentMenu->barracks_menu.Show();
                    parentMenu->barracks_menu.isActive = true;
                    parentMenu->barracks_menu.obelisk = true;
                    parentMenu->barracks_menu.missionID = missions[sel_mission].mis_ID;
                    parentMenu->barracks_menu.mission_file = missions[sel_mission].mission_file;

                    if(CoreManager::getInstance().getSaveReader()->missionLevels[missions[sel_mission].mis_ID] != 0)
                    parentMenu->barracks_menu.mission_multiplier = 0.85 + CoreManager::getInstance().getSaveReader()->missionLevels[missions[sel_mission].mis_ID]*0.15;
                    else
                    parentMenu->barracks_menu.mission_multiplier = 1;

                    parentMenu->barracks_menu.ReloadInventory();
                    parentMenu->barracks_menu.UpdateInputControls();*/
                    SPDLOG_INFO("Set barracks mission to ID {}, mission file: {}", missions[sel_mission].mis_ID, missions[sel_mission].mission_file);
                }
            } else if (inputCtrl->isKeyPressed(InputController::Keys::LTRIGGER))
            {
                SPDLOG_DEBUG("Skipping maps to the left (Q key).");

                mapXdest += float(123) * 6;

                if (mapXdest >= 0)
                    mapXdest = 0;
            } else if (inputCtrl->isKeyPressed(InputController::Keys::RTRIGGER))
            {
                SPDLOG_DEBUG("Skipping maps to the right (E key).");

                mapXdest -= float(123) * 6;

                int maxBound = (worldmap_fields.size() * 123 - 1012) * (-1);

                if (mapXdest <= maxBound)
                    mapXdest = maxBound;
            } else if (inputCtrl->isKeyPressed(InputController::Keys::LEFT))
            {
                if (!displayMissions)
                {
                    //mapXdest += float(123);

                    //if(mapXdest >= 0)
                    //mapXdest = 0;

                    sel_prevlocation = sel_location;
                    sel_location--;
                    sel_mission = 0;

                    if (sel_location <= 1)
                        sel_location = 1;

                    SPDLOG_DEBUG("Selecting Obelisk location {}", sel_location);

                    //if((sel_location*123 + mapX - 62) < 0)
                    //{
                    mapXdest = (sel_location * 123 - 615) * (-1);

                    if (mapXdest >= 0)
                        mapXdest = 0;
                    //}

                    if (sel_location - 1 < location_bgs.size())
                    {
                        alphaA = 255;
                        alphaB = 0;

                        location_bg_a_destAlpha = 0;
                        location_bg_b_destAlpha = 255;
                    }

                    if (std::find(unlocked.begin(), unlocked.end(), sel_location - 1) != unlocked.end())
                    {
                        string L1 = "worldmap_location_" + to_string(sel_location) + "_title";
                        string L2 = "worldmap_location_" + to_string(sel_location) + "_description";
                        string wL1 = string(L1.begin(), L1.end());
                        string wL2 = string(L2.begin(), L2.end());

                        string desc = Func::wrap_text(CoreManager::getInstance().getStrRepo()->GetString(wL2), 800, font, 18);

                        location_title.setString(Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString(wL1)));
                        location_desc.setString(Func::ConvertToUtf8String(desc));
                    } else
                    {
                        string L1 = "worldmap_location_locked";
                        string wL1 = string(L1.begin(), L1.end());

                        location_title.setString(Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString(wL1)));
                        location_desc.setString("");
                    }
                }
            } else if (inputCtrl->isKeyPressed(InputController::Keys::RIGHT))
            {
                if (!displayMissions)
                {
                    //mapXdest -= float(123);

                    //int maxBound = (worldmap_fields.size()*123 - 1012) * (-1);

                    //if(mapXdest <= maxBound)
                    //mapXdest = maxBound;

                    sel_prevlocation = sel_location;
                    sel_location++;
                    sel_mission = 0;

                    if (sel_location >= worldmap_fields.size())
                        sel_location = worldmap_fields.size();

                    SPDLOG_DEBUG("Selecting Obelisk location {}", sel_location);

                    //if((sel_location*123 + mapX - 62 + 176 + 246) > 1012)
                    //{
                    //mapXdest -= float(123);
                    mapXdest = (sel_location * 123 - 615) * (-1);

                    int maxBound = (worldmap_fields.size() * 123 - 1012) * (-1);

                    if (mapXdest <= maxBound)
                        mapXdest = maxBound;
                    //}

                    if (sel_location - 1 < location_bgs.size())
                    {
                        alphaA = 255;
                        alphaB = 0;

                        location_bg_a_destAlpha = 0;
                        location_bg_b_destAlpha = 255;
                    }

                    if (std::find(unlocked.begin(), unlocked.end(), sel_location - 1) != unlocked.end())
                    {
                        string L1 = "worldmap_location_" + to_string(sel_location) + "_title";
                        string L2 = "worldmap_location_" + to_string(sel_location) + "_description";
                        string wL1 = string(L1.begin(), L1.end());
                        string wL2 = string(L2.begin(), L2.end());

                        string desc = Func::wrap_text(CoreManager::getInstance().getStrRepo()->GetString(wL2), 800, font, 18);

                        location_title.setString(Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString(wL1)));
                        location_desc.setString(Func::ConvertToUtf8String(desc));
                    } else
                    {
                        string L1 = "worldmap_location_locked";
                        string wL1 = string(L1.begin(), L1.end());

                        location_title.setString(Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString(wL1)));
                        location_desc.setString("");
                    }
                }
            } else if (inputCtrl->isKeyPressed(InputController::Keys::UP))
            {
                if (displayMissions)
                {
                    if (sel_mission > 0)
                        sel_mission--;

                    SPDLOG_DEBUG("Selecting Obelisk mission {}", sel_mission);

                    string level = "";

                    if (CoreManager::getInstance().getSaveReader()->mission_levels[missions[sel_mission].mis_ID] != 0)
                    {
                        level = to_string(CoreManager::getInstance().getSaveReader()->mission_levels[missions[sel_mission].mis_ID]);
                    }

                    mission_title.setString(Func::ConvertToUtf8String(missions[sel_mission].title) + level);
                    string desc = Func::wrap_text(missions[sel_mission].desc, 633, font, 18);
                    mission_desc.setString(Func::ConvertToUtf8String(desc));
                }
            } else if (inputCtrl->isKeyPressed(InputController::Keys::DOWN))
            {
                if (displayMissions)
                {
                    if (sel_mission < missions.size() - 1)
                        sel_mission++;

                    SPDLOG_DEBUG("Selecting Obelisk mission {}", sel_mission);

                    string level = "";

                    if (CoreManager::getInstance().getSaveReader()->mission_levels[missions[sel_mission].mis_ID] != 0)
                    {
                        level = to_string(CoreManager::getInstance().getSaveReader()->mission_levels[missions[sel_mission].mis_ID]);
                    }

                    mission_title.setString(Func::ConvertToUtf8String(missions[sel_mission].title) + level);
                    string desc = Func::wrap_text(missions[sel_mission].desc, 633, font, 18);
                    mission_desc.setString(Func::ConvertToUtf8String(desc));
                }
            }
        }

        ctrlTips.x = 0;
        ctrlTips.y = (720 - ctrlTips.ySize);
        // TO-DO: fix ctrltips to not use window
        //ctrlTips.draw(window);

        screenFade.draw();

        if (screenFade.checkFinished())
        {
            switch (goto_id)
            {
                case 0: {
                    StateManager::getInstance().setState(StateManager::PATAPOLIS);
                    break;
                }

                case 1: {
                    StateManager::getInstance().setState(StateManager::BARRACKS);
                    break;
                }
            }

            goto_id = -1;
        }
    }
}

void ObeliskMenu::UpdateButtons()
{
}

void ObeliskMenu::OnExit()
{
}

ObeliskMenu::~ObeliskMenu()
{
    //dtor
}
