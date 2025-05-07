#ifndef DIALOGBOX_H
#define DIALOGBOX_H

#include "../Graphics/PSprite.h"
#include "../Graphics/PataText.h"
#include "../Input/InputController.h"
#include "RoundedRect.h"
#include <SFML/Graphics.hpp>
#include <functional>

class PataDialogBox
{
public:
    struct Option
    {
        std::string text;
        std::function<void()> callback;
    };

    int id = 0; ///for finding what dialog box is what

    RoundedRect rr_main;
    RoundedRect rr_shadow;

    PataText t_dialogType, t_dialogText;
    std::vector<std::function<void()>> callbacks;

    std::vector<Option> options;
    PataText t_option;

    sf::RectangleShape highlight;
    unsigned int option = 0;

    PSprite arrow;

    float arrow_x;

    float width = 0, height = 0;
    float x = 0, y = 0;

    bool closed = false;

    PataDialogBox* m_parent = nullptr;
    bool has_child = false;

    PataDialogBox();
    PataDialogBox(std::string main_text, std::vector<Option> opt, int type = 1);
    PataDialogBox(std::string main_text, std::vector<Option> opt, int type, std::unique_ptr<PataDialogBox>& parent);
    void Create(std::string main_text, std::vector<Option> opt, int type = 1);
    void SetType(int type);
    void SetMainText(std::string text);
    // this will add a new option and add a callback to a function
    void AddOption(std::string option_text, std::function<void()> callback);
    void Readjust();
    int CheckSelectedOption();
    void MoveUp();
    void MoveDown();
    void Execute();
    void Close();
    void HandleInput();
    void Draw();
};

#endif // DIALOGBOX_H
