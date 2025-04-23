#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "MessageCloud.h"
#include <iostream>
#include <math.h>
#include <spdlog/spdlog.h>
#include "../CoreManager.h"

using namespace std;

MessageCloud::MessageCloud()
{
}

void MessageCloud::setSize(float new_x, float new_y)
{
    xsize = new_x;
    ysize = new_y;
}

void MessageCloud::Create(int speed, sf::Vector2f start_pos, sf::Color color, bool can_speedup, int q)
{
    SPDLOG_DEBUG("MessageCloud::Create()");

    timeout = speed;
    regular_timeout = timeout;
    quality = q;

    //speedable = can_speedup;
    speedable = true; //unskippable dialogues are dumb, let the player speed up anything

    cloud.load("resources/graphics/ui/dialog/message.png");
    cloud.setOrigin(cloud.getLocalBounds().size.x, cloud.getLocalBounds().size.y);
    cloud.setColor(color);

    cur_color = color;

    cross.loadFromFile("resources/graphics/ui/dialog/cross.png", q);
    cross_highlight.loadFromFile("resources/graphics/ui/dialog/crosshighlight.png", q);
    cross_arrow.loadFromFile("resources/graphics/ui/dialog/crossarrow.png", q);

    triangle.setPointCount(3);
    triangle.setFillColor(cur_color);

    startpos = start_pos;

    StringRepository* strRepo = CoreManager::getInstance().getStrRepo();
    //TO-DO: PataText rework
    /* dialogue_ptext.defaultStyleSetFont(strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage()));
    dialogue_ptext.setTextQuality(q);
    dialogue_ptext.defaultStyleSetCharSize(fontSize);
    dialogue_ptext.setString("");

    visual_ptext.defaultStyleSetFont(strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage()));
    visual_ptext.setTextQuality(q);
    visual_ptext.defaultStyleSetCharSize(fontSize);
    visual_ptext.setString("");

    visual_ptext.force_nonspeech = true; */

    SPDLOG_DEBUG("MessageCloud::Create(): finished");
}

void MessageCloud::AdditionalData(sf::String add_data)
{
    additional[dialogue_strings.size()-1].push_back(add_data);
}

void MessageCloud::setFontSize(int newFontSize)
{
    fontSize = newFontSize;
}

void MessageCloud::AddDialog(sf::String text, bool nextdialog)
{
    // here i dont think we need to use more than one ptext anymore
    // just push the new dialogues into a vector of sf::Strings and let PataText parse it :)

    SPDLOG_DEBUG("MessageCloud::AddDialog(): {}, nextdialog?: {}", std::string(text), nextdialog);

    dialogue_strings.push_back(text);
    next_dialog.push_back(nextdialog);

    SPDLOG_DEBUG("MessageCloud::AddDialog(): finished");
}

void MessageCloud::Show()
{
    if (!active)
    {
        SPDLOG_DEBUG("MessageCloud::Show()");

        active = true;
        done = false;

        if (dialogue_strings.size() <= 0)
        {
            SPDLOG_ERROR("No messages found in message cloud.");
            AddDialog(sf::String("no message"), true);
        }

        //TO-DO: PataText rework
        /*
        dialogue_ptext.append(Func::GetStrFromKey(dialogue_strings[cur_dialog]);
        visual_ptext.append(Func::GetStrFromKey(dialogue_strings[cur_dialog]);

        for(auto x:additional[cur_dialog])
        {
            dialogue_ptext.append(x);
            visual_ptext.append(x);
        }

        dialogue_ptext.draw();
        visual_ptext.draw();

        dest_xsize = visual_ptext.getLocalBounds().size.x + 120 + (visual_ptext.getLocalBounds().size.x / 30);
        dest_ysize = visual_ptext.getLocalBounds().size.y + 150 + (visual_ptext.getLocalBounds().size.y / 4.5);
        */

        text_timeout.restart();

        SPDLOG_DEBUG("MessageCloud::Show(): finished");
    }
}

void MessageCloud::Hide()
{
    active = false;
}

void MessageCloud::End()
{
    dest_xsize = 0;
    dest_ysize = 0;

    done = true;
    interrupt = true;
}

void MessageCloud::NextDialog()
{
    if (cur_dialog < dialogue_strings.size() - 1)
    {
        ready = false;

        cur_dialog++;

        cur_char = 0;

        old_xsize = dest_xsize;
        old_ysize = dest_ysize;

        dialogue_ptext.append(dialogue_strings[cur_dialog]);
        visual_ptext.append(dialogue_strings[cur_dialog]);

        dialogue_ptext.draw();
        visual_ptext.draw();

        dest_xsize = visual_ptext.getGlobalBounds().size.x + 120 + (visual_ptext.getGlobalBounds().size.x / 30);
        dest_ysize = visual_ptext.getGlobalBounds().size.y + 150 + (visual_ptext.getGlobalBounds().size.y / 4.5);

        text_timeout.restart();
    } else
    {
        // TO-DO: fix
        //if(dialogue_ptext.goback)
        //{
        //    goback = true;
        //}

        dest_xsize = 0;
        dest_ysize = 0;

        done = true;
    }
}

void MessageCloud::SpeedUp()
{
    speedup = true;
}

