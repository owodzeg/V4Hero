#include "Object.h"
#include <iostream>

using namespace std;

Object::Object()
{
}

Object::Object(std::string mem, int xpos, int ypos, int l, int p)
{
    //cout << "Obtained a memory file of size " << mem.size() << " bytes" << endl;

    tex_obj.loadFromMemory(mem.data(), mem.size());
    tex_obj.setSmooth(true);

    s_obj.setTexture(tex_obj);

    or_x = s_obj.getGlobalBounds().width / 2;
    or_y = s_obj.getGlobalBounds().height / 2;

    x = xpos;
    y = ypos;

    layer = l;
    parent = p;
}

void Object::Load(string filename, int xpos, int ypos)
{
    //cout << "Loading object" << endl;
    texture_path = filename.substr(filename.find_last_of("\\/") + 1);
    //cout << "Texture path: " << texture_path << endl;

    if (tex_obj.loadFromFile(filename))
    {
        s_obj.setTexture(tex_obj);

        or_x = s_obj.getGlobalBounds().width / 2;
        or_y = s_obj.getGlobalBounds().height / 2;

        x = xpos;
        y = ypos;

        exported = false;

        //cout << "Object loaded successfully" << endl;
    }
}

void Object::Load(sf::Texture& texture, int xpos, int ypos)
{
    tex_obj = texture;
    s_obj.setTexture(tex_obj);

    or_x = s_obj.getGlobalBounds().width / 2;
    or_y = s_obj.getGlobalBounds().height / 2;

    x = xpos;
    y = ypos;

    exported = false;

    //cout << "Object loaded successfully" << endl;
}


void Object::LoadFromMemory(std::string mem, int xpos, int ypos)
{
    tex_obj.loadFromMemory(mem.data(), mem.size());
    tex_obj.setSmooth(true);

    s_obj.setTexture(tex_obj);

    or_x = s_obj.getGlobalBounds().width / 2;
    or_y = s_obj.getGlobalBounds().height / 2;

    x = xpos;
    y = ypos;

    exported = false;

    //cout << "Object loaded successfully" << endl;
}

void Object::swapTexture(sf::Image img)
{
    //cout << "[AnimatedObject::swapTexture A] swapping texture" << endl;
    //cout << "[Object] Object::swapTexture(): " << texture_path << endl;

    sf::Clock c;
    s_obj.t.loadFromImage(img);
    //cout << c.getElapsedTime().asMicroseconds() << "us ";
    s_obj.t.setSmooth(true);
    //cout << c.getElapsedTime().asMicroseconds() << "us ";
    s_obj.applyTexture();
    //cout << c.getElapsedTime().asMicroseconds() << "us" << endl;;
    //cout << "[AnimatedObject::swapTexture A] swapping done" << endl;
}

void Object::swapTexture(sf::Image first, vector<Pixel> px)
{
    //cout << "[AnimatedObject::swapTexture B] swapping texture" << endl;
    //cout << "[Object] Object::swapTexture(): " << texture_path << endl;

    //sf::Clock c;

    sf::Image nw = first;
    //cout << c.getElapsedTime().asMicroseconds() << "us ";
    for (int i = 0; i < px.size(); i++)
    {
        nw.setPixel(px[i].x, px[i].y, px[i].color);
    }
    //cout << c.getElapsedTime().asMicroseconds() << "us ";
    s_obj.t.loadFromImage(nw);
    //cout << c.getElapsedTime().asMicroseconds() << "us ";
    s_obj.t.setSmooth(true);
    //cout << c.getElapsedTime().asMicroseconds() << "us ";
    s_obj.applyTexture();
    //cout << c.getElapsedTime().asMicroseconds() << "us" << endl;;
    //cout << "[AnimatedObject::swapTexture B] swapping done" << endl;
}

void Object::SetFrame(float time)
{
    Frame tmp;
    tmp.time = time;
    tmp.pos_x = x;
    tmp.pos_y = y;
    tmp.or_x = or_x;
    tmp.or_y = or_y;
    tmp.rotation = r + (rs * 360);
    tmp.scale_x = s_x;
    tmp.scale_y = s_y;

    frames.push_back(tmp);

    std::sort(frames.begin(), frames.end(), [](auto const& a, auto const& b) { return a.time < b.time; });
}

void Object::SetCustomFrame(float in_time, float in_pos_x, float in_pos_y, float in_or_x, float in_or_y, float in_rotation, float in_scale_x, float in_scale_y)
{
    Frame tmp;
    tmp.time = in_time;
    tmp.pos_x = in_pos_x;
    tmp.pos_y = in_pos_y;
    tmp.or_x = in_or_x;
    tmp.or_y = in_or_y;
    tmp.rotation = in_rotation;
    tmp.scale_x = in_scale_x;
    tmp.scale_y = in_scale_y;

    frames.push_back(tmp);

    std::sort(frames.begin(), frames.end(), [](auto const& a, auto const& b) { return a.time < b.time; });
}

