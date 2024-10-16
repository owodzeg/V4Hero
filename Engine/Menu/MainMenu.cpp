#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "MainMenu.h"
#include "../CoreManager.h"
#include "../StateManager.h"

#include "../V4Core.h"
#include <iostream>
#include <fstream>
#include <Graphics/ConcaveShape.h>
#include <Graphics/CurveSegment.h>
#include <Graphics/CurveShape.h>


MainMenu::MainMenu()
{
    //ctor
    is_active = true;

    Config* config = CoreManager::getInstance().getConfig();
    StringRepository* strRepo = CoreManager::getInstance().getStrRepo();

    SPDLOG_DEBUG("Initializing main menu...");

    int q = config->GetInt("textureQuality");
    int r = 1;

    quality = q;

    float resRatioX = config->GetInt("resX") / float(3840);
    float resRatioY = config->GetInt("resY") / float(2160);

    rs_cover.setSize(sf::Vector2f(config->GetInt("resX"), config->GetInt("resY")));
    rs_cover.setFillColor(sf::Color(0, 0, 0, 255));
    rs_cover2.setSize(sf::Vector2f(config->GetInt("resX"), config->GetInt("resY")));
    rs_cover2.setFillColor(sf::Color(0, 0, 0, 0));

    logow_bg.load("resources/graphics/ui/menu/logowbg.png");
    logow_text.load("resources/graphics/ui/menu/logowtxt.png");
    logow_shadow.load("resources/graphics/ui/menu/logowsh.png");

    logow_bg.setColor(sf::Color(120, 0, 0, ui_alpha));
    logow_shadow.setColor(sf::Color(64, 64, 64, ui_alpha));
    logow_text.setColor(sf::Color(255, 255, 255, ui_alpha));

    std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    t_pressanykey.setFont(font);
    t_pressanykey.setCharacterSize(26);
    t_pressanykey.setColor(sf::Color(255, 255, 255, t_alpha));
    t_pressanykey.setStringKey("menu_pressanykey");


    sb_smash.loadFromFile("resources/sfx/menu/smash.ogg");
    s_smash.setBuffer(sb_smash);
    s_smash.setVolume(float(config->GetInt("masterVolume")) * (float(config->GetInt("sfxVolume")) / 100.f));

    for (int g = 0; g < 4; g++)
    {
        grass[g].load("resources/graphics/ui/menu/grass_" + to_string(g+1) + ".png");
        grass[g].setScale(1.05, 1.05);
        grass[g].setOrigin(grass[g].getGlobalBounds().width / float(100), grass[g].getGlobalBounds().height);
    }

    logo.load("resources/graphics/ui/menu/logo.png");
    logo_shadow.load("resources/graphics/ui/menu/logo_shadow.png");

    logo.setOrigin(logo.getGlobalBounds().width / 2, logo.getGlobalBounds().height / 2);
    logo_shadow.setOrigin(logo_shadow.getGlobalBounds().width / 2, logo_shadow.getGlobalBounds().height / 2);

    for (int t = 0; t < 4; t++)
    {
        totem[t].load("resources/graphics/ui/menu/totem_" + to_string(t+1) + ".png");
        totem[t].setOrigin(0, totem[t].getGlobalBounds().height);
    }

    for (int f = 0; f < 3; f++)
    {
        fire[f].load("resources/graphics/ui/menu/fire_" + to_string(f+1) + ".png");
        fire[f].setOrigin(fire[f].getGlobalBounds().width / 2, fire[f].getGlobalBounds().height);
    }

    for (int s = 0; s < 2; s++)
    {
        sword[s].load("resources/graphics/ui/menu/sword.png");
        sword[s].setOrigin(sword[s].getGlobalBounds().width / 2, sword[s].getGlobalBounds().height);
    }

    aura.load("resources/graphics/ui/menu/aura.png");
    aura.setOrigin(aura.getGlobalBounds().width / 2, aura.getGlobalBounds().height / 2);

    for (int i = 0; i <= 3; i++)
    {
        t_option[i].setFont(font);
        t_option[i].setCharacterSize(24);
        t_option[i].setColor(sf::Color(255, 255, 255));
        t_option[i].setString("");
    }

    string vx_params = "0,135,38,23;240,135,38,23;2040,205,107,132;-1,205,107,132";

    vector<string> v_vxparams = Func::Split(vx_params, ';');
    std::vector<sf::Vector2f> vx_pos;
    std::vector<sf::Color> vx_color;

    for (int i = 0; i < v_vxparams.size(); i++)
    {
        vector<string> tmp = Func::Split(v_vxparams[i], ',');

        sf::Vector2f tmp_vector;
        sf::Color tmp_color;

        tmp_vector.x = 0;
        tmp_vector.y = atof(tmp[0].c_str()) * resRatioY;

        if (tmp[0] == "-1")
        {
            tmp_vector.y = 2160 * resRatioY;
        }

        tmp_color.r = atoi(tmp[1].c_str());
        tmp_color.g = atoi(tmp[2].c_str());
        tmp_color.b = atoi(tmp[3].c_str());

        sf::Vector2f tmp_vector2;

        tmp_vector2.x = 3840 * resRatioX;
        tmp_vector2.y = atof(tmp[0].c_str()) * resRatioY;

        if (tmp[0] == "-1")
        {
            tmp_vector2.y = 2160 * resRatioY;
        }

        vx_pos.push_back(tmp_vector);
        vx_color.push_back(tmp_color);

        vx_pos.push_back(tmp_vector2);
        vx_color.push_back(tmp_color);
    }

    sf::VertexArray tmp(sf::TrianglesStrip, vx_pos.size());
    v_background = tmp;

    for (int i = 0; i < vx_pos.size(); i++)
    {
        v_background[i].position = vx_pos[i];
        v_background[i].color = vx_color[i];

        SPDLOG_TRACE("main menu background vx_pos: {} {}", vx_pos[i].x, vx_pos[i].y);
    }

    g_x[0] = 0;
    g_x[1] = 0;
    g_x[2] = 0;
    g_x[3] = 0;

    float volume = (float(config->GetInt("masterVolume")) * (float(config->GetInt("bgmVolume")) / 100.f));

    sb_title_loop.loadFromFile("resources/sfx/menu/menuloop.ogg");
    title_loop.setBuffer(sb_title_loop);
    title_loop.setLoop(true);
    title_loop.setVolume(volume);

    ifstream fr("resources/firstrun");
    if (fr.good())
    {
        firstrun = false;
        premenu = true;
    } else
    {
        SPDLOG_INFO("It's your first time running the game!");
        firstrun = true;
    }

    msgcloud.Create(45, sf::Vector2f(1920, 1440), sf::Color::White, true, config->GetInt("textureQuality"));
    msgcloud.AddDialog("firstrun_dialog_1", true);
    msgcloud.AddDialog("firstrun_dialog_2", true);
    msgcloud.AddDialog("firstrun_dialog_3", true);
    msgcloud.AddDialog("firstrun_dialog_4", true);
    msgcloud.AddDialog("firstrun_dialog_5", true);
    msgcloud.AddDialog("firstrun_dialog_6", true);
    msgcloud.AddDialog("firstrun_dialog_7", true);
    msgcloud.AddDialog("firstrun_dialog_8", true);
    msgcloud.AddDialog("firstrun_dialog_9", true);

    temp_menu.push_back("menu_newgame");
    temp_menu.push_back("menu_continue");
    temp_menu.push_back("menu_options");
    temp_menu.push_back("menu_exit");

    SPDLOG_DEBUG("Main menu initialized.");
    //title_loop.play();
    startClock.restart();
    frClock.restart();

    t_pressanykey.setPosition(1920, 1320);

    initialized = true;
    
    //Checks if there is a save then use "continue" as default totem*/
    ifstream check("resources/data/sv1.p4sv");
    bool exists = check.good();
    check.close();
    if (exists) {
        totem_sel = 1;
    }
}

