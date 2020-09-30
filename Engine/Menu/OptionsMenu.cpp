#include "OptionsMenu.h"
#include "ButtonList.h"
#include <iostream>
#include <windows.h>
#include "../V4Core.h"
OptionsMenu::OptionsMenu()
{
    //ctor
    f_font.loadFromFile("resources/fonts/patapon.ttf");
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
void OptionsMenu::Initialise(Config *thisConfigs,V4Core *parent, Menu *curParentMenu)
{
    parent->SaveToDebugLog("Initializing Options menu...");
    cout << "Initialize Options menu" << endl;
    Scene::Initialise(thisConfigs,parent);
    //buttonList.Initialise(&m_font,*thisConfig,keymap,&(v4core->currentController),this);
    parentMenu = curParentMenu;
    cout << "Initial values loaded, loading assets" << endl;
    /*t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"menu_button_3")));

    t_title.setOrigin(t_title.getGlobalBounds().width/2,t_title.getGlobalBounds().height/2);
    t_disclaimer.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"option_disclaimer")));
    t_disclaimer.setOrigin(t_disclaimer.getGlobalBounds().width/2,t_disclaimer.getGlobalBounds().height/2);*/

    m_font.loadFromFile(thisConfigs->fontPath);

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

    t_restart.createText(m_font, 26, sf::Color::Black, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_restart_notice")), q, 2);

    vector<string> restart_opt = {Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_restart_button1")), Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_restart_button2"))};
    restart_prompt.Create(m_font, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_restart_notice")), restart_opt, q);
    options_header.createText(m_font, 45, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_options")), q, 2);

    PText opt;
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_graphics")), q, 2);
    options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_audio")), q, 2);
    options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_input")), q, 2);
    options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_language")), q, 2);
    options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_backtomain")), q, 2);
    options.push_back(opt);

    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_graphics1")), q, 2);
    g_options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_graphics2")), q, 2);
    g_options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_graphics4")), q, 2);
    g_options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_graphics5")), q, 2);
    g_options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_graphics6")), q, 2);
    g_options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_back")), q, 2);
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
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_back")), q, 2);
    resolutions.push_back(opt);

    opt.createText(m_font, 25, sf::Color::White, "0 "+Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_unlimited")), q, 2);
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
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_back")), q, 2);
    framerates.push_back(opt);

    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_quality_low")), q, 2);
    qualities.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_quality_medium")), q, 2);
    qualities.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_quality_high")), q, 2);
    qualities.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_quality_ultra")), q, 2);
    qualities.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_back")), q, 2);
    qualities.push_back(opt);

    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_audio1")), q, 2);
    a_options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_audio2")), q, 2);
    a_options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_audio3")), q, 2);
    a_options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_audio4")), q, 2);
    a_options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_back")), q, 2);
    a_options.push_back(opt);

    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_volume1")), q, 2);
    ms_volume.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_volume2")), q, 2);
    ms_volume.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_volume3")), q, 2);
    ms_volume.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_back")), q, 2);
    ms_volume.push_back(opt);

    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_enable")), q, 2);
    switches.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_disable")), q, 2);
    switches.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_back")), q, 2);
    switches.push_back(opt);

    opt.createText(m_font, 25, sf::Color::Black, "Restart game", q, 2);
    restarts.push_back(opt);
    opt.createText(m_font, 25, sf::Color::Black, "Revert changes", q, 2);
    restarts.push_back(opt);

    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_input2")), q, 2);
    inputs.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_input1")), q, 2);
    inputs.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_back")), q, 2);
    inputs.push_back(opt);

    ifstream langfile("resources/lang/lang.txt");
    string buf;
    int langcount = 0;
    int page = 0;

    while(getline(langfile, buf))
    {
        vector<string> param = Func::Split(buf, '|');

        opt.createText(m_font, 25, sf::Color::White, param[0], q, 2);
        langs[page].push_back(opt);

        langcount++;

        if(langcount >= 5)
        {
            opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"nav_nextpage")), q, 2);
            langs[page].push_back(opt);

            opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_back")), q, 2);
            langs[page].push_back(opt);

            langcount = 0;
            page++;
        }
    }

    if(langcount != 0)
    {
        opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"nav_nextpage")), q, 2);
        langs[page].push_back(opt);

        opt.createText(m_font, 25, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_back")), q, 2);
        langs[page].push_back(opt);

        page++;
    }

    langfile.close();

    lang_pages = page;

    ///Input manager (change keybinds)
    input_manager.loadFromFile("resources/graphics/ui/options/inputmenu.png", q, 1); ///sprite

    for(int i=0; i<9; i++)
    {
        t_presets[i].createText(m_font, 18, sf::Color::Black, to_string(i+1), q, 1);
    }

    t_igbutton.createText(m_font, 18, sf::Color::Black, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_ingame_btn")), q, 1);
    t_assigned.createText(m_font, 18, sf::Color::Black, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_assign_key")), q, 1);

    for(int i=0; i<12; i++)
    {
        t_igkey[i].createText(m_font, 18, sf::Color::Black, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_key")), q, 1);
        t_askey[i].createText(m_font, 18, sf::Color::Black, "", q, 1);
    }

    t_im_tip.createText(m_font, 18, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_tip1")), q, 1); ///input manager's tip

    t_change_title.createText(m_font, 28, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_change_title")), q, 1);
    t_change_button.createText(m_font, 62, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_change_button")), q, 1);
    t_change_anykey.createText(m_font, 28, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_change_anykey")), q, 1); ///change dialog

    ///Controller setup
    t_cs_title.createText(m_font, 26, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_input1")), q, 1);
    t_cs_desc.createText(m_font, 16, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_input_setup1")), q, 1);
    t_cs_bigbutton.createText(m_font, 30, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_bigbutton")), q, 1);
    t_cs_tip.createText(m_font, 15, sf::Color::White, Func::ConvertToUtf8String(thisConfigs->strRepo.GetUnicodeString(L"options_tip2")), q, 1);

    parent->SaveToDebugLog("Options menu initialized.");
}

void OptionsMenu::SelectMenuOption()
{
    if(sel != -1)
    {
        int newState = 0;

        if(state == 0)
        {
            newState = sel+1;
        }
        else
        {
            newState = state*10 + (sel+1);
        }

        if(newState > state)
        prevStates.push_back(state);

        state = newState;
    }

    sel = 0;

    cout << "OptionsMenu::SelectMenuOption(): State switched to " << state << endl;
}

void OptionsMenu::GoBackMenuOption(int a)
{
    if(prevStates.size() > a-1)
    {
        state = prevStates[prevStates.size()-a];

        for(int i=0; i<a; i++)
        prevStates.pop_back();
    }
    else
    {
        state = 0;
    }

    sel = 0;

    cout << "OptionsMenu::GoBackMenuOption(): State switched to " << state << endl;
}

void OptionsMenu::SetConfigValue(std::string key, std::string value, bool selectmenu)
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

        ///Change the state
        if(selectmenu)
        SelectMenuOption();
    }
}

void OptionsMenu::EventFired(sf::Event event){
    if(event.type == sf::Event::KeyPressed)
    {

    } else if (event.type == sf::Event::MouseButtonReleased){
        // We use mouse released so a user can change their mind by keeping the mouse held and moving away.
        //buttonList.MouseReleasedEvent(event);
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            if(state != 31)
            SelectMenuOption();
        }
    }
    else if (event.type == sf::Event::MouseMoved)
    {
        mouseX = event.mouseMove.x;
        mouseY = event.mouseMove.y;
    }
}
void OptionsMenu::Update(sf::RenderWindow &window, float fps, InputController& inputCtrl)
{
    if(isActive)
    {
        bg.setPosition(0,0);
        bg.draw(window);

        if(sel == -1)
        sword.setPosition(-999, -999);
        else
        sword.setPosition(725, 507 + 40*sel);

        switch(state)
        {
            case 0:
            {
                lang_current = 0;

                options_header.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_options")));
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                maxSel = options.size();

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
                options_header.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_graphics")));
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                maxSel = g_options.size();

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
                options_header.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_audio")));
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                maxSel = a_options.size();

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

            case 3:
            {
                options_header.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_input")));
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                maxSel = inputs.size();

                for(int i=0; i<inputs.size(); i++)
                {
                    inputs[i].setOrigin(0, inputs[i].getGlobalBoundsScaled().height/2);
                    inputs[i].setPosition(810, 520 + 40*i);
                    inputs[i].setColor(sf::Color::White);

                    if(mouseY / window.getSize().y * 1080 >= (inputs[i].getPosition().y - inputs[i].getGlobalBoundsScaled().height/2 + 8))
                    {
                        if(mouseY / window.getSize().y * 1080 <= (inputs[i].getPosition().y + inputs[i].getGlobalBoundsScaled().height/2 + 8))
                        {
                            inputs[i].setColor(sf::Color(255,255,255,192));

                            sel = i;
                        }
                    }

                    inputs[i].draw(window);
                }

                sword.draw(window);

                break;
            }

            case 4:
            {
                options_header.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_language")));
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                maxSel = langs[lang_current].size();

                for(int i=0; i<langs[lang_current].size(); i++)
                {
                    langs[lang_current][i].setOrigin(0, langs[lang_current][i].getGlobalBoundsScaled().height/2);
                    langs[lang_current][i].setPosition(810, 520 + 40*i);
                    langs[lang_current][i].setColor(sf::Color::White);

                    if(lang_current*5+i+1 == thisConfig->GetInt("lang"))
                    {
                        if(i < 5)
                        langs[lang_current][i].setColor(sf::Color(0,192,0,255));
                    }

                    if(mouseY / window.getSize().y * 1080 >= (langs[lang_current][i].getPosition().y - langs[lang_current][i].getGlobalBoundsScaled().height/2 + 8))
                    {
                        if(mouseY / window.getSize().y * 1080 <= (langs[lang_current][i].getPosition().y + langs[lang_current][i].getGlobalBoundsScaled().height/2 + 8))
                        {
                            langs[lang_current][i].setColor(sf::Color(255,255,255,192));

                            sel = i;
                        }
                    }

                    langs[lang_current][i].draw(window);
                }

                sword.draw(window);

                break;
            }

            case 5:
            {
                if(madeChanges)
                {
                    if(inputCtrl.isKeyPressed(InputController::Keys::UP))
                    restart_prompt.MoveUp();
                    if(inputCtrl.isKeyPressed(InputController::Keys::DOWN))
                    restart_prompt.MoveDown();

                    sel = restart_prompt.CheckSelectedOption();

                    restart_prompt.x = 640;
                    restart_prompt.y = 360;
                    restart_prompt.Draw(window, fps, inputCtrl);
                }
                else
                {
                    if(goto_id == -1)
                    {
                        cout << "Create a screenFade" << endl;
                        screenFade.Create(thisConfig, 1, 512);
                        goto_id = 0;
                    }
                    sel = 0;
                }

                break;
            }

            case 11:
            {
                options_header.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_graphics1")));
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                maxSel = resolutions.size();

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
                options_header.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_graphics2")));
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                maxSel = qualities.size();

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
                options_header.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_graphics4")));
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                maxSel = framerates.size();

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
                options_header.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_graphics5")));
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                maxSel = switches.size();

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
                options_header.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_graphics6")));
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                maxSel = switches.size();

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
                GoBackMenuOption();
                break;
            }

            case 21:
            {
                options_header.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_audio1")));
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                maxSel = ms_volume.size();

                string key = "";

                switch(sel)
                {
                    case 0:
                    key = "masterVolume";
                    break;

                    case 1:
                    key = "sfxVolume";
                    break;

                    case 2:
                    key = "bgmVolume";
                    break;
                }

                if(inputCtrl.isKeyPressed(InputController::Keys::LEFT))
                {
                    int newVolume = thisConfig->GetInt(key) - 10;

                    if(newVolume <= 0)
                    newVolume = 0;

                    SetConfigValue(key, to_string(newVolume), false);
                }
                else if(inputCtrl.isKeyPressed(InputController::Keys::RIGHT))
                {
                    int newVolume = thisConfig->GetInt(key) + 10;

                    if(newVolume >= 100)
                    newVolume = 100;

                    SetConfigValue(key, to_string(newVolume), false);
                }

                for(int i=0; i<ms_volume.size(); i++)
                {
                    string key = "";
                    string str = "";

                    switch(i)
                    {
                        case 0:
                        key = "masterVolume";
                        str = Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_volume1"));
                        break;

                        case 1:
                        key = "sfxVolume";
                        str = Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_volume2"));
                        break;

                        case 2:
                        key = "bgmVolume";
                        str = Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_volume3"));
                        break;
                    }

                    if(i<=2)
                    ms_volume[i].setString(str+to_string(thisConfig->GetInt(key))+"%");
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
                options_header.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_audio2")));
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                maxSel = switches.size();

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
                options_header.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_audio3")));
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                maxSel = switches.size();

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
                options_header.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_audio4")));
                options_header.setOrigin(options_header.getGlobalBoundsScaled().width/2, options_header.getGlobalBoundsScaled().height/2);
                options_header.setPosition(930, 460);
                options_header.draw(window);

                maxSel = switches.size();

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
                GoBackMenuOption();
                break;
            }

            case 31:
            {
                ///Input manager (change keybinds)
                input_manager.setOrigin(input_manager.getLocalBounds().width/2, input_manager.getLocalBounds().height/2);
                input_manager.setPosition(640, 360);
                input_manager.draw(window); ///sprite

                for(int i=0; i<9; i++)
                {
                    t_presets[i].setColor(sf::Color::Black);
                    t_presets[currentPreset].setColor(sf::Color(0,192,0,255));

                    t_presets[i].setOrigin(t_presets[i].getLocalBounds().width/2, t_presets[i].getLocalBounds().height/2);
                    t_presets[i].setPosition(input_manager.getPosition().x-158 + (39*i), input_manager.getPosition().y-248);
                    t_presets[i].draw(window);
                }

                t_igbutton.setPosition(input_manager.getPosition().x-168, input_manager.getPosition().y-217);
                t_igbutton.draw(window);
                t_assigned.setPosition(input_manager.getPosition().x+18, input_manager.getPosition().y-217);
                t_assigned.draw(window);

                for(int i=0; i<12; i++)
                {
                    t_igkey[i].setColor(sf::Color(64,64,64,255));
                    t_igkey[i].setPosition(input_manager.getPosition().x-168, input_manager.getPosition().y-181+(i*37.5));
                    t_igkey[i].setString(ingame_buttons[i]);
                    t_igkey[i].draw(window);

                    t_askey[i].setColor(sf::Color(0,0,0,255));
                    t_askey[currentOption].setColor(sf::Color(0,192,0,255));
                    t_askey[i].setPosition(input_manager.getPosition().x+18, input_manager.getPosition().y-181+(i*37.5));

                    string key = "keybind"+ingame_buttons[i]+to_string(currentPreset+1);
                    //cout << key << endl;

                    if(thisConfig->keyExists(key))
                    {
                        int keyID = (thisConfig->GetInt(key)+1);

                        if(keyID >= 1000)
                        {
                            keyID -= 1000;
                            t_askey[i].setString("Joystick "+to_string(keyID));
                        }
                        else
                        {
                            t_askey[i].setString(assigned_names[keyID]);
                        }
                    }
                    else
                    {
                        t_askey[i].setString("unassigned");
                    }

                    t_askey[i].draw(window);
                }

                t_im_tip.setOrigin(t_im_tip.getLocalBounds().width/2, 0);
                t_im_tip.setPosition(input_manager.getPosition().x,input_manager.getPosition().y+273);
                t_im_tip.draw(window);

                if(changeInput)
                {
                    block.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
                    block.setFillColor(sf::Color(0,0,0,192));
                    window.draw(block);

                    t_change_title.setOrigin(t_change_title.getLocalBounds().width/2, t_change_title.getLocalBounds().height/2);
                    t_change_title.setPosition(640, 200);
                    t_change_title.draw(window);

                    t_change_button.setString(ingame_buttons[currentOption]);
                    t_change_button.setOrigin(t_change_button.getLocalBounds().width/2, t_change_button.getLocalBounds().height/2);
                    t_change_button.setPosition(640, 300);
                    t_change_button.draw(window);

                    t_change_anykey.setOrigin(t_change_anykey.getLocalBounds().width/2, t_change_anykey.getLocalBounds().height/2);
                    t_change_anykey.setPosition(640, 500);
                    t_change_anykey.draw(window);
                }

                break;
            }

            case 32:
            {
                ///Controller setup

                block.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
                block.setFillColor(sf::Color(0,0,0,96));
                window.draw(block);

                if(setup_stage == 1)
                {
                    t_cs_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_input1")));
                    t_cs_title.setOrigin(t_cs_title.getLocalBounds().width/2, t_cs_title.getLocalBounds().height/2);
                    t_cs_title.setPosition(640, 240);
                    t_cs_title.draw(window);

                    string key = "options_input_setup"+to_string(setup_stage);
                    t_cs_desc.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(key)));
                    t_cs_desc.setOrigin(t_cs_desc.getLocalBounds().width/2, t_cs_desc.getLocalBounds().height/2);
                    t_cs_desc.setPosition(640, 280);
                    t_cs_desc.draw(window);

                    int checkKey = inputCtrl.whatKeyPressed(InputController::RestrictMode::ONLYJOYSTICK);

                    if(checkKey != -2)
                    setup_key = checkKey+1;

                    if(setup_key >= 1000)
                    {
                        t_cs_bigbutton.setString("Joystick "+to_string(setup_key-1000));
                    }
                    else if(setup_key >= 0)
                    {
                        t_cs_bigbutton.setString(assigned_names[setup_key]);
                    }
                    else
                    {
                        t_cs_bigbutton.setString("");
                    }

                    t_cs_bigbutton.setOrigin(t_cs_bigbutton.getLocalBounds().width/2, t_cs_bigbutton.getLocalBounds().height/2);
                    t_cs_bigbutton.setPosition(640, 340);
                    t_cs_bigbutton.draw(window);

                    t_cs_tip.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_tip3")));
                    t_cs_tip.setOrigin(t_cs_tip.getLocalBounds().width/2, t_cs_tip.getLocalBounds().height/2);
                    t_cs_tip.setPosition(640, 440);
                    t_cs_tip.draw(window);
                }
                else if(setup_stage != 14)
                {
                    t_cs_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_input1")));
                    t_cs_title.setOrigin(t_cs_title.getLocalBounds().width/2, t_cs_title.getLocalBounds().height/2);
                    t_cs_title.setPosition(640, 320);
                    t_cs_title.draw(window);

                    string key = "options_input_setup"+to_string(setup_stage);
                    t_cs_desc.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(key)));
                    t_cs_desc.setOrigin(t_cs_desc.getLocalBounds().width/2, t_cs_desc.getLocalBounds().height/2);
                    t_cs_desc.setPosition(640, 360);
                    t_cs_desc.draw(window);

                    int checkKey = inputCtrl.whatKeyPressed(InputController::RestrictMode::ONLYJOYSTICK);

                    if(checkKey != -2)
                    {
                        setup_key = checkKey;

                        ///Key has been pressed, save and go back
                        string key = "keybind"+ingame_buttons[setup_stage-2]+"2";
                        SetConfigValue(key, to_string(setup_key), false);

                        setup_stage++;
                    }
                }
                else
                {
                    int checkKey = inputCtrl.whatKeyPressed(InputController::RestrictMode::ONLYKEYBOARD);

                    if(checkKey != -2)
                    {
                        setup_stage = 1;

                        sel = 9999;
                        SelectMenuOption();
                        GoBackMenuOption();
                    }

                    t_cs_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_input1")));
                    t_cs_title.setOrigin(t_cs_title.getLocalBounds().width/2, t_cs_title.getLocalBounds().height/2);
                    t_cs_title.setPosition(640, 320);
                    t_cs_title.draw(window);

                    string key = "options_input_setup"+to_string(setup_stage);
                    t_cs_desc.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(key)));
                    t_cs_desc.setOrigin(t_cs_desc.getLocalBounds().width/2, t_cs_desc.getLocalBounds().height/2);
                    t_cs_desc.setPosition(640, 360);
                    t_cs_desc.draw(window);

                    t_cs_tip.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"options_tip2")));
                    t_cs_tip.setOrigin(t_cs_tip.getLocalBounds().width/2, t_cs_tip.getLocalBounds().height/2);
                    t_cs_tip.setPosition(640, 440);
                    t_cs_tip.draw(window);
                }

                break;
            }

            case 33:
            {
                GoBackMenuOption();
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

                if(goto_id == -1)
                {
                    cout << "Create a screenFade" << endl;
                    screenFade.Create(thisConfig, 1, 512);
                    goto_id = 0;
                }

                break;
            }

            case 121:
            {
                SetConfigValue("textureQuality", "0");

                GoBackMenuOption();
                break;
            }

            case 122:
            {
                SetConfigValue("textureQuality", "1");

                GoBackMenuOption();
                break;
            }

            case 123:
            {
                SetConfigValue("textureQuality", "2");

                GoBackMenuOption();
                break;
            }

            case 124:
            {
                SetConfigValue("textureQuality", "3");

                GoBackMenuOption();
                break;
            }

            case 125:
            {
                GoBackMenuOption();
                break;
            }

            case 141:
            {
                SetConfigValue("enableFullscreen", "1");

                GoBackMenuOption();
                break;
            }

            case 142:
            {
                SetConfigValue("enableFullscreen", "0");

                GoBackMenuOption();
                break;
            }

            case 143:
            {
                GoBackMenuOption();
                break;
            }

            case 151:
            {
                SetConfigValue("enableBorderlessWindow", "1");

                GoBackMenuOption();
                break;
            }

            case 152:
            {
                SetConfigValue("enableBorderlessWindow", "0");

                GoBackMenuOption();
                break;
            }

            case 153:
            {
                GoBackMenuOption();
                break;
            }

            case 211:
            {
                GoBackMenuOption(1);
                break;
            }

            case 212:
            {
                GoBackMenuOption(1);
                break;
            }

            case 213:
            {
                GoBackMenuOption(1);
                break;
            }

            case 214:
            {
                GoBackMenuOption();
                break;
            }

            case 221:
            {
                SetConfigValue("enableDrums", "1");

                GoBackMenuOption();
                break;
            }

            case 222:
            {
                SetConfigValue("enableDrums", "0");

                GoBackMenuOption();
                break;
            }

            case 223:
            {
                GoBackMenuOption();
                break;
            }

            case 231:
            {
                SetConfigValue("enableDrumChants", "1");

                GoBackMenuOption();
                break;
            }

            case 232:
            {
                SetConfigValue("enableDrumChants", "0");

                GoBackMenuOption();
                break;
            }

            case 233:
            {
                GoBackMenuOption();
                break;
            }

            case 241:
            {
                SetConfigValue("enablePataponChants", "1");

                GoBackMenuOption();
                break;
            }

            case 242:
            {
                SetConfigValue("enablePataponChants", "0");

                GoBackMenuOption();
                break;
            }

            case 243:
            {
                GoBackMenuOption();
                break;
            }
        }

        if((state >= 40) && (state <= 40+langs[lang_current].size()-2))
        {
            cout << "Select lang ID: " << lang_current*5+(state-40) << endl;
            SetConfigValue("lang", to_string(lang_current*5+(state-40)), false);

            GoBackMenuOption(1);
        }

        if(state == 40+langs[lang_current].size()-1)
        {
            lang_current++;

            if(lang_current >= lang_pages)
            lang_current = 0;

            GoBackMenuOption(1);
        }

        if(state == 40+langs[lang_current].size())
        {
            GoBackMenuOption();
        }

        if((state >= 111) && (state <= 110+resolutions.size()-1))
        {
            cout << "Apply the resolution" << endl;

            int resID = state-111;

            SetConfigValue("resX", to_string(float_resolutions[resID].width));
            SetConfigValue("resY", to_string(float_resolutions[resID].height));
            GoBackMenuOption(3);
        }

        if(state == 110+resolutions.size())
        {
            cout << "Leave the resolution options" << endl;

            GoBackMenuOption();
        }

        if((state >= 131) && (state <= 130+framerates.size()-1))
        {
            int fpsID = state-131;

            SetConfigValue("framerateLimit", to_string(float_framerates[fpsID]));

            GoBackMenuOption();
        }

        if(state == 130+framerates.size())
        {
            GoBackMenuOption();
        }

        window.setView(window.getDefaultView());

        if(state == 31)
        {
            if(!changeInput)
            {
                if(inputCtrl.isKeyPressed(InputController::Keys::UP))
                {
                    if(currentOption > 0)
                    currentOption--;
                }

                if(inputCtrl.isKeyPressed(InputController::Keys::DOWN))
                {
                    if(currentOption < 11)
                    currentOption++;
                }

                if(inputCtrl.isKeyPressed(InputController::Keys::LEFT))
                {
                    if(currentPreset > 0)
                    currentPreset--;
                }

                if(inputCtrl.isKeyPressed(InputController::Keys::RIGHT))
                {
                    if(currentPreset < 8)
                    currentPreset++;
                }

                if(inputCtrl.isKeyPressed(InputController::Keys::CIRCLE))
                {
                    sel = 9999;
                    SelectMenuOption();
                    GoBackMenuOption();
                }

                if(inputCtrl.isKeyPressed(InputController::Keys::CROSS))
                {
                    changeInput = true;
                }
            }
            else
            {
                int keyID = inputCtrl.whatKeyPressed();

                if(keyID != -2)
                {
                    ///Key has been pressed, save and go back
                    string key = "keybind"+ingame_buttons[currentOption]+to_string(currentPreset+1);
                    SetConfigValue(key, to_string(keyID), false);

                    changeInput = false;
                }
            }
        }
        else if(state == 32)
        {
            if(setup_stage == 1)
            {
                ///Only keyboard inputs here
                if(inputCtrl.isKeyPressed(InputController::Keys::CIRCLE, InputController::RestrictMode::ONLYKEYBOARD))
                {
                    sel = 9999;
                    SelectMenuOption();
                    GoBackMenuOption();
                }

                if(inputCtrl.isKeyPressed(InputController::Keys::START, InputController::RestrictMode::ONLYKEYBOARD))
                {
                    setup_stage = 2;
                }
            }
        }
        else
        {
            if(inputCtrl.isKeyPressed(InputController::Keys::CROSS))
            {
                SelectMenuOption();
            }

            if(inputCtrl.isKeyPressed(InputController::Keys::CIRCLE))
            {
                sel = 9999;
                SelectMenuOption();
                GoBackMenuOption();
            }

            if(inputCtrl.isKeyPressed(InputController::Keys::UP))
            {
                if(sel > 0)
                sel--;
            }

            if(inputCtrl.isKeyPressed(InputController::Keys::DOWN))
            {
                if(sel < maxSel-1)
                sel++;
            }
        }

        screenFade.draw(window, fps);

        if(screenFade.checkFinished())
        {
            if(goto_id != -1)
            {
                switch(goto_id)
                {
                    case 0: ///Back to main menu
                    {
                        cout << "Back to main menu" << endl;

                        Back();

                        parentMenu->screenFade.Create(thisConfig, 0, 512);

                        break;
                    }
                }

                goto_id = -1;
            }
        }
    }
}

void OptionsMenu::Back()
{
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
