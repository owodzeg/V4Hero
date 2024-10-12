#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "DialogBox.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include "../CoreManager.h"
#include "../Func.h"
#include "../Input/InputController.h"

using namespace std;

PataDialogBox::PataDialogBox()
{
}

void PataDialogBox::Create(std::string font, sf::String text, std::vector<sf::String> options, int qualitySetting, int type)
{
    SPDLOG_DEBUG("Creating new PataDialogBox");

    switch(type)
    {
        case 1:
        case 3: {
            t_dialogType.setFont(font);
            t_dialogType.setCharacterSize(16);
            t_dialogType.setTextQuality(qualitySetting);
            t_dialogType.setStringKey("dialog_category1");
            break;
        }
        case 2: {
            t_dialogType.setFont(font);
            t_dialogType.setCharacterSize(16);
            t_dialogType.setTextQuality(qualitySetting);
            t_dialogType.setStringKey("dialog_category2");
            break;
        }
    }
    t_dialogText.setFont(font);
    t_dialogText.setCharacterSize(30);
    t_dialogText.setTextQuality(qualitySetting);

    if(type != 3)
        t_dialogText.setStringKey(text);
    else
        t_dialogText.setString(text);

    for (unsigned int i = 0; i < options.size(); i++)
    {
        PText tmp;
        tmp.setFont(font);
        tmp.setCharacterSize(22);
        tmp.setTextQuality(qualitySetting);

        if(type != 3)
            tmp.setStringKey(options[i]);
        else
            tmp.setString(options[i]);

        t_options.push_back(tmp);
    }

    arrow.loadFromFile("resources/graphics/ui/dialog/arrow.png", qualitySetting, 3);

    option = t_options.size() - 1;
}

void PataDialogBox::CreateCustom(std::string font, sf::String text, std::vector<sf::String> options, int qualitySetting, int type)
{
    SPDLOG_DEBUG("Creating new PataDialogBox");

    switch(type)
    {
        case 1: {
            t_dialogType.setFont(font);
            t_dialogType.setCharacterSize(16);
            t_dialogType.setTextQuality(qualitySetting);
            t_dialogType.setStringKey("dialog_category1");
            break;
        }
        case 2: {
            t_dialogType.setFont(font);
            t_dialogType.setCharacterSize(16);
            t_dialogType.setTextQuality(qualitySetting);
            t_dialogType.setStringKey("dialog_category2");
            break;
        }
    }
    t_dialogText.setFont(font);
    t_dialogText.setCharacterSize(30);
    t_dialogText.setTextQuality(qualitySetting);
    t_dialogText.setStringKey(text);

    for (unsigned int i = 0; i < options.size(); i++)
    {
        PText tmp;
        tmp.setFont(font);
        tmp.setCharacterSize(22);
        tmp.setTextQuality(qualitySetting);
        tmp.setString(options[i]);
        t_options.push_back(tmp);
        options_saved.push_back(options[i]);
    }

    arrow.loadFromFile("resources/graphics/ui/arrow.png", qualitySetting, 3);

    option = t_options.size() - 1;
}

void PataDialogBox::Readjust()
{
    width = 0;
    height = 0;

    for (unsigned int i = 0; i < t_options.size(); i++)
    {
        if (t_options[i].getLocalBounds().width > width)
            width = t_options[i].getLocalBounds().width;

        height += 90;
    }

    if (t_dialogText.rendered)
    {
        if (t_dialogText.getLocalBounds().width > width)
            width = t_dialogText.getLocalBounds().width;

        height += t_dialogText.getLocalBounds().height;
    }

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
    if (option < t_options.size() - 1)
        option++;
}

void PataDialogBox::Close()
{
    closed = true;
}

void PataDialogBox::Draw()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    InputController* inputCtrl = CoreManager::getInstance().getInputController();
    float fps = CoreManager::getInstance().getCore()->getFPS();

    if (!closed)
    {
        if (inputCtrl->isKeyPressed(Input::Keys::UP))
            MoveUp();

        if (inputCtrl->isKeyPressed(Input::Keys::DOWN))
            MoveDown();

        float resRatio = window->getSize().x / float(3840);

        if (!rendered)
        {
            t_dialogType.draw();
            t_dialogText.draw();

            for (unsigned int i = 0; i < t_options.size(); i++)
            {
                t_options[i].draw();
            }

            rendered = true;
        }

        Readjust();

        rr_shadow.Create(width + 2, height + 2, 60, window->getSize().x / float(3840), sf::Color(0, 0, 0, 96));
        rr_shadow.x = x - 1;
        rr_shadow.y = y - 1;
        rr_shadow.setOrigin(sf::Vector2f((width + 120) / 2, (height + 120) / 2));
        rr_shadow.Draw();

        rr_main.Create(width, height, 60, window->getSize().x / float(3840));
        rr_main.x = x;
        rr_main.y = y;
        rr_main.setOrigin(sf::Vector2f((width + 120) / 2, (height + 120) / 2));
        rr_main.Draw();

        t_dialogType.setPosition(x - rr_main.orx + 30, y - rr_main.ory);
        t_dialogType.draw();

        t_dialogText.setOrigin(t_dialogText.getLocalBounds().width / 2, 0);
        t_dialogText.setPosition(x, y - rr_main.ory + t_dialogType.getLocalBounds().height);
        t_dialogText.draw();

        //cout << "stuff: " << rr_main.orx << " " << rr_main.ory << endl;

        highlight.setSize(sf::Vector2f((width + 120) * resRatio, 90 * resRatio));
        highlight.setFillColor(sf::Color(0, 200, 0, 255));
        highlight.setOrigin(highlight.getLocalBounds().width / 2, 0);
        highlight.setPosition((x) *resRatio, (y - rr_main.ory + 138 + t_dialogType.getLocalBounds().height + t_dialogText.getLocalBounds().height + (option * 90)) * resRatio);
        window->draw(highlight);

        arrow_x -= 9 / fps;

        if (arrow_x <= -9)
            arrow_x = 0;

        arrow.setOrigin(arrow.getLocalBounds().width / 2, arrow.getLocalBounds().height / 2);
        arrow.setPosition(x - rr_main.orx + arrow_x, y - rr_main.ory + 192 + t_dialogType.getLocalBounds().height + t_dialogText.getLocalBounds().height + (option * 90));
        arrow.draw(window);

        for (unsigned int i = 0; i < t_options.size(); i++)
        {
            t_options[i].setOrigin(t_options[i].getLocalBounds().width / 2, 0);
            t_options[i].setPosition(x, y - rr_main.ory + 138 + t_dialogType.getLocalBounds().height + t_dialogText.getLocalBounds().height + (i * 90));
            t_options[i].draw();
        }
    }
}
