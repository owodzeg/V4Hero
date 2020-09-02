#include "MessageCloud.h"

#include <iostream>

using namespace std;

MessageCloud::MessageCloud()
{

}

void MessageCloud::setSize(float new_x, float new_y)
{
    cout << "MessageCloud::setSize(" << new_x << " " << new_y << "): scale ";

    ///default x = 1080
    ///new_x = 540
    ///need to approprietly scale it

    float scale_x = 1080.f / new_x;
    float scale_y = 250.f / new_y;

    cout << scale_x << " " << scale_y << endl;

    cloud[0].setScale(1.f / scale_x, 1.f / scale_y);
    cloud[1].setScale(-1.f / scale_x, 1.f / scale_y);
    cloud[2].setScale(1.f / scale_x, -1.f / scale_y);
    cloud[3].setScale(-1.f / scale_x, -1.f / scale_y);

    xsize = new_x;
    ysize = new_y;
}

void MessageCloud::Create(int speed, sf::Vector2f start_pos, sf::Color color, int q)
{
    cout << "MessageCloud::Create()" << endl;

    timeout = speed;
    regular_timeout = timeout;
    quality = q;

    font.loadFromFile("resources/fonts/p4kakupop-pro.ttf");

    cloud[0].loadFromFile("resources/graphics/ui/dialog/message.png", q);
    cloud[1] = cloud[0];
    cloud[2] = cloud[0];
    cloud[3] = cloud[0];

    cloud[0].setOrigin(cloud[0].getLocalBounds().width, cloud[0].getLocalBounds().height);
    cloud[1].setOrigin(cloud[1].getLocalBounds().width, cloud[1].getLocalBounds().height);
    cloud[2].setOrigin(cloud[2].getLocalBounds().width, cloud[2].getLocalBounds().height);
    cloud[3].setOrigin(cloud[3].getLocalBounds().width, cloud[3].getLocalBounds().height);

    cloud[1].setScale(-1,1);
    cloud[2].setScale(1,-1);
    cloud[3].setScale(-1,-1);

    for(int i=0; i<4; i++)
    cloud[i].setColor(color);

    cur_color = color;

    cross.loadFromFile("resources/graphics/ui/dialog/cross.png", q, 1);
    cross_highlight.loadFromFile("resources/graphics/ui/dialog/crosshighlight.png", q, 1);
    cross_arrow.loadFromFile("resources/graphics/ui/dialog/crossarrow.png", q, 1);

    triangle.setPointCount(3);
    triangle.setFillColor(cur_color);

    startpos = start_pos;

    cout << "MessageCloud::Create(): finished" << endl;
}

void MessageCloud::AddDialog(sf::String text, bool nextdialog)
{
    cout << "MessageCloud::AddDialog()" << endl;

    PText a,b;

    a.createText(font, 28, sf::Color(32,32,32,255), text, quality, 1);
    b.createText(font, 28, sf::Color(32,32,32,255), "", quality, 1);

    ptext.push_back(a);
    showtext.push_back(b);

    loaded_text.push_back(text);
    viewed_text.push_back("");

    next_dialog.push_back(nextdialog);

    cout << "MessageCloud::AddDialog(): finished" << endl;
}

void MessageCloud::Show()
{
    if(!active)
    {
        cout << "MessageCloud::Show()" << endl;

        active = true;

        dest_xsize = ptext[cur_dialog].getLocalBounds().width+64+(ptext[cur_dialog].getLocalBounds().width / 10);
        dest_ysize = ptext[cur_dialog].getLocalBounds().height+64+(ptext[cur_dialog].getLocalBounds().height / 1.5);

        text_timeout.restart();

        cout << "MessageCloud::Show(): finished" << endl;
    }
}

void MessageCloud::Hide()
{
    active = false;
}

void MessageCloud::NextDialog()
{
    if(cur_dialog < ptext.size()-1)
    {
        ready = false;

        cur_dialog++;

        cur_char = 0;

        old_xsize = dest_xsize;
        old_ysize = dest_ysize;

        dest_xsize = ptext[cur_dialog].getLocalBounds().width+64+(ptext[cur_dialog].getLocalBounds().width / 10);
        dest_ysize = ptext[cur_dialog].getLocalBounds().height+64+(ptext[cur_dialog].getLocalBounds().height / 1.5);

        text_timeout.restart();
    }
    else
    {
        dest_xsize = 0;
        dest_ysize = 0;

        done = true;
    }
}

void MessageCloud::SpeedUp()
{
    speedup = true;
}

