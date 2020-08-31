#include "Obelisk.h"
#include "ButtonList.h"
#include "iostream"
#include "../V4Core.h"
#include "math.h"
#include <sstream>
#include <string>

ObeliskMenu::ObeliskMenu()
{
    isActive=false;
}

void ObeliskMenu::addMission(string missiondata)
{
    vector<string> mission = Func::Split(missiondata, '|');

    Mission tmp;
    tmp.mis_ID = atoi(mission[0].c_str());
    tmp.loc_ID = atoi(mission[1].c_str());

    wstring title_key = wstring(mission[2].begin(), mission[2].end());
    wstring desc_key = wstring(mission[3].begin(), mission[3].end());

    tmp.title = thisConfig->strRepo.GetUnicodeString(title_key);
    tmp.desc = thisConfig->strRepo.GetUnicodeString(desc_key);

    tmp.mission_file = mission[4];

    PText tm;
    tm.createText(font, 18, sf::Color::Black, "", quality, 1);
    tm.setString(Func::ConvertToUtf8String(tmp.title));
    tm.setOrigin(tm.getLocalBounds().width/2, tm.getLocalBounds().height/2);
    tmp.p_mis = tm;

    cout << "[WorldMap] Making text " << Func::ConvertToUtf8String(tmp.title).toAnsiString() << " " << endl;

    missions.push_back(tmp);
}

void ObeliskMenu::Initialise(Config *thisConfigs,V4Core *parent, PatapolisMenu *curParentMenu){
    parent->SaveToDebugLog("Initializing Obelisk...");

    Scene::Initialise(thisConfigs,parent);
    parentMenu = curParentMenu;

    font.loadFromFile("resources/fonts/p4kakupop-pro.ttf");

    quality = thisConfig->GetInt("textureQuality");

    float ratioX, ratioY;

    switch(quality)
    {
        case 0: ///low
        {
            ratioX = thisConfigs->GetInt("resX") / float(640);
            ratioY = thisConfigs->GetInt("resY") / float(360);
            break;
        }

        case 1: ///med
        {
            ratioX = thisConfigs->GetInt("resX") / float(1280);
            ratioY = thisConfigs->GetInt("resY") / float(720);
            break;
        }

        case 2: ///high
        {
            ratioX = thisConfigs->GetInt("resX") / float(1920);
            ratioY = thisConfigs->GetInt("resY") / float(1080);
            break;
        }

        case 3: ///ultra
        {
            ratioX = thisConfigs->GetInt("resX") / float(3840);
            ratioY = thisConfigs->GetInt("resY") / float(2160);
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
    mainbox.loadFromFile("resources/graphics/ui/worldmap/main_box.png", quality, 1);
    mainbox.setOrigin(mainbox.getLocalBounds().width/2, mainbox.getLocalBounds().height/2);
    mainbox.setPosition(640, 320);
    descbox.loadFromFile("resources/graphics/ui/worldmap/description_box.png", quality, 1);
    descbox.setOrigin(descbox.getLocalBounds().width/2, descbox.getLocalBounds().height/2);
    descbox.setPosition(820, 542);
    iconbox.loadFromFile("resources/graphics/ui/worldmap/icon_box.png", quality, 1);
    missionbox.loadFromFile("resources/graphics/ui/worldmap/mission_box.png", quality, 1);
    missionbox.setOrigin(missionbox.getLocalBounds().width/2, missionbox.getLocalBounds().height/2);
    missionbox.setPosition(290, 542);
    missionselect.loadFromFile("resources/graphics/ui/worldmap/mission_select.png", quality, 1);

    worldmap_title.createText(font, 34, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"worldmap_header_1")), quality, 1);
    location_title.createText(font, 27, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"worldmap_location_1_title")), quality, 1);
    string desc = Func::wrap_text(thisConfig->strRepo.GetString(L"worldmap_location_1_description"), 800, font, 18);

    location_desc.createText(font, 17, sf::Color::Black, desc, quality, 1);
    select_quest.createText(font, 18, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"worldmap_select")), quality, 1);
    mission_title.createText(font, 18, sf::Color::Black, "Hunting Kacheek", quality, 1);
    mission_desc.createText(font, 18, sf::Color::Black, "(no translation needed)", quality, 1);

    float resRatioX = thisConfigs->GetInt("resX") / float(1280);
    float resRatioY = thisConfigs->GetInt("resY") / float(720);

    unavailable.loadFromFile("resources/graphics/ui/worldmap/unavailable.png", quality, 1);
    location_highlight.loadFromFile("resources/graphics/ui/worldmap/location_highlight.png", quality, 1);
    location_highlight.setOrigin(location_highlight.getLocalBounds().width/2, location_highlight.getLocalBounds().height/2);

    cout << "[WorldMap] Creating render_map " << 1012*resRatioX << "x" << 162*resRatioY << endl;
    if(!render_map.create(1012*resRatioX, 162*resRatioY))
    {
        cout << "[WorldMap] Failed to create render_map!" << endl;
        thisConfigs->thisCore->SaveToDebugLog("[WorldMap] Failed to create render_map!");
    }

    cout << "[WorldMap] Creating render_map " << 280*resRatioX << "x" << 120*resRatioY << endl;
    if(!render_missions_map.create(280*resRatioX, 120*resRatioY))
    {
        cout << "[WorldMap] Failed to create render_map!" << endl;
        thisConfigs->thisCore->SaveToDebugLog("[WorldMap] Failed to create render_missions_map!");
    }

    mission_select.loadFromFile("resources/graphics/ui/worldmap/mission_select.png", quality, 1);

    ctrlTips.create(66, font, 20, sf::String(L"Left/Right: Select field      ×: View missions      〇: Exit to Patapolis"), quality);

    parent->SaveToDebugLog("Initializing Obelisk finished.");
}