void Object::SetPos(float time)
{
    //cout << "[OBJ] SETPOS called @ " << time << endl;

    for (int i = 0; i < frames.size(); i++)
    {
        if (debug)
            cout << "[OBJ] Check frame " << i << ", ftime " << frames[i].time << " vs " << time << " sz: " << frames.size() - 1 << " >= " << i + 1 << endl;

        if (frames[i].time < time)
        {
            if (frames.size() - 1 >= i + 1)
            {
                ///another frame exists

                if (frames[i + 1].time > time)
                {
                    if (debug)
                        cout << "[OBJ] HANDLER 1: another frame, calc inbetween" << endl;

                    old_x = x;
                    old_y = y;
                    old_r = r;

                    ///Calculate in-between positions
                    float time_diff = frames[i + 1].time - frames[i].time;
                    float time_pos = time - frames[i].time;
                    float time_percentage = time_pos / time_diff;

                    x = frames[i].pos_x + ((frames[i + 1].pos_x - frames[i].pos_x) * time_percentage);
                    y = frames[i].pos_y + ((frames[i + 1].pos_y - frames[i].pos_y) * time_percentage);
                    r = frames[i].rotation + ((frames[i + 1].rotation - frames[i].rotation) * time_percentage);
                    or_x = frames[i].or_x + ((frames[i + 1].or_x - frames[i].or_x) * time_percentage);
                    or_y = frames[i].or_y + ((frames[i + 1].or_y - frames[i].or_y) * time_percentage);
                    s_x = frames[i].scale_x + ((frames[i + 1].scale_x - frames[i].scale_x) * time_percentage);
                    s_y = frames[i].scale_y + ((frames[i + 1].scale_y - frames[i].scale_y) * time_percentage);

                    if (frames[i + 1].pos_x == -999999)
                        disable = true;
                    else
                        disable = false;

                    break;
                } else
                {
                    if (debug)
                        cout << "[OBJ] HANDLER 4: skip or get last pos" << endl;

                    old_x = x;
                    old_y = y;
                    old_r = r;

                    int sz = frames.size() - 1;

                    x = frames[sz].pos_x;
                    y = frames[sz].pos_y;
                    r = frames[sz].rotation;
                    or_x = frames[sz].or_x;
                    or_y = frames[sz].or_y;
                    s_x = frames[sz].scale_x;
                    s_y = frames[sz].scale_y;

                    if (frames[sz].pos_x == -999999)
                        disable = true;
                    else
                        disable = false;
                }
            } else
            {
                if (debug)
                    cout << "[OBJ] HANDLER 2: last frame, get last pos" << endl;

                old_x = x;
                old_y = y;
                old_r = r;

                x = frames[i].pos_x;
                y = frames[i].pos_y;
                r = frames[i].rotation;
                or_x = frames[i].or_x;
                or_y = frames[i].or_y;
                s_x = frames[i].scale_x;
                s_y = frames[i].scale_y;

                if (frames[i].pos_x == -999999)
                    disable = true;
                else
                    disable = false;

                break;
            }
        } else
        {
            if (debug)
                cout << "[OBJ] HANDLER 3: first frame/before first frame, get first pos" << endl;

            old_x = x;
            old_y = y;
            old_r = r;

            x = frames[i].pos_x;
            y = frames[i].pos_y;
            r = frames[i].rotation;
            or_x = frames[i].or_x;
            or_y = frames[i].or_y;
            s_x = frames[i].scale_x;
            s_y = frames[i].scale_y;

            if (frames[i].pos_x == -999999)
                disable = true;
            else
                disable = false;

            break;
        }
    }

    // cout << x << " " << y << " " << r << " " << or_x << " " << or_y << " " << s_x << " " << s_y << endl;
}

