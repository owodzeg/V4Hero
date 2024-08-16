#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "DialogBox.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include "../CoreManager.h"

using namespace std;

PataDialogBox::PataDialogBox()
{
}

void PataDialogBox::Create(sf::Font font, sf::String text, std::vector<sf::String> options, int qualitySetting, int type)
{
    SPDLOG_DEBUG("Creating new PataDialogBox");

    StringRepository* strRepo = CoreManager::getInstance().getStrRepo();

    switch(type)
    {
        case 1: {
            t_dialogType.createText(font, 16, sf::Color::Red, Func::ConvertToUtf8String(strRepo->GetString("dialog_category1")), qualitySetting, 1);
            break;
        }
        case 2: {
            t_dialogType.createText(font, 16, sf::Color::Red, Func::ConvertToUtf8String(strRepo->GetString("dialog_category2")), qualitySetting, 1);
            break;
        }
    }
    t_dialogText.createText(font, 30, sf::Color::Black, text, qualitySetting, 1);

    for (unsigned int i = 0; i < options.size(); i++)
    {
        PText tmp;
        tmp.createText(font, 22, sf::Color::Black, options[i], qualitySetting, 1);
        t_options.push_back(tmp);
    }

    arrow.loadFromFile("resources/graphics/ui/dialog/arrow.png", qualitySetting, 1);

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
            t_dialogType.update(window);
            t_dialogText.update(window);

            for (unsigned int i = 0; i < t_options.size(); i++)
            {
                t_options[i].update(window);
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
        t_dialogType.draw(window);

        t_dialogText.setOrigin(t_dialogText.getLocalBounds().width / 2, 0);
        t_dialogText.setPosition(x, y - rr_main.ory + t_dialogType.getLocalBounds().height);
        t_dialogText.draw(window);

        //cout << "stuff: " << rr_main.orx << " " << rr_main.ory << endl;

        highlight.setSize(sf::Vector2f((width + 120) * resRatio, 90 * resRatio));
        highlight.setFillColor(sf::Color(0, 200, 0, 255));
        highlight.setOrigin(highlight.getLocalBounds().width / 2, 0);
        highlight.setPosition((x) *resRatio, (y - rr_main.ory + 138 + t_dialogType.getLocalBounds().height + t_dialogText.getLocalBounds().height + (option * 90)) * resRatio);
        window->draw(highlight);

        arrow_x -= 3 / fps;

        if (arrow_x <= -3)
            arrow_x = 0;

        arrow.setOrigin(arrow.getLocalBounds().width / 2, arrow.getLocalBounds().height / 2);
        arrow.setPosition(x - rr_main.orx + arrow_x, y - rr_main.ory + 192 + t_dialogType.getLocalBounds().height + t_dialogText.getLocalBounds().height + (option * 90));
        arrow.draw(window);

        for (unsigned int i = 0; i < t_options.size(); i++)
        {
            t_options[i].setOrigin(t_options[i].getLocalBounds().width / 2, 0);
            t_options[i].setPosition(x, y - rr_main.ory + 138 + t_dialogType.getLocalBounds().height + t_dialogText.getLocalBounds().height + (i * 90));
            t_options[i].draw(window);
        }
    }
}
