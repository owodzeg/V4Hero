#include "OptionsMenu.h"
#include "ButtonList.h"
#include <iostream>
#include <windows.h>
#include "../V4Core.h"
OptionsMenu::OptionsMenu()
{
    //ctor
    f_font.loadFromFile("resources/fonts/patapon.ttf");
    m_font.loadFromFile("resources/fonts/p4kakupop-pro.ttf");
    /*t_title.setFont(f_font);

    t_title.setCharacterSize(112);
    //t_title.setColor(sf::Color::White);
    t_title.setFillColor(sf::Color::White);


    t_disclaimer.setFont(m_font);

    t_disclaimer.setCharacterSize(42);
    //t_title.setColor(sf::Color::White);
    t_disclaimer.setFillColor(sf::Color::Red);

    //t_pressToContinue.setFont(f_font);
    //t_pressToContinue.setCharacterSize(32);
    //t_pressToContinue.setColor(sf::Color::White);
    //t_pressToContinue.setFillColor(sf::Color::White);
    //t_pressToContinue.setString("Press any key to continue...");
    //t_pressToContinue.setOrigin(t_pressToContinue.getGlobalBounds().width/2,t_pressToContinue.getGlobalBounds().height/2);

    mm_bigBox.setSize(sf::Vector2f(100,10));
    mm_bigBox.setFillColor(sf::Color(4,0,90));

    mm_smallerBox.setSize(sf::Vector2f(100,10));
    mm_smallerBox.setFillColor(sf::Color::Red);

    mm_titleBox.setSize(sf::Vector2f(100,10));
    mm_titleBox.setFillColor(sf::Color::Red);*/
    isActive=false;
    madeChanges=false;
}
void OptionsMenu::Initialise(Config *thisConfigs,std::map<int,bool> *keymap,V4Core *parent, Menu *curParentMenu)
{
    cout << "Initialize Options menu" << endl;
    Scene::Initialise(thisConfigs,keymap,parent);
    //buttonList.Initialise(&m_font,*thisConfig,keymap,&(v4core->currentController),this);
    parentMenu = curParentMenu;
    cout << "Initial values loaded, loading assets" << endl;
    /*t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"menu_button_3")));
    t_title.setOrigin(t_title.getGlobalBounds().width/2,t_title.getGlobalBounds().height/2);
    t_disclaimer.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"option_disclaimer")));
    t_disclaimer.setOrigin(t_disclaimer.getGlobalBounds().width/2,t_disclaimer.getGlobalBounds().height/2);*/

    int q = thisConfigs->GetInt("textureQuality");
    cout << "Quality:" << q << endl;
    bg.loadFromFile("resources/graphics/ui/options/options.png", q, 1);
    sword.loadFromFile("resources/graphics/ui/options/sword.png", q, 2);

    handle.loadFromFile("resources/graphics/ui/options/handle.png", q, 1);

    aura1.loadFromFile("resources/graphics/ui/options/aura.png", q, 1);
    aura2.loadFromFile("resources/graphics/ui/options/aura.png", q, 1);

    l_fire1.loadFromFile("resources/graphics/ui/options/l_fire1.png", q, 1);
    l_fire2.loadFromFile("resources/graphics/ui/options/l_fire2.png", q, 1);
    l_fire3.loadFromFile("resources/graphics/ui/options/l_fire3.png", q, 1);

    r_fire1.loadFromFile("resources/graphics/ui/options/r_fire1.png", q, 1);
    r_fire2.loadFromFile("resources/graphics/ui/options/r_fire2.png", q, 1);
    r_fire3.loadFromFile("resources/graphics/ui/options/r_fire3.png", q, 1);

    dg_restart.loadFromFile("resources/graphics/ui/options/dg_restart.png", q, 1);
    dg_select.loadFromFile("resources/graphics/ui/options/dg_select.png", q, 2);

    t_restart.createText(m_font, 26, sf::Color::Black, "You need to restart your game to\napply changes. Restart now?", q, 2);

    options_header.createText(m_font, 45, sf::Color::White, "Options", q, 2);

    PText opt;
    opt.createText(m_font, 25, sf::Color::White, "Graphics settings", q, 2);
    options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "Audio settings", q, 2);
    options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "Input settings", q, 2);
    options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "Language settings", q, 2);
    options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "Back to Main menu", q, 2);
    options.push_back(opt);

    opt.createText(m_font, 25, sf::Color::White, "Game resolution", q, 2);
    g_options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "Texture quality", q, 2);
    g_options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "Framerate limit", q, 2);
    g_options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "Switch fullscreen", q, 2);
    g_options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "Borderless window", q, 2);
    g_options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "Go back", q, 2);
    g_options.push_back(opt);

    vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    for(int i=0; i<modes.size(); i++)
    {
        if(float(modes[i].height) / float(modes[i].width) == 0.5625)
        {
            string res = to_string(modes[i].width)+"x"+to_string(modes[i].height);

            opt.createText(m_font, 25, sf::Color::White, res, q, 2);
            resolutions.push_back(opt);

            Resolution tmp;
            tmp.width = modes[i].width;
            tmp.height = modes[i].height;

            float_resolutions.push_back(tmp);
        }
    }
    opt.createText(m_font, 25, sf::Color::White, "Go back", q, 2);
    resolutions.push_back(opt);

    opt.createText(m_font, 25, sf::Color::White, "0 (unlimited)", q, 2);
    framerates.push_back(opt);
    float_framerates.push_back(0);
    opt.createText(m_font, 25, sf::Color::White, "240", q, 2);
    framerates.push_back(opt);
    float_framerates.push_back(240);
    opt.createText(m_font, 25, sf::Color::White, "120", q, 2);
    framerates.push_back(opt);
    float_framerates.push_back(120);
    opt.createText(m_font, 25, sf::Color::White, "60", q, 2);
    framerates.push_back(opt);
    float_framerates.push_back(60);
    opt.createText(m_font, 25, sf::Color::White, "Go back", q, 2);
    framerates.push_back(opt);

    opt.createText(m_font, 25, sf::Color::White, "Low", q, 2);
    qualities.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "Medium", q, 2);
    qualities.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "High", q, 2);
    qualities.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "Ultra", q, 2);
    qualities.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "Go back", q, 2);
    qualities.push_back(opt);

    opt.createText(m_font, 25, sf::Color::White, "Master volume", q, 2);
    a_options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "Drum sounds", q, 2);
    a_options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "Patapon drum chants", q, 2);
    a_options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "Patapon responses", q, 2);
    a_options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "Go back", q, 2);
    a_options.push_back(opt);

    opt.createText(m_font, 25, sf::Color::White, "Volume: 100%", q, 2);
    ms_volume.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "Go back", q, 2);
    ms_volume.push_back(opt);

    opt.createText(m_font, 25, sf::Color::White, "Enable", q, 2);
    switches.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "Disable", q, 2);
    switches.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "Go back", q, 2);
    switches.push_back(opt);

    opt.createText(m_font, 25, sf::Color::Black, "Restart game", q, 2);
    restarts.push_back(opt);
    opt.createText(m_font, 25, sf::Color::Black, "Revert changes", q, 2);
    restarts.push_back(opt);
}