void Object::SetPosFrame(float time, int frame)
{
    //debug = true;

    if (frame > 0)
        frame -= 1;

    if (!first_set)
    {
        old_x = x;
        old_y = y;
        old_r = r;

        x = frames[0].pos_x;
        y = frames[0].pos_y;
        r = frames[0].rotation;
        or_x = frames[0].or_x;
        or_y = frames[0].or_y;
        s_x = frames[0].scale_x;
        s_y = frames[0].scale_y;

        if (frames[0].pos_x == -999999)
            disable = true;
        else
            disable = false;

        first_set = true;
    }

    int sz = frames.size() - 1;

    for (int i = frame; i <= sz; i++)
    {
        if (debug)
            cout << "[OBJ] Check frame " << i << ", ftime " << frames[i].time << " vs " << time << " sz: " << frames.size() - 1 << " >= " << i + 1 << endl;

        if (frames[i].time < time)
        {
            if (sz - 1 >= i + 1)
            {
                ///another frame exists

                if (frames[i + 1].time > time)
                {
                    if (debug)
                        cout << "[OBJ] HANDLER 1: another frame, calc inbetween" << endl;

                    old_x = x;
                    old_y = y;
                    old_r = r;

                    ///Calculate in-between positions
                    float time_diff = frames[i + 1].time - frames[i].time;
                    float time_pos = time - frames[i].time;
                    float time_percentage = time_pos / time_diff;

                    x = frames[i].pos_x + ((frames[i + 1].pos_x - frames[i].pos_x) * time_percentage);
                    y = frames[i].pos_y + ((frames[i + 1].pos_y - frames[i].pos_y) * time_percentage);
                    r = frames[i].rotation + ((frames[i + 1].rotation - frames[i].rotation) * time_percentage);
                    or_x = frames[i].or_x + ((frames[i + 1].or_x - frames[i].or_x) * time_percentage);
                    or_y = frames[i].or_y + ((frames[i + 1].or_y - frames[i].or_y) * time_percentage);
                    s_x = frames[i].scale_x + ((frames[i + 1].scale_x - frames[i].scale_x) * time_percentage);
                    s_y = frames[i].scale_y + ((frames[i + 1].scale_y - frames[i].scale_y) * time_percentage);

                    if (frames[i + 1].pos_x == -999999)
                        disable = true;
                    else
                        disable = false;

                    break;
                }
            } else
            {
                if (debug)
                    cout << "[OBJ] HANDLER 2: last frame, get last pos" << endl;

                old_x = x;
                old_y = y;
                old_r = r;

                x = frames[i].pos_x;
                y = frames[i].pos_y;
                r = frames[i].rotation;
                or_x = frames[i].or_x;
                or_y = frames[i].or_y;
                s_x = frames[i].scale_x;
                s_y = frames[i].scale_y;

                if (frames[i].pos_x == -999999)
                    disable = true;
                else
                    disable = false;

                break;
            }
        } else
        {
            if (debug)
                cout << "[OBJ] HANDLER 3: first frame/before first frame, get first pos" << endl;

            old_x = x;
            old_y = y;
            old_r = r;

            x = frames[i].pos_x;
            y = frames[i].pos_y;
            r = frames[i].rotation;
            or_x = frames[i].or_x;
            or_y = frames[i].or_y;
            s_x = frames[i].scale_x;
            s_y = frames[i].scale_y;

            if (frames[i].pos_x == -999999)
                disable = true;
            else
                disable = false;

            break;
        }
    }

    // cout << x << " " << y << " " << r << " " << or_x << " " << or_y << " " << s_x << " " << s_y << endl;
}

void Object::Draw(sf::RenderWindow& window, int orx, int ory)
{
    if (!disable)
    {
        //s_obj.setTexture(tex_obj);
        s_obj.setScale(s_x + (g_sx - 1), s_y + (g_sy - 1));
        s_obj.setOrigin(or_x + orx, or_y + ory);
        s_obj.setPosition(x + g_x + gl_x, y + g_y + gl_y);
        s_obj.setRotation(g_r + r);
        s_obj.setColor(color);
        s_obj.draw(window);

        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) && (sf::Keyboard::isKeyPressed(sf::Keyboard::F9)))
        {
            if (!exported)
            {
                sf::Image img;
                img = s_obj.t.copyToImage();
                int rrr = rand() % 100000000;
                img.saveToFile("texDump/obj_" + std::to_string(rrr) + ".png");

                exported = true;
            }
        }
    }
}


void Object::Draw(sf::RenderWindow& window, int x1, int y1, int x2, int y2, int orx, int ory)
{
    if (!disable)
    {
        //cout << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
        s_obj.setTextureRect(sf::IntRect(x1, y1, x2, y2));

        //s_obj.setTexture(tex_obj);
        s_obj.setScale(s_x + (g_sx - 1), s_y + (g_sy - 1));
        s_obj.setOrigin(or_x + orx, or_y + ory);
        s_obj.setPosition(x + g_x + gl_x, y + g_y + gl_y);
        s_obj.setRotation(g_r + r);
        s_obj.setColor(color);
        s_obj.draw(window);


        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) && (sf::Keyboard::isKeyPressed(sf::Keyboard::F9)))
        {
            if (!exported)
            {
                sf::Image img;
                img = s_obj.t.copyToImage();
                int rrr = rand() % 100000000;
                img.saveToFile("texDump/obj_" + std::to_string(rrr) + ".png");

                exported = true;
            }
        }
    }
}
