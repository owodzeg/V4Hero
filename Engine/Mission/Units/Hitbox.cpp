#include "Hitbox.h"
#include <iostream>

using namespace std;

Hitbox::Hitbox()
{

}

void Hitbox::SetFrame(float time)
{
    Frame tmp;
    tmp.time = time;
    tmp.g_x = g_x;
    tmp.g_y = g_y;
    tmp.x = hitboxObject.left;
    tmp.y = hitboxObject.top;
    tmp.width = hitboxObject.width;
    tmp.height = hitboxObject.height;

    frames.push_back(tmp);

    std::sort(frames.begin(), frames.end(), [](auto const &a, auto const &b) { return a.time < b.time; });
}

void Hitbox::SetCustomFrame(float in_time, float in_gx, float in_gy, float in_x, float in_y, float in_width, float in_height)
{
    Frame tmp;
    tmp.time = in_time;
    tmp.g_x = in_gx;
    tmp.g_y = in_gy;
    tmp.x = in_x;
    tmp.y = in_y;
    tmp.width = in_width;
    tmp.height = in_height;

    frames.push_back(tmp);

    std::sort(frames.begin(), frames.end(), [](auto const &a, auto const &b) { return a.time < b.time; });
}

void Hitbox::SetPos(float time)
{
    //cout << "[OBJ] SETPOS called @ " << time << endl;

    for(int i=0; i<frames.size(); i++)
    {
        if(debug)
        cout << "[OBJ] Check frame " << i << ", ftime " << frames[i].time << " vs " << time << " sz: " << frames.size()-1 << " >= " << i+1 << endl;

        if(frames[i].time < time)
        {
            if(frames.size()-1 >= i+1)
            {
                ///another frame exists

                if(frames[i+1].time > time)
                {
                    if(debug)
                    cout << "[OBJ] HANDLER 1: another frame, calc inbetween" << endl;

                    ///Calculate in-between positions
                    float time_diff = frames[i+1].time - frames[i].time;
                    float time_pos = time - frames[i].time;
                    float time_percentage = time_pos / time_diff;

                    g_x = frames[i].g_x + ((frames[i+1].g_x - frames[i].g_x) * time_percentage);
                    g_y = frames[i].g_y + ((frames[i+1].g_y - frames[i].g_y) * time_percentage);

                    float t_x = frames[i].x + ((frames[i+1].x - frames[i].x) * time_percentage);
                    float t_y = frames[i].y + ((frames[i+1].y - frames[i].y) * time_percentage);
                    float t_width = frames[i].width + ((frames[i+1].width - frames[i].width) * time_percentage);
                    float t_height = frames[i].height + ((frames[i+1].height - frames[i].height) * time_percentage);

                    hitboxObject = sf::Rect<float>(t_x,t_y,t_width,t_height);

                    break;
                }
                else
                {
                    if(debug)
                    cout << "[OBJ] HANDLER 4: skip or get last pos" << endl;

                    g_x = frames[frames.size()-1].g_x;
                    g_y = frames[frames.size()-1].g_y;

                    float t_x = frames[frames.size()-1].x;
                    float t_y = frames[frames.size()-1].y;
                    float t_width = frames[frames.size()-1].width;
                    float t_height = frames[frames.size()-1].height;

                    hitboxObject = sf::Rect<float>(t_x,t_y,t_width,t_height);
                }
            }
            else
            {
                if(debug)
                cout << "[OBJ] HANDLER 2: last frame, get last pos" << endl;

                g_x = frames[i].g_x;
                g_y = frames[i].g_y;

                float t_x = frames[i].x;
                float t_y = frames[i].y;
                float t_width = frames[i].width;
                float t_height = frames[i].height;

                hitboxObject = sf::Rect<float>(t_x,t_y,t_width,t_height);

                break;
            }
        }
        else
        {
            if(debug)
            cout << "[OBJ] HANDLER 3: first frame/before first frame, get first pos" << endl;

            g_x = frames[i].g_x;
            g_y = frames[i].g_y;

            float t_x = frames[i].x;
            float t_y = frames[i].y;
            float t_width = frames[i].width;
            float t_height = frames[i].height;

            hitboxObject = sf::Rect<float>(t_x,t_y,t_width,t_height);

            break;
        }
    }
}

Hitbox::~Hitbox()
{

}
