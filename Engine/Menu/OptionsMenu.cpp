#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "OptionsMenu.h"
#include <iostream>
#include <spdlog/spdlog.h>
//#include <windows.h>
#include "math.h"
#include "../CoreManager.h"
#include "../StateManager.h"

OptionsMenu::OptionsMenu()
{
    //ctor
    /*t_title.defaultStyleSetFont(f_font);

    t_title.defaultStyleSetCharSize(112);
    //t_title.setColor(sf::Color::White);
    t_title.setFillColor(sf::Color::White);


    t_disclaimer.defaultStyleSetFont(m_font);

    t_disclaimer.defaultStyleSetCharSize(42);
    //t_title.setColor(sf::Color::White);
    t_disclaimer.setFillColor(sf::Color::Red);

    //t_pressToContinue.defaultStyleSetFont(f_font);
    //t_pressToContinue.defaultStyleSetCharSize(32);
    //t_pressToContinue.setColor(sf::Color::White);
    //t_pressToContinue.setFillColor(sf::Color::White);
    //t_pressToContinue.setString("Press any key to continue...");
    //t_pressToContinue.setOrigin(t_pressToContinue.getGlobalBounds().size.x/2,t_pressToContinue.getGlobalBounds().size.y/2);

    mm_bigBox.setSize(sf::Vector2f(100,10));
    mm_bigBox.setFillColor(sf::Color(4,0,90));

    mm_smallerBox.setSize(sf::Vector2f(100,10));
    mm_smallerBox.setFillColor(sf::Color::Red);

    mm_titleBox.setSize(sf::Vector2f(100,10));
    mm_titleBox.setFillColor(sf::Color::Red);*/
    is_active = false;
    madeChanges = false;

    Config* config = CoreManager::getInstance().getConfig();
    StringRepository* strRepo = CoreManager::getInstance().getStrRepo();
    std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    SPDLOG_DEBUG("Initial values loaded, loading assets");

    int q = config->GetInt("textureQuality");
    SPDLOG_TRACE("Quality: {}");

    ResourceManager::getInstance().loadSprite("resources/graphics/ui/options/options.png");
    ResourceManager::getInstance().loadSprite("resources/graphics/ui/options/sword.png");

    t_restart.defaultStyleSetFont(font);
    t_restart.defaultStyleSetCharSize(26);
    t_restart.defaultStyleSetColor(sf::Color::Black);
    t_restart.append(Func::GetStrFromKey("options_restart_notice"));

    //std::vector<sf::String> restart_opt = {"options_restart_button1", "options_restart_button2"};
    //restart_prompt.Create(font, "options_restart_notice", restart_opt);

    // TODO: this gets rewritten anyway
    /*
    options_header.defaultStyleSetFont(font);
    options_header.defaultStyleSetCharSize(45);
    options_header.setColor(sf::Color::White);
    options_header.append(Func::GetStrFromKey("options_options");

    PataText opt;
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_graphics");
    options.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_audio");
    options.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_input");
    options.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_language");
    options.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_backtomain");
    options.push_back(opt);

    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_graphics1");
    g_options.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_graphics2");
    g_options.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_graphics4");
    g_options.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_graphics5");
    g_options.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_graphics6");
    g_options.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_back");
    g_options.push_back(opt);

    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    for (int i = 0; i < modes.size(); i++)
    {
        if (float(modes[i].height) / float(modes[i].width) == 0.5625)
        {
            std::string res = std::to_string(modes[i].width) + "x" + std::to_string(modes[i].height);

            opt.defaultStyleSetFont(font);
            opt.defaultStyleSetCharSize(25);
            opt.setColor(sf::Color::White);
            opt.setString(res);

            Resolution tmp;
            tmp.width = modes[i].width;
            tmp.height = modes[i].height;

            bool exist = false;

            for (int m = 0; m < float_resolutions.size(); m++)
            {
                if ((float_resolutions[m].width == tmp.width) && (float_resolutions[m].height == tmp.height))
                    exist = true;
            }

            if (!exist)
            {
                resolutions.push_back(opt);
                float_resolutions.push_back(tmp);
            }
        }
    }

    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_back");
    resolutions.push_back(opt);

    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_unlimited");
    framerates.push_back(opt);
    float_framerates.push_back(0);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.setString("240");
    framerates.push_back(opt);
    float_framerates.push_back(240);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.setString("120");
    framerates.push_back(opt);
    float_framerates.push_back(120);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.setString("60");
    framerates.push_back(opt);
    float_framerates.push_back(60);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_back");
    framerates.push_back(opt);

    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_quality_low");
    qualities.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_quality_medium");
    qualities.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_quality_high");
    qualities.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_quality_ultra");
    qualities.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_back");
    qualities.push_back(opt);

    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_audio1");
    a_options.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_audio2");
    a_options.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_audio3");
    a_options.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_audio4");
    a_options.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_back");
    a_options.push_back(opt);

    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_volume1");
    ms_volume.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_volume2");
    ms_volume.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_volume3");
    ms_volume.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_back");
    ms_volume.push_back(opt);

    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_enable");
    switches.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_disable");
    switches.push_back(opt);
    opt.defaultStyleSetFont(font);
    opt.defaultStyleSetCharSize(25);
    opt.setColor(sf::Color::White);
    opt.append(Func::GetStrFromKey("options_back");
    switches.push_back(opt);

    opt.createText(m_font, 25, sf::Color::Black, "Restart game", q, 2);
    restarts.push_back(opt);
    opt.createText(m_font, 25, sf::Color::Black, "Revert changes", q, 2);
    restarts.push_back(opt);

    opt.createText(m_font, 25, sf::Color::White, "options_input2", q, 2); // Keyboard Bindings
    inputs.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "options_input1", q, 2); // Controller Setup
    inputs.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "options_input3", q, 2); // Beat Difficulty
    inputs.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "options_back", q, 2);
    inputs.push_back(opt);

    opt.createText(m_font, 25, sf::Color::White, "options_diff1", q, 2); // Low Range
    diff_options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "options_diff2", q, 2); // High Range
    diff_options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "options_diff3", q, 2); // Apply
    diff_options.push_back(opt);
    opt.createText(m_font, 25, sf::Color::White, "options_back", q, 2);
    diff_options.push_back(opt);

    std::ifstream langfile("resources/lang/lang.txt");
    std::string buf;
    int langcount = 0;
    int page = 0;

    while (getline(langfile, buf))
    {
        std::vector<std::string> param = Func::Split(buf, '|');

        opt.createText(m_font, 25, sf::Color::White, param[1], q, 2);
        langs[page].push_back(opt);

        langIDs.push_back(atoi(param[0].c_str()));

        langcount++;

        if (langcount >= 5)
        {
            opt.createText(m_font, 25, sf::Color::White, "nav_nextpage", q, 2);
            langs[page].push_back(opt);

            opt.createText(m_font, 25, sf::Color::White, "options_back", q, 2);
            langs[page].push_back(opt);

            langcount = 0;
            page++;
        }
    }

    if (langcount != 0)
    {
        opt.createText(m_font, 25, sf::Color::White, "nav_nextpage", q, 2);
        langs[page].push_back(opt);

        opt.createText(m_font, 25, sf::Color::White, "options_back", q, 2);
        langs[page].push_back(opt);

        page++;
    }

    langfile.close();

    lang_pages = page;

    ///Input manager (change keybinds)
    input_manager.loadFromFile("resources/graphics/ui/options/inputmenu.png", q, 1); ///sprite

    for (int i = 0; i < 9; i++)
    {
        t_presets[i].createText(m_font, 18, sf::Color::Black, std::to_string(i + 1), q, 1);
    }

    t_igbutton.createText(m_font, 18, sf::Color::Black, "options_ingame_btn")), q, 1);
    t_assigned.createText(m_font, 18, sf::Color::Black, "options_assign_key")), q, 1);

    for (int i = 0; i < 12; i++)
    {
        t_igkey[i].createText(m_font, 18, sf::Color::Black, "options_key")), q, 1);
        t_askey[i].createText(m_font, 18, sf::Color::Black, "", q, 1);
    }

    t_im_tip.createText(m_font, 18, sf::Color::White, "options_tip1")), q, 1); ///input manager's tip

    t_change_title.createText(m_font, 28, sf::Color::White, "options_change_title")), q, 1);
    t_change_button.createText(m_font, 62, sf::Color::White, "options_change_button")), q, 1);
    t_change_anykey.createText(m_font, 28, sf::Color::White, "options_change_anykey")), q, 1); ///change dialog

    ///Controller setup
    t_cs_title.createText(m_font, 26, sf::Color::White, "options_input1")), q, 1);
    t_cs_desc.createText(m_font, 16, sf::Color::White, "options_input_setup1")), q, 1);
    t_cs_bigbutton.createText(m_font, 30, sf::Color::White, "options_bigbutton")), q, 1);
    t_cs_tip.createText(m_font, 15, sf::Color::White, "options_tip2")), q, 1);

    */
    SPDLOG_DEBUG("Options menu initialized.");
    initialized = true;
}

