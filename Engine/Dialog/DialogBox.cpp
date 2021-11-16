#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "DialogBox.h"
#include <iostream>
#include <spdlog/spdlog.h>

using namespace std;

PataDialogBox::PataDialogBox()
{
}

/*void PataDialogBox::Create(sf::Font font, std::string text, std::vector<std::string> options, int qualitySetting)
{
    t_dialogType.createText(font, 16, sf::Color::Red, "Information", qualitySetting, 1);
    t_dialogText.createText(font, 30, sf::Color::Black, text, qualitySetting, 1);

    for(int i=0; i<options.size(); i++)
    {
        PText tmp;
        tmp.createText(font, 22, sf::Color::Black, options[i], qualitySetting, 1);
        t_options.push_back(tmp);
    }

    arrow.loadFromFile("resources/graphics/ui/dialog/arrow.png", qualitySetting, 1);

    option = t_options.size()-1;
}*/

void PataDialogBox::Create(sf::Font font, sf::String text, std::vector<sf::String> options, int qualitySetting)
{
    SPDLOG_DEBUG("Creating new PataDialogBox");

    t_dialogType.createText(font, 16, sf::Color::Red, "Information", qualitySetting, 1);
    t_dialogText.createText(font, 30, sf::Color::Black, text, qualitySetting, 1);

    for (int i = 0; i < options.size(); i++)
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

    for (int i = 0; i < t_options.size(); i++)
    {
        if (t_options[i].getLocalBounds().width > width)
            width = t_options[i].getLocalBounds().width;

        height += 30;
    }

    if (t_dialogText.rendered)
    {
        if (t_dialogText.getLocalBounds().width > width)
            width = t_dialogText.getLocalBounds().width;

        height += t_dialogText.getLocalBounds().height;
    }

    height += 40; ///gap for options
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

void PataDialogBox::Draw(sf::RenderWindow& window, float fps, InputController& inputCtrl)
{
    if (!closed)
    {
        if (inputCtrl.isKeyPressed(InputController::Keys::UP))
            MoveUp();

        if (inputCtrl.isKeyPressed(InputController::Keys::DOWN))
            MoveDown();

        float resRatio = window.getSize().x / float(1280);

        if (!rendered)
        {
            t_dialogType.update(window);
            t_dialogText.update(window);

            for (int i = 0; i < t_options.size(); i++)
            {
                t_options[i].update(window);
            }

            rendered = true;
        }

        Readjust();

        rr_shadow.Create(width + 2, height + 2, 20, window.getSize().x / float(1280), sf::Color(0, 0, 0, 96));
        rr_shadow.x = x - 1;
        rr_shadow.y = y - 1;
        rr_shadow.setOrigin(sf::Vector2f((width + 40) / 2, (height + 40) / 2));
        rr_shadow.Draw(window);

        rr_main.Create(width, height, 20, window.getSize().x / float(1280));
        rr_main.x = x;
        rr_main.y = y;
        rr_main.setOrigin(sf::Vector2f((width + 40) / 2, (height + 40) / 2));
        rr_main.Draw(window);

        t_dialogType.setPosition(x - rr_main.orx + 10, y - rr_main.ory);
        t_dialogType.draw(window);

        t_dialogText.setOrigin(t_dialogText.getLocalBounds().width / 2, 0);
        t_dialogText.setPosition(x, y - rr_main.ory + t_dialogType.getLocalBounds().height);
        t_dialogText.draw(window);

        //cout << "stuff: " << rr_main.orx << " " << rr_main.ory << endl;

        highlight.setSize(sf::Vector2f((width + 40) * resRatio, 30 * resRatio));
        highlight.setFillColor(sf::Color(0, 200, 0, 255));
        highlight.setOrigin(highlight.getLocalBounds().width / 2, 0);
        highlight.setPosition((x) *resRatio, (y - rr_main.ory + 46 + t_dialogType.getLocalBounds().height + t_dialogText.getLocalBounds().height + (option * 30)) * resRatio);
        window.draw(highlight);

        arrow_x -= 3 / fps;

        if (arrow_x <= -3)
            arrow_x = 0;

        arrow.setOrigin(arrow.getLocalBounds().width / 2, arrow.getLocalBounds().height / 2);
        arrow.setPosition(x - rr_main.orx + arrow_x, y - rr_main.ory + 64 + t_dialogType.getLocalBounds().height + t_dialogText.getLocalBounds().height + (option * 30));
        arrow.draw(window);

        for (int i = 0; i < t_options.size(); i++)
        {
            t_options[i].setOrigin(t_options[i].getLocalBounds().width / 2, 0);
            t_options[i].setPosition(x, y - rr_main.ory + 46 + t_dialogType.getLocalBounds().height + t_dialogText.getLocalBounds().height + (i * 30));
            t_options[i].draw(window);
        }
    }
}