void MessageCloud::Draw(sf::RenderWindow& window, float fps, InputController& inputCtrl)
{
    if(!firstrender)
    firstrender = true;

    if(inputCtrl.isKeyPressed(InputController::Keys::START))
    Show();

    if(inputCtrl.isKeyHeld(InputController::Keys::CIRCLE))
    SpeedUp();

    if(ready)
    {
        if(inputCtrl.isKeyPressed(InputController::Keys::CROSS))
        {
            NextDialog();
        }
    }

    for(int i=0; i<ptext.size(); i++)
    ptext[i].update(window);

    if(active)
    {
        x = startpos.x-xsize/40;
        y = startpos.y-ysize;

        float xsize_diff = (dest_xsize - xsize) * 10 / fps;
        float ysize_diff = (dest_ysize - ysize) * 10 / fps;

        cout << "diff: " << xsize << " " << ysize << " " << dest_xsize << " " << dest_ysize << " " << xsize_diff << " " << ysize_diff << endl;

        setSize(xsize + xsize_diff, ysize + ysize_diff);

        if(((floor(xsize) <= 1) || (floor(ysize) <= 1)) && (done))
        active = false;

        if((abs(xsize-dest_xsize) < 50) && (abs(ysize-dest_ysize) < 50))
        {
            canwrite = true;
        }
        else
        {
            canwrite = false;
            text_timeout.restart();
        }

        for(int i=0; i<4; i++)
        {
            cloud[i].setColor(cur_color);
            cloud[i].setPosition(x,y);
            cloud[i].draw(window);
        }

        if(canwrite)
        {
            if(loaded_text[cur_dialog].getSize() != viewed_text[cur_dialog].getSize())
            {
                float ms = text_timeout.getElapsedTime().asMilliseconds();

                if(speedup)
                timeout = speed_timeout;
                else
                timeout = regular_timeout;

                int addchar = floor(ms / timeout);

                for(int i=0; i<addchar; i++)
                {
                    ///second check to not add out of bounds characters
                    if(loaded_text[cur_dialog].getSize() != viewed_text[cur_dialog].getSize())
                    {
                        viewed_text[cur_dialog] += loaded_text[cur_dialog][cur_char];
                        cur_char++;

                        cout << "adding letter " << cur_char << endl;

                        text_timeout.restart();
                    }
                }
            }
        }

        float rX = window.getSize().x / float(1280);
        float rY = window.getSize().y / float(720);

        triangle.setPoint(0, sf::Vector2f(startpos.x*rX, startpos.y*rY));
        triangle.setPoint(1, sf::Vector2f((x-(xsize/25))*rX,y*rY));
        triangle.setPoint(2, sf::Vector2f((x+(xsize/25))*rX,y*rY));

        triangle.setFillColor(cur_color);
        window.draw(triangle);

        if(!done)
        {

            showtext[cur_dialog].setPosition(x-ptext[cur_dialog].getLocalBounds().width/2,y-4-ptext[cur_dialog].getLocalBounds().height/2);
            showtext[cur_dialog].setString(viewed_text[cur_dialog]);
            showtext[cur_dialog].draw(window);

            if(next_dialog[cur_dialog])
            {
                if(loaded_text[cur_dialog].getSize() == viewed_text[cur_dialog].getSize())
                {
                    ready = true;

                    arrow_y -= 36 / fps;

                    if(arrow_y <= -20)
                    arrow_y = -20;

                    if(arrow_timeout.getElapsedTime().asSeconds() >= 0.75)
                    {
                        arrow_y = 0;
                        arrow_timeout.restart();
                    }

                    cross.setOrigin(cross.getLocalBounds().width/2, cross.getLocalBounds().height/2);
                    cross_highlight.setOrigin(cross_highlight.getLocalBounds().width/2, cross_highlight.getLocalBounds().height/2);
                    cross_arrow.setOrigin(cross_highlight.getLocalBounds().width/2, cross_highlight.getLocalBounds().height);

                    float cross_x = x+xsize/2-8;
                    float cross_y = y+ysize/2-20;

                    cross.setPosition(cross_x,cross_y);
                    cross_highlight.setPosition(cross_x,cross_y);
                    cross_arrow.setPosition(cross_x-2,cross_y+20+arrow_y);

                    cross_highlight.setColor(sf::Color(255,255,255,128+(arrow_y*6.4)));

                    cross.draw(window);
                    cross_highlight.draw(window);
                    cross_arrow.draw(window);
                }
            }
        }

        speedup = false;
    }
}
