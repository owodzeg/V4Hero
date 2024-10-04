#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H
#include "../Config.h"
#include "../Dialog/DialogBox.h"
#include "../Graphics/Menu.h"
#include "../Graphics/PSprite.h"
#include "../Graphics/PText.h"
#include "../Input/InputController.h"
#include "../Mission/MissionController.h"
#include <SFML/Graphics.hpp>

class V4Core;
class OptionsMenu : public Menu
{
public:
    sf::RectangleShape mm_bigBox;
    sf::RectangleShape mm_smallerBox;
    sf::RectangleShape mm_titleBox;

    sf::Text t_title;

    sf::Text t_disclaimer;
    sf::Text t_pressToContinue;
    bool madeChanges;

    PSprite bg, sword;
    // PSprite handle, aura1, aura2, l_fire1, l_fire2, l_fire3, r_fire1, r_fire2, r_fire3;

    float mouseX = 0, mouseY = 0;
    std::vector<int> prevStates;
    int state = 0;
    int diff_sel = 0;
    int sel = -1;
    int maxSel = 0;

    struct Resolution {
        float width;
        float height;
    };

    struct ConfigValue {
        std::string index;
        std::string value;
    };

    PText options_header;
    std::vector<PText> options;              ///main options
    std::vector<PText> g_options;            /// graphics options
    std::vector<PText> a_options;            /// audio options
    std::map<int, std::vector<PText>> langs; ///languages (with pages)
    std::vector<Resolution> float_resolutions;
    std::vector<PText> resolutions;
    std::vector<float> float_framerates;
    std::vector<PText> framerates;
    std::vector<PText> qualities;
    std::vector<PText> ms_volume;
    std::vector<PText> switches;
    std::vector<ConfigValue> original_config; ///If the changes are reverted, keep the original settings to return them back
    std::vector<PText> restarts;
    std::vector<PText> inputs;
    std::vector<PText> diff_options;
    PText t_restart;

    PSprite dg_restart, dg_select;

    PataDialogBox restart_prompt;

    ///Input manager (change keybinds)
    PSprite input_manager;                                  ///sprite
    PText t_presets[9];                                     ///preset numbers
    PText t_igbutton, t_assigned;                           ///titles
    PText t_igkey[12], t_askey[12];                         ///keys
    PText t_im_tip;                                         ///input manager's tip
    PText t_change_title, t_change_button, t_change_anykey; ///change dialog
    sf::RectangleShape block;

    int currentPreset = 0;
    int currentOption = 0;
    bool changeInput = false;

    std::vector<std::string> ingame_buttons = {"Square", "Circle", "Cross", "Triangle", "LTrigger", "RTrigger", "Left", "Right", "Up", "Down", "Start", "Select"};
    std::vector<std::string> assigned_names = {"Unknown", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "Num0", "Num1", "Num2", "Num3", "Num4", "Num5", "Num6", "Num7", "Num8", "Num9", "Escape", "LControl", "LShift", "LAlt", "LSystem", "RControl", "RShift", "RAlt", "RSystem", "Menu", "LBracket", "RBracket", "Semicolon", "Comma", "Period", "Quote", "Slash", "Backslash", "Tilde", "Equal", "Hyphen", "Space", "Enter", "Backspace", "Tab", "PageUp", "PageDown", "End", "Home", "Insert", "Delete", "Add", "Subtract", "Multiply", "Divide", "Left", "Right", "Up", "Down", "Numpad0", "Numpad1", "Numpad2", "Numpad3", "Numpad4", "Numpad5", "Numpad6", "Numpad7", "Numpad8", "Numpad9", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15", "Pause"};

    ///Controller setup
    PText t_cs_title, t_cs_desc, t_cs_bigbutton, t_cs_tip;
    int setup_stage = 1;
    int setup_key = -2;

    ///Languages
    int lang_pages = 0;
    int lang_current = 0;
    std::vector<int> langIDs;

    bool initialized = false;

    Menu* parentMenu;
    void Update();
    void SelectMenuOption();
    void GoBackMenuOption(int a = 2);
    void SetConfigValue(std::string key, std::string value, bool selectmenu = true);
    void EventFired(sf::Event event);
    void OnExit();
    void Back();
    void UpdateButtons();
    void Show();
    OptionsMenu();
    ~OptionsMenu();
};


#endif // OPTIONSMENU_H
