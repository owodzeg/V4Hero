#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H
#include "../Config.h"
#include "../Dialog/DialogBox.h"
#include "../Graphics/Menu.h"
#include "../Graphics/PSprite.h"
#include "../Graphics/PataText.h"
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

    PataText t_title;

    PataText t_disclaimer;
    PataText t_pressToContinue;
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

    PataText options_header;
    std::vector<PataText> options;              ///main options
    std::vector<PataText> g_options;            /// graphics options
    std::vector<PataText> a_options;            /// audio options
    std::map<int, std::vector<PataText>> langs; ///languages (with pages)
    std::vector<Resolution> float_resolutions;
    std::vector<PataText> resolutions;
    std::vector<float> float_framerates;
    std::vector<PataText> framerates;
    std::vector<PataText> qualities;
    std::vector<PataText> ms_volume;
    std::vector<PataText> switches;
    std::vector<ConfigValue> original_config; ///If the changes are reverted, keep the original settings to return them back
    std::vector<PataText> restarts;
    std::vector<PataText> inputs;
    std::vector<PataText> diff_options;
    PataText t_restart;

    PSprite dg_restart, dg_select;

    ///Input manager (change keybinds)
    PSprite input_manager;                                  ///sprite
    PataText t_presets[9];                                     ///preset numbers
    PataText t_igbutton, t_assigned;                           ///titles
    PataText t_igkey[12], t_askey[12];                         ///keys
    PataText t_im_tip;                                         ///input manager's tip
    PataText t_change_title, t_change_button, t_change_anykey; ///change dialog
    sf::RectangleShape block;

    int currentPreset = 0;
    int currentOption = 0;
    bool changeInput = false;

    std::vector<std::string> ingame_buttons = {"Square", "Circle", "Cross", "Triangle", "LTrigger", "RTrigger", "Left", "Right", "Up", "Down", "Start", "Select"};
    std::vector<std::string> assigned_names = {"Unknown", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "Num0", "Num1", "Num2", "Num3", "Num4", "Num5", "Num6", "Num7", "Num8", "Num9", "Escape", "LControl", "LShift", "LAlt", "LSystem", "RControl", "RShift", "RAlt", "RSystem", "Menu", "LBracket", "RBracket", "Semicolon", "Comma", "Period", "Quote", "Slash", "Backslash", "Tilde", "Equal", "Hyphen", "Space", "Enter", "Backspace", "Tab", "PageUp", "PageDown", "End", "Home", "Insert", "Delete", "Add", "Subtract", "Multiply", "Divide", "Left", "Right", "Up", "Down", "Numpad0", "Numpad1", "Numpad2", "Numpad3", "Numpad4", "Numpad5", "Numpad6", "Numpad7", "Numpad8", "Numpad9", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15", "Pause"};

    ///Controller setup
    PataText t_cs_title, t_cs_desc, t_cs_bigbutton, t_cs_tip;
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
    void OnExit();
    void Back();
    void UpdateButtons();
    void Show();
    OptionsMenu();
    ~OptionsMenu();
};


#endif // OPTIONSMENU_H