void OptionsMenu::SelectMenuOption()
{
    if (sel != -1)
    {
        int newState = 0;

        if (state == 0)
        {
            newState = sel + 1;
        } else
        {
            newState = state * 10 + (sel + 1);
        }

        if (newState > state)
            prevStates.push_back(state);

        state = newState;
    }

    sel = 0;

    SPDLOG_INFO("State switched to {}", state);
}

void OptionsMenu::GoBackMenuOption(int a)
{
    if (a > 1)
    {
        diff_sel = 0;
    }

    if (prevStates.size() > a - 1)
    {
        state = prevStates[prevStates.size() - a];

        for (int i = 0; i < a; i++)
            prevStates.pop_back();
    } else
    {
        state = 0;
    }

    sel = 0;

    SPDLOG_INFO("State switched to ", state);
}

void OptionsMenu::SetConfigValue(std::string key, std::string value, bool selectmenu)
{
    Config* config = CoreManager::getInstance().getConfig();

    if (config->GetString(key) != value)
    {
        SPDLOG_DEBUG("Changing key {} to {}", key, value);

        ///Save the original key first, if not already saved
        bool found = false;

        for (int v = 0; v < original_config.size(); v++)
        {
            if (original_config[v].index == key)
                found = true;
        }

        if (!found)
        {
            SPDLOG_DEBUG("Past changes not found; save original config");

            ConfigValue tmp;
            tmp.index = key;
            tmp.value = config->GetString(key);
            original_config.push_back(tmp);
        }

        ///Make the changes
        config->SetString(key, value);

        if (key != "textureQuality")
        madeChanges = true;
    }

    ///Change the state
    if (selectmenu)
        SelectMenuOption();
}