void MainMenu::EventFired(sf::Event event)
{

}
void MainMenu::SelectMenuOption()
{
    StringRepository* strRepo = CoreManager::getInstance().getStrRepo();
    Config* config = CoreManager::getInstance().getConfig();
    SaveReader* saveReader = CoreManager::getInstance().getSaveReader();
    V4Core* core = CoreManager::getInstance().getCore();

    std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    switch (totem_sel)
    {
        case 0: // load the start game cutscenes and menu
        {
            ifstream check("resources/data/sv1.p4sv");
            bool exists = check.good();
            check.close();

            if (!exists)
            {
                SPDLOG_INFO("There is no save. Start new game!");

                screenFade.Create(1, 512);
                goto_id = 0;
            } else
            {
                SPDLOG_INFO("There is an existing save data. Ask if overwrite");

                std::vector<sf::String> a = {"nav_yes", "nav_no"};

                PataDialogBox db;
                db.Create(font, "menu_saveexists", a, config->GetInt("textureQuality"));
                db.id = 0;
                dialogboxes.push_back(db);
            }

            break;
        }
        case 1: // load save and patapolis
        {
            ifstream check("resources/data/sv1.p4sv");
            bool exists = check.good();
            check.close();

            if (exists)
            {
                /** Load save from saveReader **/
                saveReader->Flush();
                saveReader->LoadSave();

                if (saveReader->save_ver != "2.0")
                {
                    SPDLOG_WARN("Outdated save data!");

                    std::vector<sf::String> a = {"nav_understood"};

                    PataDialogBox db;
                    db.Create(font, "menu_nosupportdata", a, config->GetInt("textureQuality"));
                    db.id = 2;
                    dialogboxes.push_back(db);
                } else
                {
                    screenFade.Create(1, 512);
                    goto_id = 1;
                }
            } else
            {
                SPDLOG_WARN("There is no savedata.");

                std::vector<sf::String> a = {"nav_understood"};

                PataDialogBox db;
                db.Create(font, "menu_nodata", a, config->GetInt("textureQuality"));
                db.id = 1;
                dialogboxes.push_back(db);
            }

            break;
        }
        case 2: {
            // load the options menu
            screenFade.Create(1, 512);
            goto_id = 2;
            break;
        }
        case 3: {
            // quit the game probably
            core->close_window = true;
            break;
        }
    }
}
void MainMenu::Update()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    InputController* inputCtrl = CoreManager::getInstance().getInputController();
    MouseController* mouseCtrl = CoreManager::getInstance().getMouseController();

    float fps = CoreManager::getInstance().getCore()->getFPS();

    if (firstrun)
    {
        if (frClock.getElapsedTime().asSeconds() > 2)
        {
            if (!msgcloud.done)
            {
                if (msgcloud.firstrender)
                    msgcloud.Show();
            }
        }

        if (msgcloud.done)
        {
            if (!premenu)
                frwaitClock.restart();

            premenu = true;

            startClock.restart();
        }

        if (premenu)
        {
            if (frwaitClock.getElapsedTime().asSeconds() > 1)
            {
                firstrun = false;

                std::ofstream fr("resources/firstrun", ios::trunc);
                fr.close();
            }
        }

        msgcloud.Draw();

    } else if (premenu)
    {
        if (startClock.getElapsedTime().asSeconds() > 2)
        {
            if (!keypressed)
            {
                if (ui_alpha < 255)
                {
                    ui_alpha += 255.0 / fps;
                }

                if (ui_alpha >= 255)
                {
                    ui_alpha = 255;
                }

                if (t_alpha < 128)
                {
                    t_alpha += 128.0 / fps;
                }

                if (t_alpha >= 128)
                {
                    t_alpha = 128;
                }

                logow_bg.setColor(sf::Color(120, 0, 0, ui_alpha));
                logow_text.setColor(sf::Color(255, 255, 255, ui_alpha));
                logow_shadow.setColor(sf::Color(64, 0, 0, ui_alpha));

                if (inputCtrl->isAnyKeyPressed())
                {
                    s_smash.play();
                    logow_bg.setColor(sf::Color(200, 0, 0, 255));
                    logow_shadow.setColor(sf::Color(200, 0, 0, 255));
                    logow_text.setColor(sf::Color(255, 255, 255, 255));
                    ui_alpha = 255;
                    logow_scale = 1.2;
                    logow_shscale = 1.2;
                    dest_y = 1080;
                    keypressed = true;
                    t_pressanykey.setPosition(1920, 13200);
                    menuClock.restart();
                }
            }

            logow_bg.setOrigin(logow_bg.getGlobalBounds().width / 2, logow_bg.getGlobalBounds().height / 2);
            logow_text.setOrigin(logow_text.getGlobalBounds().width / 2, logow_text.getGlobalBounds().height / 2);
            logow_shadow.setOrigin(logow_shadow.getGlobalBounds().width / 2, logow_shadow.getGlobalBounds().height / 2);

            if (dest_y > cur_y)
            {
                cur_y += abs(dest_y - cur_y) * 2 / fps;
            }
            if (dest_y < cur_y)
            {
                cur_y -= abs(dest_y - cur_y) * 2 / fps;
            }

            logow_bg.setPosition(1920, cur_y);
            logow_text.setPosition(logow_bg.getPosition().x + 3, logow_bg.getPosition().y - 9);
            logow_shadow.setPosition(logow_bg.getPosition().x, logow_bg.getPosition().y);

            if (logow_scale > 1)
                logow_scale -= 0.5 / fps;
            if (logow_shscale > 1)
                logow_shscale -= 0.5 / fps;

            if (logow_scale <= 1)
                logow_scale = 1;
            if (logow_shscale <= 1)
                logow_shscale = 1;

            logow_bg.setScale(logow_scale);
            logow_text.setScale(logow_scale);
            logow_shadow.setScale(logow_shscale);

            window->draw(rs_cover);

            logow_shadow.draw();
            logow_bg.draw();
            logow_text.draw();

            if (keypressed)
            {
                t_alpha -= 255.0 / fps;

                if (t_alpha <= 0)
                    t_alpha = 0;

                if (menuClock.getElapsedTime().asSeconds() > 3)
                {
                    cv_alpha += 255.0 / fps;

                    if (cv_alpha >= 255)
                        cv_alpha = 255;
                }

                if (menuClock.getElapsedTime().asSeconds() > 4.5)
                {
                    premenu = false;
                }
            }

            t_pressanykey.setOrigin(t_pressanykey.getLocalBounds().width / 2, t_pressanykey.getLocalBounds().height / 2);
            t_pressanykey.setColor(sf::Color(255, 255, 255, t_alpha));
            t_pressanykey.draw();

            rs_cover2.setFillColor(sf::Color(0, 0, 0, cv_alpha));
            window->draw(rs_cover2);
        }
    } else
    {
        if (title_loop.getStatus() == sf::Sound::Status::Stopped)
        {
            SPDLOG_DEBUG("Playing title_loop");
            title_loop.play();
        }

        window->draw(v_background);

        if (fade == 0)
            alpha -= float(15) / fps;

        if (fade == 1)
            alpha += float(15) / fps;

        if (alpha <= 220)
            fade = 1;

        if (alpha >= 250)
            fade = 0;

        float scale = 1 + ((alpha - 220) / 500);
        float aurascale = 1 + ((alpha - 220)) / 250;

        g_dest[0] = (mouseCtrl->getMousePos().x / 960.f - 6.f) * (-1);
        g_dest[1] = (mouseCtrl->getMousePos().x / 318.f - 18.f) * (-1);
        g_dest[2] = (mouseCtrl->getMousePos().x / 135.f - 42.f) * (-1);
        g_dest[3] = (mouseCtrl->getMousePos().x / 63.f - 90.f) * (-1);

        for (int i = 0; i <= 3; i++)
        {
            if (g_dest[i] < g_x[i])
                g_x[i] -= abs(g_dest[i] - g_x[i]) / 100 / fps * 240;
            if (g_dest[i] > g_x[i])
                g_x[i] += abs(g_dest[i] - g_x[i]) / 100 / fps * 240;
        }


        ///dont make it go off bounds
        if (g_x[0] > 6)
            g_x[0] = 6;

        if (g_x[0] < -6)
            g_x[0] = -6;


        if (g_x[1] > 18)
            g_x[1] = 18;

        if (g_x[1] < -18)
            g_x[1] = -18;


        if (g_x[2] > 42)
            g_x[2] = 42;

        if (g_x[2] < -42)
            g_x[2] = -42;


        if (g_x[3] > 90)
            g_x[3] = 90;

        if (g_x[3] < -90)
            g_x[3] = -90;

        for (int g=0; g<3; g++)
        {
            grass[g].setPosition(g_x[g], g_y[g]);
            grass[g].draw();
        }

        logo_shadow.setPosition(1920, 420);
        logo.setPosition(1920, 420);

        logo_shadow.setColor(sf::Color(255, 255, 255, alpha));
        logo_shadow.setScale(scale);

        logo_shadow.draw();
        logo.draw();

        float fire_shift = 0;

        //cout << "mouseCtrl->getMousePos().x: " << (mouseCtrl->getMousePos().x / window->getSize().x) * 1280 << endl;

        mouseInBounds = false;

        for (int i = 0; i < 4; i++)
        {
            totem[i].setPosition((float(360) + float(918) * (i)) + g_x[3] / 1.4, 2160);

            if (mouseCtrl->getMousePos().x / (window->getSize().x / 3840.f) > totem[i].getPosition().x)
            {
                if (mouseCtrl->getMousePos().x / (window->getSize().x / 3840.f) < (totem[i].getPosition().x + totem[i].getTransformedBounds().width))
                {
                    if (mouseCtrl->getMousePos().y / (window->getSize().y / 2160.f) > totem[i].getPosition().y - totem[i].getTransformedBounds().height)
                    {
                        totem_sel = i;
                        mouseInBounds = true;
                    }
                }
            }
        }

        float fire_x = 0, fire_y = 0;

        switch (totem_sel)
        {
            case 0: {
                fire_shift = float(102);
                fire_x = float(217);
                fire_y = float(960);
                break;
            }
            case 1: {
                fire_shift = float(120);
                fire_x = float(179);
                fire_y = float(1113);
                break;
            }
            case 2: {
                fire_shift = float(18);
                fire_x = float(165);
                fire_y = float(1355);
                break;
            }
            case 3: {
                fire_shift = float(30);
                fire_x = float(179);
                fire_y = float(1496);
                break;
            }
        }

        for (int f = 0; f < 3; f++)
        {
            fire[f].setPosition(totem[totem_sel].getPosition().x + fire_x, fire_y + fire_shift);
        }

        fire[int(floor(fire_count))].draw();

        fire_count += float(14) / fps;
        if (fire_count >= 3)
            fire_count = 0;

        aura.setPosition(fire[0].getPosition().x, fire[0].getPosition().y - (fire[0].getTransformedBounds().height / 2));
        aura.setScale(aurascale);
        aura.draw();

        for (int i = 0; i < 4; i++)
        {
            totem[i].draw();
        }

        grass[3].draw();

        if (old_sel != totem_sel)
        {
            SPDLOG_TRACE("Totem changed to {}", totem_sel);
            t_option[totem_sel].setScale(1.2, 1.2);
        }

        for (int i = 0; i <= 3; i++)
        {
            if (t_option[i].getScale().x > 1)
            {
                float new_scale = t_option[i].getScale().x - float(1) / fps;
                t_option[i].setScale(new_scale, new_scale);
            } else
            {
                t_option[i].setScale(1, 1);
            }

            t_option[i].setStringKey(temp_menu[i]);
            t_option[i].setOrigin(t_option[i].getLocalBounds().width / 2, t_option[i].getLocalBounds().height / 2);

            t_option[i].setPosition(totem[i].getPosition().x + totem[i].getTransformedBounds().width / 2.f, 2160 - totem[i].getTransformedBounds().height - 84);
            
            if (i == totem_sel)
            {
                t_option[i].setPosition(totem[i].getPosition().x + totem[i].getTransformedBounds().width / 2.f, 2160 - totem[i].getTransformedBounds().height - 210);
            }

            t_option[i].setColor(sf::Color(255, 255, 255, 96));
            t_option[totem_sel].setColor(sf::Color::White);

            t_option[i].draw();
        }

        sword[0].setScale(1, 1);
        sword[0].setPosition(t_option[totem_sel].getPosition().x - t_option[totem_sel].getLocalBounds().width / 2 - sword[0].getTransformedBounds().width / 2 - 15, t_option[totem_sel].getPosition().y + 63);
        sword[0].draw();

        sword[1].setScale(-1, 1);
        sword[1].setPosition(t_option[totem_sel].getPosition().x + t_option[totem_sel].getLocalBounds().width / 2 + sword[0].getTransformedBounds().width / 2 + 15, t_option[totem_sel].getPosition().y + 63);
        sword[1].draw();

        old_sel = totem_sel;

        cv_alpha -= 255.0 / fps;

                


        if (cv_alpha <= 0)
            cv_alpha = 0;

        rs_cover2.setFillColor(sf::Color(0, 0, 0, cv_alpha));
        window->draw(rs_cover2);

        window->setView(window->getDefaultView());

        vector<int> db_e; ///dialog box erase

        for (int i = 0; i < dialogboxes.size(); i++)
        {
            dialogboxes[i].x = 1920;
            dialogboxes[i].y = 1080;
            dialogboxes[i].Draw();

            if (dialogboxes[i].closed)
                db_e.push_back(i);
        }

        for (int i = 0; i < db_e.size(); i++)
        {
            dialogboxes.erase(dialogboxes.begin() + db_e[i] - i);
        }

        if (dialogboxes.size() <= 0)
        {
            if ((inputCtrl->isKeyPressed(Input::Keys::LEFT)) || (inputCtrl->isKeyPressed(Input::Keys::LTRIGGER)))
            {
                totem_sel -= 1;
                if (totem_sel < 0)
                    totem_sel = 3;
                old_sel = totem_sel;
            }

            // TEMP TESTING OF BEZIER CURVES
            temp_anim_t += 1;
            //shp->fps = fps;
            //shp->Draw(); 


            if ((inputCtrl->isKeyPressed(Input::Keys::RIGHT)) || (inputCtrl->isKeyPressed(Input::Keys::RTRIGGER)))
            {
                totem_sel += 1;
                if (totem_sel > 3)
                    totem_sel = 0;
                old_sel = totem_sel;
            }

            if (inputCtrl->isKeyPressed(Input::Keys::CROSS) || mouseCtrl->getClick(0))
            {
                SelectMenuOption();
                //title_loop.stop();
            }
        } else
        {
            if (inputCtrl->isKeyPressed(Input::Keys::CROSS))
            {
                switch (dialogboxes[dialogboxes.size() - 1].CheckSelectedOption())
                {
                    case 0: {
                        if (dialogboxes[dialogboxes.size() - 1].id == 0)
                        {
                            SPDLOG_INFO("Starting new game!");
                            dialogboxes[dialogboxes.size() - 1].Close();

                            screenFade.Create(1, 512);
                            goto_id = 0;

                            break;
                        } else if (dialogboxes[dialogboxes.size() - 1].id == 1)
                        {
                            dialogboxes[dialogboxes.size() - 1].Close();
                            break;
                        } else if (dialogboxes[dialogboxes.size() - 1].id == 2)
                        {
                            dialogboxes[dialogboxes.size() - 1].Close();
                            break;
                        }
                    }

                    case 1: {
                        SPDLOG_INFO("Returning to title screen!");
                        dialogboxes[dialogboxes.size() - 1].Close();

                        break;
                    }
                }
            }
        }

        screenFade.draw();

        if (screenFade.checkFinished())
        {
            if (goto_id != -1)
            {
                switch (goto_id)
                {
                    case 0: ///New game
                    {
                        SaveReader* saveReader = CoreManager::getInstance().getSaveReader();

                        saveReader->Flush();
                        saveReader->CreateBlankSave();

                        title_loop.stop();
                        StateManager::getInstance().setState(StateManager::INTRODUCTION);

                        break;
                    }

                    case 1: ///Continue
                    {
                        title_loop.stop();
                        StateManager::getInstance().setState(StateManager::PATAPOLIS);

                        break;
                    }

                    case 2: ///Options
                    {
                        title_loop.stop();
                        StateManager::getInstance().setState(StateManager::OPTIONSMENU);
                        goto_id = -1;
                        break;
                    }
                }
            }
        }
    }
}

MainMenu::~MainMenu()
{
    //dtor
    SPDLOG_DEBUG("MainMenu destructor");
}