void ObeliskMenu::Reload()
{
    location_bgs.clear();
    location_icons.clear();

    worldmap_fields.clear();
    worldmap_icons.clear();

    ///Access the save data
    field_unlocked = v4core->savereader.locationsUnlocked;
    missions_unlocked = v4core->savereader.missionsUnlocked;

    PSprite fld;
    fld.loadFromFile("resources/graphics/ui/worldmap/location_field.png", quality, 1);

    ///worldmap contents
    for(int i=1; i<=field_unlocked; i++)
    {
        PSprite bg;
        bg.loadFromFile("resources/graphics/ui/worldmap/locationbg_"+to_string(i)+".png", quality, 1);
        PSprite loc;
        loc.loadFromFile("resources/graphics/ui/worldmap/location_"+to_string(i)+".png", quality, 1);

        location_bgs.push_back(bg);
        location_icons.push_back(loc);
    }

    for(int i=1; i<=20; i++)
    {
        worldmap_fields.push_back(fld);

        if(i <= location_icons.size())
        {
            worldmap_icons.push_back(location_icons[i-1]);
        }
        else
        {
            worldmap_icons.push_back(dullpon);
        }
    }

    location_bg_a = location_bgs[cur_location];
    location_bg_b = location_bgs[cur_location];

    unlocked.clear();

    for(int i=0; i<field_unlocked; i++)
    unlocked.push_back(i);

    //vector<int> unlocked = {0};
}

