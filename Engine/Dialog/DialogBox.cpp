#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "DialogBox.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include "../CoreManager.h"
#include "../Func.h"
#include "../Input/InputController.h"

PataDialogBox::PataDialogBox()
{
    SPDLOG_DEBUG("Creating new empty PataDialogBox");
    arrow.loadFromFile("resources/graphics/ui/dialog/arrow.png");
    SetType(1);
}

PataDialogBox::PataDialogBox(std::string main_text, std::vector<Option> opt, int type)
{
    SPDLOG_DEBUG("Creating new PataDialogBox");
    arrow.loadFromFile("resources/graphics/ui/dialog/arrow.png");
    Create(main_text, opt, type);
}

PataDialogBox::PataDialogBox(std::string main_text, std::vector<Option> opt, int type, std::unique_ptr<PataDialogBox>& parent)
{
    SPDLOG_DEBUG("Creating new PataDialogBox");
    arrow.loadFromFile("resources/graphics/ui/dialog/arrow.png");
    Create(main_text, opt, type);

    m_parent = parent.get();
    m_parent->has_child = true;
}

void PataDialogBox::Create(std::string main_text, std::vector<Option> opt, int type)
{
    SetType(type);
    SetMainText(main_text);
    options = opt;
}


void PataDialogBox::SetType(int type)
{
    auto* strRepo = CoreManager::getInstance().getStrRepo();
    std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    switch (type)
    {
        case 1:
        case 3: {
            t_dialogType.defaultStyleSetFont(font);
            t_dialogType.defaultStyleSetCharSize(48);
            t_dialogType.append(Func::GetStrFromKey("dialog_category1"));
            break;
        }
        case 2: {
            t_dialogType.defaultStyleSetFont(font);
            t_dialogType.defaultStyleSetCharSize(48);
            t_dialogType.append(Func::GetStrFromKey("dialog_category2"));
            break;
        }
    }
}

void PataDialogBox::SetMainText(std::string text)
{
    auto* strRepo = CoreManager::getInstance().getStrRepo();
    std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    t_dialogText.defaultStyleSetFont(font);
    t_dialogText.defaultStyleSetCharSize(90);
    t_dialogText.append(text);
}

// this will add a new option and add a callback to a function
void PataDialogBox::AddOption(std::string option, std::function<void()> callback)
{
    options.push_back({option, callback});
    option = options.size() - 1;
}

void PataDialogBox::Readjust()
{
    width = 0;
    height = 0;

    for (unsigned int i = 0; i < options.size(); i++)
    {
        auto* strRepo = CoreManager::getInstance().getStrRepo();
        std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

        t_option.defaultStyleSetFont(font);
        t_option.defaultStyleSetCharSize(66);
        t_option.defaultStyleSetColor(sf::Color::Black);
        t_option.reset();
        t_option.append(options[i].text);
        
        if (t_option.getGlobalBounds().size.x > width)
            width = t_option.getGlobalBounds().size.x;

        height += 90;
    }

    if (t_dialogText.getGlobalBounds().size.x > width)
        width = t_dialogText.getGlobalBounds().size.x;

    height += t_dialogText.getGlobalBounds().size.y;

    height += 120; ///gap for options
}

int PataDialogBox::CheckSelectedOption()
{
    return option;
}

void PataDialogBox::MoveUp()
{
    if (option > 0)
        option--;
}

void PataDialogBox::MoveDown()
{
    if (option < options.size() - 1)
        option++;
}

void PataDialogBox::Execute()
{
    // i want this function to execute a function that would be attached to the currently selected option
    SPDLOG_DEBUG("Execute called. Looking for function to call back.");
    if (option < options.size())
    {
        SPDLOG_DEBUG("Found function to call back.");
        options[option].callback();
    } else
    {
        SPDLOG_DEBUG("No function found to call back.");
    }

    if (!has_child)
        Close();
}

void PataDialogBox::Close()
{
    closed = true;

    if (m_parent != nullptr)
        m_parent->Close();
}

void PataDialogBox::HandleInput()
{
    InputController* inputCtrl = CoreManager::getInstance().getInputController();

    if (!closed)
    {
        if (inputCtrl->isKeyPressed(Input::Keys::UP))
            MoveUp();

        if (inputCtrl->isKeyPressed(Input::Keys::DOWN))
            MoveDown();

        if (inputCtrl->isKeyPressed(Input::Keys::CROSS))
            Execute();
    }
}

void PataDialogBox::Draw()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    float fps = CoreManager::getInstance().getCore()->getFPS();

    if (!closed)
    {
        Readjust();

        rr_shadow.Create(width + 2, height + 2, 60, sf::Color(0, 0, 0, 96));
        rr_shadow.x = x - 1;
        rr_shadow.y = y - 1;
        rr_shadow.setOrigin(sf::Vector2f((width + 120) / 2, (height + 120) / 2));
        rr_shadow.Draw();

        rr_main.Create(width, height, 60);
        rr_main.x = x;
        rr_main.y = y;
        rr_main.setOrigin(sf::Vector2f((width + 120) / 2, (height + 120) / 2));
        rr_main.Draw();

        t_dialogType.setGlobalPosition(x - rr_main.orx + 30, y - rr_main.ory);
        t_dialogType.draw();

        t_dialogText.setGlobalOrigin(t_dialogText.getGlobalBounds().size.x / 2, 0);
        t_dialogText.setGlobalPosition(x, y - rr_main.ory + t_dialogType.getGlobalBounds().size.y);
        t_dialogText.draw();

        highlight.setSize(sf::Vector2f((width + 120) * CoreManager::getInstance().getCore()->resRatio, 90 * CoreManager::getInstance().getCore()->resRatio));
        highlight.setFillColor(sf::Color(0, 200, 0, 255));
        highlight.setOrigin({highlight.getLocalBounds().size.x / 2, 0});
        highlight.setPosition(sf::Vector2f((x) *CoreManager::getInstance().getCore()->resRatio, (y - rr_main.ory + 122 + t_dialogType.getGlobalBounds().size.y + t_dialogText.getGlobalBounds().size.y + (option * 90)) * CoreManager::getInstance().getCore()->resRatio));
        window->draw(highlight);

        arrow_x -= 9 / fps;

        if (arrow_x <= -9)
            arrow_x = 0;

        arrow.setOrigin(arrow.getLocalBounds().size.x / 2, arrow.getLocalBounds().size.y / 2);
        arrow.setPosition(x - rr_main.orx + arrow_x, y - rr_main.ory + 176 + t_dialogType.getGlobalBounds().size.y + t_dialogText.getGlobalBounds().size.y + (option * 90));
        arrow.draw();

        for (unsigned int i = 0; i < options.size(); i++)
        {
            auto* strRepo = CoreManager::getInstance().getStrRepo();
            std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

            t_option.defaultStyleSetFont(font);
            t_option.defaultStyleSetCharSize(66);
            t_option.defaultStyleSetColor(sf::Color::Black);
            t_option.reset();
            t_option.append(options[i].text);
            t_option.setGlobalOrigin(t_option.getGlobalBounds().size.x / 2, 0);
            t_option.setGlobalPosition(x, y - rr_main.ory + 122 + t_dialogType.getGlobalBounds().size.y + t_dialogText.getGlobalBounds().size.y + (i * 90));
            t_option.draw();
        }
    }
}