void OptionsMenu::SelectMenuOption()
{
    if(sel != -1)
    {
        if(state == 0)
        {
            state = sel+1;
        }
        else
        {
            state = state*10 + (sel+1);
        }
    }

    sel = 0;

    cout << "State switched to " << state << endl;
}

void OptionsMenu::SetConfigValue(std::string key, std::string value)
{
    if(thisConfig->GetString(key) != value)
    {
        cout << "Changing key " << key << " to " << value << endl;

        ///Save the original key first, if not already saved
        bool found = false;

        for(int v=0; v<original_config.size(); v++)
        {
            if(original_config[v].index == key)
            found = true;
        }

        if(!found)
        {
            cout << "Past changes not found; save original config" << endl;

            ConfigValue tmp;
            tmp.index = key;
            tmp.value = thisConfig->GetString(key);
            original_config.push_back(tmp);
        }

        ///Make the changes
        thisConfig->SetString(key,value);
        madeChanges = true;
    }
}

void OptionsMenu::EventFired(sf::Event event){
    if(event.type == sf::Event::KeyPressed)
    {
        // do something here;
        // buttonList.KeyPressedEvent(event);
        if(event.key.code==sf::Keyboard::Escape) {
            thisConfig->debugOut->DebugMessage("Returning to main menu...");
            Back();
        }
    } else if (event.type == sf::Event::MouseButtonReleased){
        // We use mouse released so a user can change their mind by keeping the mouse held and moving away.
        //buttonList.MouseReleasedEvent(event);
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            SelectMenuOption();
        }
    }
    else if (event.type == sf::Event::MouseMoved)
    {
        mouseX = event.mouseMove.x;
        mouseY = event.mouseMove.y;
    }
}
void OptionsMenu::Update(sf::RenderWindow &window, float fps)
{
    if(isActive)
    {
        cout << "state: " << state << endl;

        bg.setPosition(0,0);
        bg.draw(window);

        if(sel == -1)
        sword.setPosition(-999, -999);
        else
        sword.setPosition(725, 507 + 40*sel);

        cout << "MouseY: " << mouseY / window.getSize().y * 1080 << endl;

        switch(state)
        {
            case 0:
            {
                options_header.setString("Options");
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                for(int i=0; i<options.size(); i++)
                {
                    //cout << "Options menu " << i << " " << options[i].getGlobalBounds().width/2 << " " << options[i].getGlobalBounds().height/2 << " " << options[i].getGlobalBoundsScaled().width/2 << " " << options[i].getGlobalBoundsScaled().height/2 << " " << options[i].orX << " " << options[i].orY << " " << options[i].lx << " " << options[i].ly << endl;

                    options[i].setOrigin(0, options[i].getGlobalBoundsScaled().height/2);
                    options[i].setPosition(810, 520 + 40*i);
                    options[i].setColor(sf::Color::White);

                    if(mouseY / window.getSize().y * 1080 >= (options[i].getPosition().y - options[i].getGlobalBoundsScaled().height/2 + 8))
                    {
                        if(mouseY / window.getSize().y * 1080 <= (options[i].getPosition().y + options[i].getGlobalBoundsScaled().height/2 + 8))
                        {
                            options[i].setColor(sf::Color(255,255,255,192));

                            sel = i;
                        }
                    }

                    options[i].draw(window);
                }

                sword.draw(window);

                break;
            }

            case 1:
            {
                options_header.setString("Graphics settings");
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                for(int i=0; i<g_options.size(); i++)
                {
                    g_options[i].setOrigin(0, g_options[i].getGlobalBoundsScaled().height/2);
                    g_options[i].setPosition(810, 520 + 40*i);
                    g_options[i].setColor(sf::Color::White);

                    if(mouseY / window.getSize().y * 1080 >= (g_options[i].getPosition().y - g_options[i].getGlobalBoundsScaled().height/2 + 8))
                    {
                        if(mouseY / window.getSize().y * 1080 <= (g_options[i].getPosition().y + g_options[i].getGlobalBoundsScaled().height/2 + 8))
                        {
                            g_options[i].setColor(sf::Color(255,255,255,192));

                            sel = i;
                        }
                    }

                    g_options[i].draw(window);
                }

                sword.draw(window);

                break;
            }

            case 2:
            {
                options_header.setString("Audio settings");
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                for(int i=0; i<a_options.size(); i++)
                {
                    a_options[i].setOrigin(0, a_options[i].getGlobalBoundsScaled().height/2);
                    a_options[i].setPosition(810, 520 + 40*i);
                    a_options[i].setColor(sf::Color::White);

                    if(mouseY / window.getSize().y * 1080 >= (a_options[i].getPosition().y - a_options[i].getGlobalBoundsScaled().height/2 + 8))
                    {
                        if(mouseY / window.getSize().y * 1080 <= (a_options[i].getPosition().y + a_options[i].getGlobalBoundsScaled().height/2 + 8))
                        {
                            a_options[i].setColor(sf::Color(255,255,255,192));

                            sel = i;
                        }
                    }

                    a_options[i].draw(window);
                }

                sword.draw(window);

                break;
            }

            case 5:
            {
                if(madeChanges)
                {
                    dg_restart.setPosition(474,290);
                    dg_restart.draw(window);

                    t_restart.setPosition(765,447);
                    t_restart.draw(window);

                    //dg_select.setOrigin(dg_select.getGlobalBoundsScaled().width/2, dg_select.getGlobalBoundsScaled().height/2);
                    dg_select.setPosition(711, 536 + 45*sel);
                    dg_select.draw(window);

                    for(int i=0; i<restarts.size(); i++)
                    {
                        restarts[i].setOrigin(restarts[i].getGlobalBoundsScaled().width/2, restarts[i].getGlobalBoundsScaled().height/2);
                        restarts[i].setPosition(960, 550 + 45*i);
                        restarts[i].setColor(sf::Color::Black);

                        if(mouseY / window.getSize().y * 1080 >= (restarts[i].getPosition().y - restarts[i].getGlobalBoundsScaled().height/2 + 8))
                        {
                            if(mouseY / window.getSize().y * 1080 <= (restarts[i].getPosition().y + restarts[i].getGlobalBoundsScaled().height/2 + 8))
                            {
                                sel = i;
                            }
                        }

                        restarts[i].draw(window);
                    }
                }
                else
                {
                    Back();
                    //thisConfig->SaveConfig();
                    sel = 0;
                }

                break;
            }

            case 11:
            {
                options_header.setString("Screen resolution");
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                for(int i=0; i<resolutions.size(); i++)
                {
                    resolutions[i].setOrigin(0, resolutions[i].getGlobalBoundsScaled().height/2);
                    resolutions[i].setPosition(810, 520 + 40*i);
                    resolutions[i].setColor(sf::Color::White);

                    if((float_resolutions[i].width == thisConfig->GetInt("resX")) && (float_resolutions[i].height == thisConfig->GetInt("resY")))
                    resolutions[i].setColor(sf::Color(0,192,0,255));

                    if(mouseY / window.getSize().y * 1080 >= (resolutions[i].getPosition().y - resolutions[i].getGlobalBoundsScaled().height/2 + 8))
                    {
                        if(mouseY / window.getSize().y * 1080 <= (resolutions[i].getPosition().y + resolutions[i].getGlobalBoundsScaled().height/2 + 8))
                        {
                            resolutions[i].setColor(sf::Color(255,255,255,192));

                            sel = i;
                        }
                    }

                    resolutions[i].draw(window);
                }

                sword.draw(window);

                break;
            }

            case 12:
            {
                options_header.setString("Texture quality");
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                for(int i=0; i<qualities.size(); i++)
                {
                    qualities[i].setOrigin(0, qualities[i].getGlobalBoundsScaled().height/2);
                    qualities[i].setPosition(810, 520 + 40*i);
                    qualities[i].setColor(sf::Color::White);

                    if(thisConfig->GetInt("textureQuality") == i)
                    qualities[i].setColor(sf::Color(0,192,0,255));

                    if(mouseY / window.getSize().y * 1080 >= (qualities[i].getPosition().y - qualities[i].getGlobalBoundsScaled().height/2 + 8))
                    {
                        if(mouseY / window.getSize().y * 1080 <= (qualities[i].getPosition().y + qualities[i].getGlobalBoundsScaled().height/2 + 8))
                        {
                            qualities[i].setColor(sf::Color(255,255,255,192));

                            sel = i;
                        }
                    }

                    qualities[i].draw(window);
                }

                sword.draw(window);

                break;
            }

            case 13:
            {
                options_header.setString("Framerate limit");
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                for(int i=0; i<framerates.size(); i++)
                {
                    framerates[i].setOrigin(0, framerates[i].getGlobalBoundsScaled().height/2);
                    framerates[i].setPosition(810, 520 + 40*i);
                    framerates[i].setColor(sf::Color::White);

                    if((thisConfig->GetInt("framerateLimit") == float_framerates[i]) && (i != 4))
                    framerates[i].setColor(sf::Color(0,192,0,255));

                    if(mouseY / window.getSize().y * 1080 >= (framerates[i].getPosition().y - framerates[i].getGlobalBoundsScaled().height/2 + 8))
                    {
                        if(mouseY / window.getSize().y * 1080 <= (framerates[i].getPosition().y + framerates[i].getGlobalBoundsScaled().height/2 + 8))
                        {
                            framerates[i].setColor(sf::Color(255,255,255,192));

                            sel = i;
                        }
                    }

                    framerates[i].draw(window);
                }

                sword.draw(window);

                break;
            }

            case 14:
            {
                options_header.setString("Switch fullscreen");
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                for(int i=0; i<switches.size(); i++)
                {
                    switches[i].setOrigin(0, switches[i].getGlobalBoundsScaled().height/2);
                    switches[i].setPosition(810, 520 + 40*i);
                    switches[i].setColor(sf::Color::White);

                    if((thisConfig->GetInt("enableFullscreen") == !i) && (i != 2))
                    switches[i].setColor(sf::Color(0,192,0,255));

                    if(mouseY / window.getSize().y * 1080 >= (switches[i].getPosition().y - switches[i].getGlobalBoundsScaled().height/2 + 8))
                    {
                        if(mouseY / window.getSize().y * 1080 <= (switches[i].getPosition().y + switches[i].getGlobalBoundsScaled().height/2 + 8))
                        {
                            switches[i].setColor(sf::Color(255,255,255,192));

                            sel = i;
                        }
                    }

                    switches[i].draw(window);
                }

                sword.draw(window);

                break;
            }

            case 15:
            {
                options_header.setString("Borderless window");
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                for(int i=0; i<switches.size(); i++)
                {
                    switches[i].setOrigin(0, switches[i].getGlobalBoundsScaled().height/2);
                    switches[i].setPosition(810, 520 + 40*i);
                    switches[i].setColor(sf::Color::White);

                    if((thisConfig->GetInt("enableBorderlessWindow") == !i) && (i != 2))
                    switches[i].setColor(sf::Color(0,192,0,255));

                    if(mouseY / window.getSize().y * 1080 >= (switches[i].getPosition().y - switches[i].getGlobalBoundsScaled().height/2 + 8))
                    {
                        if(mouseY / window.getSize().y * 1080 <= (switches[i].getPosition().y + switches[i].getGlobalBoundsScaled().height/2 + 8))
                        {
                            switches[i].setColor(sf::Color(255,255,255,192));

                            sel = i;
                        }
                    }

                    switches[i].draw(window);
                }

                sword.draw(window);

                break;
            }

            case 16:
            {
                state = 0;
                sel = 0;
                break;
            }

            case 21:
            {
                options_header.setString("Master volume");
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                for(int i=0; i<ms_volume.size(); i++)
                {
                    ms_volume[i].setOrigin(0, ms_volume[i].getGlobalBoundsScaled().height/2);
                    ms_volume[i].setPosition(810, 520 + 40*i);
                    ms_volume[i].setColor(sf::Color::White);

                    if(mouseY / window.getSize().y * 1080 >= (ms_volume[i].getPosition().y - ms_volume[i].getGlobalBoundsScaled().height/2 + 8))
                    {
                        if(mouseY / window.getSize().y * 1080 <= (ms_volume[i].getPosition().y + ms_volume[i].getGlobalBoundsScaled().height/2 + 8))
                        {
                            ms_volume[i].setColor(sf::Color(255,255,255,192));

                            sel = i;
                        }
                    }

                    ms_volume[i].draw(window);
                }

                sword.draw(window);

                break;
            }

            case 22:
            {
                options_header.setString("Drum sounds");
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                for(int i=0; i<switches.size(); i++)
                {
                    switches[i].setOrigin(0, switches[i].getGlobalBoundsScaled().height/2);
                    switches[i].setPosition(810, 520 + 40*i);
                    switches[i].setColor(sf::Color::White);

                    if((thisConfig->GetInt("enableDrums") == !i) && (i != 2))
                    switches[i].setColor(sf::Color(0,192,0,255));

                    if(mouseY / window.getSize().y * 1080 >= (switches[i].getPosition().y - switches[i].getGlobalBoundsScaled().height/2 + 8))
                    {
                        if(mouseY / window.getSize().y * 1080 <= (switches[i].getPosition().y + switches[i].getGlobalBoundsScaled().height/2 + 8))
                        {
                            switches[i].setColor(sf::Color(255,255,255,192));

                            sel = i;
                        }
                    }

                    switches[i].draw(window);
                }

                sword.draw(window);

                break;
            }

            case 23:
            {
                options_header.setString("Patapon drum chants");
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                for(int i=0; i<switches.size(); i++)
                {
                    switches[i].setOrigin(0, switches[i].getGlobalBoundsScaled().height/2);
                    switches[i].setPosition(810, 520 + 40*i);
                    switches[i].setColor(sf::Color::White);

                    if((thisConfig->GetInt("enableDrumChants") == !i) && (i != 2))
                    switches[i].setColor(sf::Color(0,192,0,255));

                    if(mouseY / window.getSize().y * 1080 >= (switches[i].getPosition().y - switches[i].getGlobalBoundsScaled().height/2 + 8))
                    {
                        if(mouseY / window.getSize().y * 1080 <= (switches[i].getPosition().y + switches[i].getGlobalBoundsScaled().height/2 + 8))
                        {
                            switches[i].setColor(sf::Color(255,255,255,192));

                            sel = i;
                        }
                    }

                    switches[i].draw(window);
                }

                sword.draw(window);

                break;
            }

            case 24:
            {
                options_header.setString("Patapon responses");
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                for(int i=0; i<switches.size(); i++)
                {
                    switches[i].setOrigin(0, switches[i].getGlobalBoundsScaled().height/2);
                    switches[i].setPosition(810, 520 + 40*i);
                    switches[i].setColor(sf::Color::White);

                    if((thisConfig->GetInt("enablePataponChants") == !i) && (i != 2))
                    switches[i].setColor(sf::Color(0,192,0,255));

                    if(mouseY / window.getSize().y * 1080 >= (switches[i].getPosition().y - switches[i].getGlobalBoundsScaled().height/2 + 8))
                    {
                        if(mouseY / window.getSize().y * 1080 <= (switches[i].getPosition().y + switches[i].getGlobalBoundsScaled().height/2 + 8))
                        {
                            switches[i].setColor(sf::Color(255,255,255,192));

                            sel = i;
                        }
                    }

                    switches[i].draw(window);
                }

                sword.draw(window);

                break;
            }

            case 25:
            {
                state = 0;
                sel = 0;
                break;
            }

            case 51:
            {
                thisConfig->SaveConfig();
                v4core->closeWindow = true;

                ///Run the game process again
                STARTUPINFO info = {sizeof(info)};
                PROCESS_INFORMATION processInfo;
                if (CreateProcess("V4Hero.exe", "", NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo)) {
                    CloseHandle(processInfo.hProcess); // Cleanup since you don't need this
                    CloseHandle(processInfo.hThread); // Cleanup since you don't need this
                }

                break;
            }

            case 52:
            {
                for(int i=0; i<original_config.size(); i++)
                thisConfig->SetString(original_config[i].index, original_config[i].value);

                Back();
                break;
            }

            case 121:
            {
                SetConfigValue("textureQuality", "0");

                state = 12;
                sel = 0;
                break;
            }

            case 122:
            {
                SetConfigValue("textureQuality", "1");

                state = 12;
                sel = 0;
                break;
            }

            case 123:
            {
                SetConfigValue("textureQuality", "2");

                state = 12;
                sel = 0;
                break;
            }

            case 124:
            {
                SetConfigValue("textureQuality", "3");

                state = 12;
                sel = 0;
                break;
            }

            case 125:
            {
                state = 1;
                sel = 0;
                break;
            }

            case 141:
            {
                SetConfigValue("enableFullscreen", "1");

                state = 14;
                sel = 0;
                break;
            }

            case 142:
            {
                SetConfigValue("enableFullscreen", "0");

                state = 14;
                sel = 0;
                break;
            }

            case 143:
            {
                state = 1;
                sel = 0;
                break;
            }

            case 151:
            {
                SetConfigValue("enableBorderlessWindow", "1");

                state = 15;
                sel = 0;
                break;
            }

            case 152:
            {
                SetConfigValue("enableBorderlessWindow", "0");

                state = 15;
                sel = 0;
                break;
            }

            case 153:
            {
                state = 1;
                sel = 0;
                break;
            }

            case 212:
            {
                state = 2;
                sel = 0;
                break;
            }

            case 221:
            {
                SetConfigValue("enableDrums", "1");

                state = 22;
                sel = 0;
                break;
            }

            case 222:
            {
                SetConfigValue("enableDrums", "0");

                state = 22;
                sel = 0;
                break;
            }

            case 223:
            {
                state = 2;
                sel = 0;
                break;
            }

            case 231:
            {
                SetConfigValue("enableDrumChants", "1");

                state = 23;
                sel = 0;
                break;
            }

            case 232:
            {
                SetConfigValue("enableDrumChants", "0");

                state = 23;
                sel = 0;
                break;
            }

            case 233:
            {
                state = 2;
                sel = 0;
                break;
            }

            case 241:
            {
                SetConfigValue("enablePataponChants", "1");

                state = 24;
                sel = 0;
                break;
            }

            case 242:
            {
                SetConfigValue("enablePataponChants", "0");

                state = 24;
                sel = 0;
                break;
            }

            case 243:
            {
                state = 2;
                sel = 0;
                break;
            }
        }

        if((state >= 111) && (state <= 110+resolutions.size()-1))
        {
            int resID = state-111;

            SetConfigValue("resX", to_string(float_resolutions[resID].width));
            SetConfigValue("resY", to_string(float_resolutions[resID].height));
            state = 11;
            sel = 0;
        }

        if(state == 110+resolutions.size())
        {
            state = 1;
            sel = 0;
        }

        if((state >= 131) && (state <= 130+framerates.size()-1))
        {
            int fpsID = state-131;

            SetConfigValue("framerateLimit", to_string(float_framerates[fpsID]));

            state = 13;
            sel = 0;
        }

        if(state == 130+framerates.size())
        {
            state = 1;
            sel = 0;
        }

        window.setView(window.getDefaultView());
    }

}
void OptionsMenu::Back(){
    /// this should go back to the previous menu.
    Hide();
    parentMenu->Show();
    v4core->ChangeRichPresence("In Main menu", "logo", "");
    OnExit();
}
void OptionsMenu::OnExit(){
    /// when we exit the options menu, we should save the user's settings to a text file

}
void OptionsMenu::UpdateButtons(){
    /// this should update the text on all the buttons
    buttonList.UpdateButtons();
}
void OptionsMenu::Show(){
    isActive = true;
    /*Menu::Show();
    buttonList.Show();
    t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"menu_button_3")));
    t_title.setOrigin(t_title.getGlobalBounds().width/2,t_title.getGlobalBounds().height/2);
    t_disclaimer.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"option_disclaimer")));
    t_disclaimer.setOrigin(t_disclaimer.getGlobalBounds().width/2,t_disclaimer.getGlobalBounds().height/2);*/
}
OptionsMenu::~OptionsMenu()
{
    //dtor
}