void ObeliskMenu::EventFired(sf::Event event)
{
    if(isActive)
    {
        if(event.type == sf::Event::KeyPressed)
        {

        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {

        }
    }
}

void ObeliskMenu::Update(sf::RenderWindow &window, float fps, InputController& inputCtrl)
{
    if(isActive)
    {
        //cout << mapX << " " << mapXdest << " " << (sel_location*123) << " " << (sel_location*123) + 176 << endl;
        if(!runonce)
        {
            cout << "[DEBUG] Obelisk is currently open and active." << endl;
            thisConfig->thisCore->SaveToDebugLog("[DEBUG] Obelisk is currently open and active.");
        }

        render_map.clear(sf::Color::Transparent);

        if(round(mapX) != mapXdest)
        {
            float speed = abs(mapX - mapXdest) * 10;

            if(mapX > mapXdest)
            mapX -= speed / fps;
            else
            mapX += speed / fps;
        }

        if(mapX >= 0)
        mapX = 0;

        int maxBound = (worldmap_fields.size()*123 - 1012) * (-1);

        if(mapX <= maxBound)
        mapX = maxBound;

        for(int i=0; i<worldmap_fields.size(); i++)
        {
            worldmap_fields[i].setPosition(i*123 + mapX, 0);
            worldmap_fields[i].update(window);

            render_map.draw(worldmap_fields[i].s);
        }

        location_highlight.setOrigin(location_highlight.getLocalBounds().width/2, location_highlight.getLocalBounds().height/2);
        location_highlight.setPosition(sel_location*123 + mapX - 62, 78);
        location_highlight.update(window);
        render_map.draw(location_highlight.s);

        for(int i=0; i<worldmap_fields.size(); i++)
        {
            worldmap_icons[i].setOrigin(worldmap_icons[i].getLocalBounds().width/2, worldmap_icons[i].getLocalBounds().height);
            worldmap_icons[i].setPosition(i*123 + mapX + 61, 155);
            worldmap_icons[i].update(window);

            render_map.draw(worldmap_icons[i].s);

            if(std::find(unlocked.begin(), unlocked.end(), i) == unlocked.end())
            {
                unavailable.setPosition(i*123 + mapX - 1, 0);
                unavailable.update(window);
                render_map.draw(unavailable.s);
            }
        }

        render_map.display();

        if(!displayMissions)
        {
            mainbox_destX = 640;
            mainbox_destY = 320;
        }
        else
        {
            mainbox_destX = 640;
            mainbox_destY = 230;
        }

        if(round(mainbox.lx) != mainbox_destX)
        {
            float speed = abs(mainbox.lx - mainbox_destX) * 10;

            if(mainbox.lx > mainbox_destX)
            mainbox.lx -= speed / fps;
            else
            mainbox.lx += speed / fps;
        }

        if(round(mainbox.ly) != mainbox_destY)
        {
            float speed = abs(mainbox.ly - mainbox_destY) * 10;

            if(mainbox.ly > mainbox_destY)
            mainbox.ly -= speed / fps;
            else
            mainbox.ly += speed / fps;
        }

        if(round(alphaA) != location_bg_a_destAlpha)
        {
            float speed = abs(alphaA - location_bg_a_destAlpha) * 8;

            if(alphaA > location_bg_a_destAlpha)
            alphaA -= speed / fps;
            else
            alphaA += speed / fps;
        }

        alphaB = 255 - alphaA;

        location_bg_a.setColor(sf::Color(255,255,255,255));
        location_bg_b.setColor(sf::Color(255,255,255,alphaB));

        location_bg_a.draw(window);
        location_bg_b.draw(window);

        mainbox.draw(window);

        worldmap_title.setOrigin(worldmap_title.getLocalBounds().width/2, worldmap_title.getLocalBounds().height/2);
        worldmap_title.setPosition(mainbox.getPosition().x-303, mainbox.getPosition().y-174);
        worldmap_title.draw(window);

        location_title.setOrigin(location_title.getLocalBounds().width/2, location_title.getLocalBounds().height/2);
        location_title.setPosition(mainbox.getPosition().x, mainbox.getPosition().y+72);
        location_title.draw(window);

        location_desc.setOrigin(location_desc.getLocalBounds().width/2, location_desc.getLocalBounds().height/2);
        location_desc.setPosition(mainbox.getPosition().x, mainbox.getPosition().y+142);
        location_desc.draw(window);

        float resRatioX = window.getSize().x / float(1280);
        float resRatioY = window.getSize().y / float(720);

        spr_render_map.setTexture(render_map.getTexture(), true);
        spr_render_map.setPosition(sf::Vector2f(134*resRatioX, (mainbox.getPosition().y - 115) * resRatioY));
        window.draw(spr_render_map);

        if(displayMissions)
        {
            missionbox.draw(window);
            descbox.draw(window);

            select_quest.setOrigin(0, select_quest.getLocalBounds().height/2);
            select_quest.setPosition(missionbox.getPosition().x - 141, missionbox.getPosition().y - 74);
            select_quest.draw(window);

            mission_title.setOrigin(0, mission_title.getLocalBounds().height/2);
            mission_title.setPosition(descbox.getPosition().x - 315, descbox.getPosition().y - 74);
            mission_title.draw(window);

            mission_desc.setOrigin(0, 0);
            mission_desc.setPosition(descbox.getPosition().x - 315, descbox.getPosition().y - 55);
            mission_desc.draw(window);

            render_missions_map.clear(sf::Color::Transparent);

            for(int i=0; i<missions.size(); i++)
            {
                missions[i].p_mis.setPosition(0, i*24);
                missions[i].p_mis.update(window);
                render_missions_map.draw(missions[i].p_mis.t);
            }

            render_missions_map.display();

            mission_select.setPosition(135, missionbox.getPosition().y - 56 + (sel_mission*24));
            mission_select.draw(window);

            spr_render_missions_map.setTexture(render_missions_map.getTexture(), true);
            spr_render_missions_map.setPosition(sf::Vector2f(143*resRatioX, (missionbox.getPosition().y - 53) * resRatioY));
            window.draw(spr_render_missions_map);
        }

        if(inputCtrl.isKeyPressed(InputController::Keys::CIRCLE))
        {
            if(displayMissions)
            {
                ctrlTips.create(66, font, 20, sf::String(L"Left/Right: Select field      ×: View missions      〇: Exit to Patapolis"), quality);

                displayMissions = false;
                thisConfig->thisCore->SaveToDebugLog("Exited mission selection.");
            }
            else
            {
                this->Hide();
                this->isActive = false;
                thisConfig->thisCore->SaveToDebugLog("Exited Obelisk.");
            }
        }
        else if(inputCtrl.isKeyPressed(InputController::Keys::CROSS))
        {
            if(!displayMissions)
            {
                thisConfig->thisCore->SaveToDebugLog("Displaying missions on Worldmap for location "+to_string(sel_location)+".");

                ///(re)load missions here
                cout << "Displaying missions" << endl;

                missions.clear();

                ifstream wmap("resources/missions/worldmap.dat");
                string buff;

                while(getline(wmap, buff))
                {
                    cout << "[WorldMap] Read: " << buff << endl;

                    if(buff.find("#") == std::string::npos)
                    {
                        vector<string> mission = Func::Split(buff, '|');

                        cout << "[WorldMap] Checking " << atoi(mission[1].c_str()) << " vs " << sel_location << endl;
                        if(atoi(mission[1].c_str()) == sel_location)
                        {
                            if(std::find(missions_unlocked.begin(), missions_unlocked.end(), atoi(mission[0].c_str())) != missions_unlocked.end())
                            {
                            cout << "Mission in location " << sel_location << " detected with ID " << mission[0] << endl;
                            addMission(buff);
                            }
                        }
                    }
                }

                wmap.close();

                if(missions.size() > 0)
                {
                    ctrlTips.create(66, font, 20, sf::String(L"Up/Down: Select mission      ×: Enter mission      〇: Return to field select"), quality);

                    displayMissions = true;

                    mission_title.setString(Func::ConvertToUtf8String(missions[sel_mission].title));
                    string desc = Func::wrap_text(Func::ConvertToUtf8String(missions[sel_mission].desc), 633, font, 18);
                    mission_desc.setString(desc);
                }
            }
            else
            {
                parentMenu->barracks_menu.Show();
                parentMenu->barracks_menu.isActive = true;
                parentMenu->barracks_menu.obelisk = true;
                parentMenu->barracks_menu.missionID = missions[sel_mission].mis_ID;
                parentMenu->barracks_menu.mission_file = missions[sel_mission].mission_file;
                parentMenu->barracks_menu.OpenBarracksMenu();
                parentMenu->barracks_menu.UpdateInputControls();
                cout << "Set barracks mission to ID " << missions[sel_mission].mis_ID << endl;
                thisConfig->thisCore->SaveToDebugLog("Barracks (In Obelisk) entered. Mission file: "+missions[sel_mission].mission_file);
            }
        }
        else if(inputCtrl.isKeyPressed(InputController::Keys::LTRIGGER))
        {
            thisConfig->thisCore->SaveToDebugLog("Skipping maps to the left (Q key).");

            mapXdest += float(123) * 6;

            if(mapXdest >= 0)
            mapXdest = 0;
        }
        else if(inputCtrl.isKeyPressed(InputController::Keys::RTRIGGER))
        {
            thisConfig->thisCore->SaveToDebugLog("Skipping maps to the right (E key).");

            mapXdest -= float(123) * 6;

            int maxBound = (worldmap_fields.size()*123 - 1012) * (-1);

            if(mapXdest <= maxBound)
            mapXdest = maxBound;
        }
        else if(inputCtrl.isKeyPressed(InputController::Keys::LEFT))
        {
            if(!displayMissions)
            {
                //mapXdest += float(123);

                //if(mapXdest >= 0)
                //mapXdest = 0;

                sel_location--;

                if(sel_location <= 1)
                sel_location = 1;

                thisConfig->thisCore->SaveToDebugLog("Selecting Obelisk location "+to_string(sel_location)+".");

                //if((sel_location*123 + mapX - 62) < 0)
                //{
                mapXdest = (sel_location*123 - 615) * (-1);

                if(mapXdest >= 0)
                mapXdest = 0;
                //}

                if(sel_location-1 < location_bgs.size())
                {
                    location_bg_a = location_bg_b;
                    location_bg_b = location_bgs[sel_location-1];

                    alphaA = 255;
                    alphaB = 0;

                    location_bg_a_destAlpha = 0;
                    location_bg_b_destAlpha = 255;
                }

                if(std::find(unlocked.begin(), unlocked.end(), sel_location-1) != unlocked.end())
                {
                    string L1 = "worldmap_location_"+to_string(sel_location)+"_title";
                    string L2 = "worldmap_location_"+to_string(sel_location)+"_description";
                    wstring wL1 = wstring(L1.begin(), L1.end());
                    wstring wL2 = wstring(L2.begin(), L2.end());

                    string desc = Func::wrap_text(thisConfig->strRepo.GetString(wL2), 800, font, 18);

                    location_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(wL1)));
                    location_desc.setString(desc);
                }
                else
                {
                    string L1 = "worldmap_location_locked";
                    wstring wL1 = wstring(L1.begin(), L1.end());

                    location_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(wL1)));
                    location_desc.setString("");
                }
            }
        }
        else if(inputCtrl.isKeyPressed(InputController::Keys::RIGHT))
        {
            if(!displayMissions)
            {
                //mapXdest -= float(123);

                //int maxBound = (worldmap_fields.size()*123 - 1012) * (-1);

                //if(mapXdest <= maxBound)
                //mapXdest = maxBound;

                sel_location++;

                if(sel_location >= worldmap_fields.size())
                sel_location = worldmap_fields.size();

                thisConfig->thisCore->SaveToDebugLog("Selecting Obelisk location "+to_string(sel_location)+".");

                //if((sel_location*123 + mapX - 62 + 176 + 246) > 1012)
                //{
                //mapXdest -= float(123);
                mapXdest = (sel_location*123 - 615) * (-1);

                int maxBound = (worldmap_fields.size()*123 - 1012) * (-1);

                if(mapXdest <= maxBound)
                mapXdest = maxBound;
                //}

                if(sel_location-1 < location_bgs.size())
                {
                    location_bg_a = location_bg_b;
                    location_bg_b = location_bgs[sel_location-1];

                    alphaA = 255;
                    alphaB = 0;

                    location_bg_a_destAlpha = 0;
                    location_bg_b_destAlpha = 255;
                }

                if(std::find(unlocked.begin(), unlocked.end(), sel_location-1) != unlocked.end())
                {
                    string L1 = "worldmap_location_"+to_string(sel_location)+"_title";
                    string L2 = "worldmap_location_"+to_string(sel_location)+"_description";
                    wstring wL1 = wstring(L1.begin(), L1.end());
                    wstring wL2 = wstring(L2.begin(), L2.end());

                    string desc = Func::wrap_text(thisConfig->strRepo.GetString(wL2), 800, font, 18);

                    location_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(wL1)));
                    location_desc.setString(desc);
                }
                else
                {
                    string L1 = "worldmap_location_locked";
                    wstring wL1 = wstring(L1.begin(), L1.end());

                    location_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(wL1)));
                    location_desc.setString("");
                }
            }
        }
        else if(inputCtrl.isKeyPressed(InputController::Keys::UP))
        {
            if(displayMissions)
            {
                if(sel_mission > 0)
                sel_mission--;

                thisConfig->thisCore->SaveToDebugLog("Selecting Obelisk mission "+to_string(sel_mission)+".");

                mission_title.setString(Func::ConvertToUtf8String(missions[sel_mission].title));
                string desc = Func::wrap_text(Func::ConvertToUtf8String(missions[sel_mission].desc), 633, font, 18);
                mission_desc.setString(desc);
            }
        }
        else if(inputCtrl.isKeyPressed(InputController::Keys::DOWN))
        {
            if(displayMissions)
            {
                if(sel_mission < missions.size()-1)
                sel_mission++;

                thisConfig->thisCore->SaveToDebugLog("Selecting Obelisk mission "+to_string(sel_mission)+".");

                mission_title.setString(Func::ConvertToUtf8String(missions[sel_mission].title));
                string desc = Func::wrap_text(Func::ConvertToUtf8String(missions[sel_mission].desc), 633, font, 18);
                mission_desc.setString(desc);
            }
        }

        ctrlTips.x = 0;
        ctrlTips.y = (720-ctrlTips.ySize);
        ctrlTips.draw(window);
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