void MessageCloud::Draw()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    InputController* inputCtrl = CoreManager::getInstance().getInputController();
    StringRepository* strRepo = CoreManager::getInstance().getStrRepo();
    float fps = CoreManager::getInstance().getCore()->getFPS();

    if(cur_lang != strRepo->GetCurrentLanguage())
    {
        active = false;
        firstrender = false;

        cur_lang = strRepo->GetCurrentLanguage();
        //dialogue_ptext.defaultStyleSetFont(strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage()));
        //visual_ptext.defaultStyleSetFont(strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage()));

        Show();

    }

    if (!firstrender)
        firstrender = true;

    if (speedable)
    {
        // here we should do something like
        // ptext timeout = 20ms=50letters/s temporarily
        // then when key is not held, return to normal speed

        if (inputCtrl->isKeyHeld(Input::Keys::CIRCLE))
            SpeedUp();
    }

    if (ready)
    {
        // advance current text id

        if (inputCtrl->isKeyPressed(Input::Keys::CROSS))
        {
            NextDialog();
        }
    }

    if (active)
    {
        x = startpos.x - xsize / 40;
        y = startpos.y - ysize;

        float xsize_diff = (dest_xsize - xsize) * 10 / fps;
        float ysize_diff = (dest_ysize - ysize) * 10 / fps;

        //cout << "diff: " << xsize << " " << ysize << " " << dest_xsize << " " << dest_ysize << " " << xsize_diff << " " << ysize_diff << endl;
        //SPDLOG_INFO("startposx {} startposy {} xsize {} ysize {} dest_xsize {} dest_ysize {} xsize_diff {} ysize_diff {}", startpos.x, startpos.y, xsize, ysize, dest_xsize, dest_ysize, xsize_diff, ysize_diff);

        setSize(xsize + xsize_diff, ysize + ysize_diff);

        if (((floor(xsize) <= 1) || (floor(ysize) <= 1)) && (done))
        {
            if(goback && !interrupt)
            {
                SPDLOG_DEBUG("Go back to first message");
                cur_dialog = 0;
                done = false;
                active = false;
                ready = false;
                Show();
            }
            else
            {
                active = false;
            }
        }

        /// adjusting the size of clouds and drawing them
        /// they are no longer an array, instead we just transform the single texture

        float scale_x = 3240.f / xsize;
        float scale_y = 750.f / ysize;

        cloud.setColor(cur_color);
        cloud.setPosition(x, y);

        cloud.setScale(1.f / scale_x, 1.f / scale_y);
        cloud.draw();

        cloud.setScale(-1.f / scale_x, 1.f / scale_y);
        cloud.draw();

        cloud.setScale(1.f / scale_x, -1.f / scale_y);
        cloud.draw();

        cloud.setScale(-1.f / scale_x, -1.f / scale_y);
        cloud.draw();

        float rX = window->getSize().x / float(3840);
        float rY = window->getSize().y / float(2160);

        triangle.setPoint(0, sf::Vector2f(startpos.x * rX - (xsize / 75) + x_start_offset, startpos.y * rY));
        triangle.setPoint(1, sf::Vector2f((x - (xsize / 25)) * rX, y * rY));
        triangle.setPoint(2, sf::Vector2f((x + (xsize / 25)) * rX, y * rY));

        triangle.setFillColor(cur_color);
        window->draw(triangle);

        if (!done)
        {
            //TO-DO: PataText rework
            //dialogue_ptext.speedup = speedup;
            //dialogue_ptext.setPosition(x - visual_ptext.getLocalBounds().size.x / 2, y - 12 - visual_ptext.getLocalBounds().size.y / 2);
            dialogue_ptext.draw();

            //loaded_text[cur_dialog].setPosition(x - ptext[cur_dialog].getLocalBounds().size.x / 2, y - 4 - ptext[cur_dialog].getLocalBounds().size.y / 2);
            //showtext[cur_dialog].setString(viewed_text[cur_dialog]);
            //loaded_text[cur_dialog].draw(window);

            //TO-DO: PataText rework
            //if (dialogue_ptext.speech_done)
            {
                ready = true;

                arrow_y -= 36 / fps;

                if (arrow_y <= -60)
                    arrow_y = -60;

                if (arrow_timeout.getElapsedTime().asSeconds() >= 0.75)
                {
                    arrow_y = 0;
                    arrow_timeout.restart();
                }

                cross.setOrigin(cross.getLocalBounds().size.x / 2, cross.getLocalBounds().size.y / 2);
                cross_highlight.setOrigin(cross_highlight.getLocalBounds().size.x / 2, cross_highlight.getLocalBounds().size.y / 2);
                cross_arrow.setOrigin(cross_highlight.getLocalBounds().size.x / 2, cross_highlight.getLocalBounds().size.y);

                float cross_x = x + xsize / 2 - 24;
                float cross_y = y + ysize / 2 - 60;

                cross.setPosition(cross_x, cross_y);
                cross_highlight.setPosition(cross_x, cross_y);
                cross_arrow.setPosition(cross_x - 6, cross_y + 60 + arrow_y);

                cross_highlight.setColor(sf::Color(255, 255, 255, 240 + (arrow_y * 8)));

                cross.draw();
                cross_highlight.draw();
                cross_arrow.draw();
            }
        }

        speedup = false;
    }
}