void OptionsMenu::EventFired(sf::Event event)
{
    /* if (event.type == sf::Event::KeyPressed)
    {

    } else if (event.type == sf::Event::MouseButtonReleased)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            if ((state != 31) && (state != 32) && (state != 33))
                SelectMenuOption();
        }
    } else if (event.type == sf::Event::MouseMoved)
    {
        mouseX = event.mouseMove.x;
        mouseY = event.mouseMove.y;
    }*/
}

void OptionsMenu::Update()
{

}

void OptionsMenu::Back()
{
    StateManager::getInstance().setState(StateManager::MAINMENU);
    OnExit();
}
void OptionsMenu::OnExit()
{
    /// when we exit the options menu, we should save the user's settings to a text file
}
void OptionsMenu::UpdateButtons()
{
    /// this should update the text on all the buttons
}
void OptionsMenu::Show()
{
    is_active = true;
    /*Menu::Show();
    buttonList.Show();
    t_title.setString("menu_button_3")));
    t_title.setOrigin(t_title.getGlobalBounds().size.x/2,t_title.getGlobalBounds().size.y/2);
    t_disclaimer.setString("option_disclaimer")));
    t_disclaimer.setOrigin(t_disclaimer.getGlobalBounds().size.x/2,t_disclaimer.getGlobalBounds().size.y/2);*/
}
OptionsMenu::~OptionsMenu()
{
    //dtor
}
