#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "Hitbox.h"
#include <iostream>
#include <spdlog/spdlog.h>

Hitbox::Hitbox()
{
}

void Hitbox::SetFrame(float time)
{
    HitboxFrame tmp;
    tmp.time = time;
    tmp.g_x = g_x;
    tmp.g_y = g_y;
    tmp.clearVertices();
    std::vector<sf::Vector2f>* oldverticies = hitboxObject.getBaseVerticiesDontUseThisUnlessYouKnowWhy();
    for (int i = 0; i < oldverticies->size(); i++)
    {
        tmp.addVertex(oldverticies->at(i).x, oldverticies->at(i).y);
    }
    tmp.rotation = 0;

    tmp.calcMaxWidth();
    frames.push_back(tmp);

    std::sort(frames.begin(), frames.end(), [](auto const& a, auto const& b) { return a.time < b.time; });
}

void Hitbox::SetCustomFrame(float in_time, float in_gx, float in_gy, float in_x, float in_y, float angle, float in_width, float in_height)
{
    HitboxFrame tmp;
    tmp.time = in_time;
    tmp.g_x = in_gx;
    tmp.g_y = in_gy;
    tmp.clearVertices();
    tmp.addVertex(in_x - in_width / 2, in_y - in_height / 2); /// "top left"
    tmp.addVertex(in_x + in_width / 2, in_y - in_height / 2); /// "top right"
    tmp.addVertex(in_x - in_width / 2, in_y + in_height / 2); /// "bottom left"
    tmp.addVertex(in_x + in_width / 2, in_y + in_height / 2); /// "bottom right"
    tmp.rotation = angle;

    tmp.calcMaxWidth();
    frames.push_back(tmp);

    std::sort(frames.begin(), frames.end(), [](auto const& a, auto const& b) { return a.time < b.time; });
}

void Hitbox::SetPos(float time)
{
    //cout << "[OBJ] SETPOS called @ " << time << endl;

    for (int i = 0; i < frames.size(); i++)
    {
        SPDLOG_TRACE("[OBJ] Check frame {}, ftime {} vs {} sz: {} >= {}", i, frames[i].time, time, frames.size()-1, i+1);

        if (frames[i].time < time)
        {
            if (frames.size() - 1 >= i + 1)
            {
                ///another frame exists

                if (frames[i + 1].time > time)
                {
                    SPDLOG_TRACE("[OBJ] HANDLER 1: another frame, calc inbetween");

                    ///Calculate in-between positions
                    float time_diff = frames[i + 1].time - frames[i].time;
                    float time_pos = time - frames[i].time;
                    float time_percentage = time_pos / time_diff;

                    g_x = frames[i].g_x + ((frames[i + 1].g_x - frames[i].g_x) * time_percentage);
                    g_y = frames[i].g_y + ((frames[i + 1].g_y - frames[i].g_y) * time_percentage);


                    //float t_x = frames[i].x + ((frames[i+1].x - frames[i].x) * time_percentage);
                    //float t_y = frames[i].y + ((frames[i+1].y - frames[i].y) * time_percentage);
                    //float t_width = frames[i].width + ((frames[i+1].width - frames[i].width) * time_percentage);
                    //float t_height = frames[i].height + ((frames[i+1].height - frames[i].height) * time_percentage);
                    float rotation = frames[i].rotation + ((frames[i + 1].rotation - frames[i].rotation) * time_percentage);


                    HitboxFrame tmp;
                    tmp.time = time;
                    tmp.g_x = g_x;
                    tmp.g_y = g_y;
                    std::vector<sf::Vector2f>* oldverticies = frames[i].getBaseVerticiesDontUseThisUnlessYouKnowWhy();
                    std::vector<sf::Vector2f>* newvertices = frames[i + 1].getBaseVerticiesDontUseThisUnlessYouKnowWhy();
                    tmp.clearVertices();
                    for (int i = 0; i < oldverticies->size(); i++)
                    {
                        float t_x = oldverticies->at(i).x + ((newvertices->at(i).x - oldverticies->at(i).x) * time_percentage);
                        float t_y = oldverticies->at(i).y + ((newvertices->at(i).y - oldverticies->at(i).y) * time_percentage);
                        tmp.addVertex(t_x, t_y);
                    }
                    tmp.rotation = rotation;
                    tmp.calcMaxWidth();

                    hitboxObject = tmp;

                    break;
                } else
                {
                    SPDLOG_TRACE("[OBJ] HANDLER 4: skip or get last pos");

                    g_x = frames[frames.size() - 1].g_x;
                    g_y = frames[frames.size() - 1].g_y;

                    HitboxFrame tmp;
                    tmp.time = time;
                    tmp.g_x = g_x;
                    tmp.g_y = g_y;
                    tmp.clearVertices();
                    std::vector<sf::Vector2f>* oldverticies = frames[frames.size() - 1].getBaseVerticiesDontUseThisUnlessYouKnowWhy();
                    for (int i = 0; i < oldverticies->size(); i++)
                    {
                        tmp.addVertex(oldverticies->at(i).x, oldverticies->at(i).y);
                    }
                    tmp.rotation = frames[frames.size() - 1].rotation;

                    tmp.calcMaxWidth();
                    hitboxObject = tmp;
                }
            } else
            {
                SPDLOG_TRACE("[OBJ] HANDLER 2: last frame, get last pos");

                frames[i].calcMaxWidth();
                hitboxObject = frames[i];

                break;
            }
        } else
        {
            SPDLOG_TRACE("[OBJ] HANDLER 3: first frame/before first frame, get first pos");

            hitboxObject = frames[i];

            break;
        }
    }
}

HitboxFrame Hitbox::getRect()
{
    return hitboxObject;
}

sf::Vector2f Hitbox::getGlobalPosition()
{
    return sf::Vector2f(g_x, g_y);
}

Hitbox::~Hitbox()
{
}
