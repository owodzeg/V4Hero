#include "Drum.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;

Drum::Drum()
{
}

void Drum::Load(string drum, int perfection, sf::RenderWindow& window, sf::Texture& drum_texture, std::map<std::string, vector<float>> patterns, sf::Texture& flash_texture)
{
    s_flash.setTexture(flash_texture);
    s_flash.setOrigin(s_flash.getLocalBounds().width/2,s_flash.getLocalBounds().height/2);

    cur_pattern = patterns;

    if(drum == "don")
    {
        isDon = true;
    }

    if(perfection == 0) ///BEST beat
    {
        if(!isDon)
        {
            x_scale = 1.2;
            y_scale = 1.2;
        }

        isBest = true;
    }

    s_drum.setTexture(drum_texture);
    s_drum.setOrigin(s_drum.getLocalBounds().width/2,s_drum.getLocalBounds().height/2);

    int particle_amount = 14;

    for(int i=0; i<particle_amount; i++)
    {
        sf::CircleShape temp;
        int radius = (rand() % max_radius) + 1;
        int angle = rand() % 360;

        float size_affection = floor(float(255) / max_radius);

        float cur_affection = 300;


        float power = log(300) / log(max_radius);

        if(radius <= max_radius)
        {
            cur_affection = pow(radius, power);
        }

        float total_color = 510;
        float color_value = total_color - cur_affection;

        int red = 0;
        int green = 0;

        if(color_value <= 255)
        {
            red = color_value;
            green = 0;
        }

        if(color_value > 255)
        {
            red = 255;
            green = color_value - 255;
        }

        temp.setFillColor(sf::Color(red,green,0,170));
        temp.setRadius(radius);
        temp.setPosition(-1000,-1000);

        int distance = (rand() % 200) + 50;

        c_particle.push_back(temp);

        particle_x.push_back(0);
        particle_y.push_back(0);
        particle_angle.push_back(angle);
        particle_didStart.push_back(false);
        particle_radius.push_back(radius);
        particle_maxDistance.push_back(distance);
        particle_curDistance.push_back(0);
        particle_speed.push_back(600);
    }

    drumClock.restart();
}

void Drum::Draw(sf::RenderWindow& window)
{
    float ratio_X = window.getSize().x / float(1280);
    float ratio_Y = window.getSize().y / float(720);
    float ratio_universal = (window.getSize().x * window.getSize().y) / (float(1280) * float(720));

    if(isDon)
    {
        if(isBest)
        {
            x_scale += 1 / fps;
            y_scale += 1 / fps;
            rotation += cur_pattern["rotateSpeed"][pattern] / fps;
            x += cur_pattern["xspeed"][pattern] / fps;
            y += cur_pattern["yspeed"][pattern] / fps;

            alpha -= float(510) / fps;

            if(alpha <= 0)
            alpha = 0;
        }
        else
        {
            if(drumClock.getElapsedTime().asSeconds() > 0.5)
            {
                alpha -= float(510) / fps;

                if(alpha <= 0)
                alpha = 0;
            }
        }
    }
    else
    {
        if(drumClock.getElapsedTime().asSeconds() > 0.5)
        {
            if(isBest)
            {
                x_scale += 1 / fps;
                y_scale += 1 / fps;
                rotation += cur_pattern["rotateSpeed"][pattern] / fps;
                x += cur_pattern["xspeed"][pattern] / fps;
                y += cur_pattern["yspeed"][pattern] / fps;
            }

            alpha -= float(510) / fps;

            if(alpha <= 0)
            alpha = 0;
        }
        else
        {
            if(isBest)
            {
                if(drumClock.getElapsedTime().asMilliseconds() <= 25)
                {
                    if((x_scale >= 1.2) && (y_scale >= 1.2))
                    {
                        x_scale += 1 / fps;
                        y_scale += 1 / fps;
                    }
                }
                else
                {
                    if((x_scale > 1) && (y_scale > 1))
                    {
                        x_scale -= 3 / fps;
                        y_scale -= 3 / fps;
                    }
                    else
                    {
                        x_scale = 1;
                        y_scale = 1;
                    }
                }
            }
        }
    }

    s_drum.setScale(x_scale,y_scale);
    s_drum.setColor(sf::Color(255,255,255,alpha));
    s_drum.setRotation((rotation+cur_pattern["angle"][pattern]) * 3.1415928 / 180.f);
    s_drum.setPosition(x+cur_pattern["x"][pattern],y+cur_pattern["y"][pattern]);

    if(drumClock.getElapsedTime().asMilliseconds() < 200)
    {
        x_flashscale += float(12) / fps;
        y_flashscale += float(12) / fps;
        flashalpha -= float(800) / fps;
    }
    else
    {
        flashalpha = 0;
    }

    if(flashalpha <= 0)
    flashalpha = 0;

    s_flash.setColor(sf::Color(255,255,255,flashalpha));
    s_flash.setScale(x_flashscale,y_flashscale);
    s_flash.setPosition(x+cur_pattern["x"][pattern],y+cur_pattern["y"][pattern]);

    if(shockwaveAlpha <= 0)
    {
        shockwaveAlpha = 0;
    }
    else
    {
        shockwaveSize += float(1300) / fps;
        shockwaveAlpha -= float(100) / fps;
    }

    if(shockwave2Alpha <= 0)
    {
        shockwave2Alpha = 0;
    }
    else
    {
        shockwave2Size += float(1300) / fps;
        shockwave2Alpha -= float(200) / fps;
    }

    if((shockwaveAlpha > 0) && (shockwave2Alpha > 0))
    {
        c_shockwave.setRadius(shockwaveSize);
        c_shockwave.setFillColor(sf::Color(255,255,255,shockwaveAlpha));
        c_shockwave.setOrigin(c_shockwave.getLocalBounds().width/2,c_shockwave.getLocalBounds().height/2);
        c_shockwave.setPosition(x+cur_pattern["x"][pattern]*ratio_X,y+cur_pattern["y"][pattern]*ratio_Y);

        c_shockwave2.setRadius(shockwave2Size);
        c_shockwave2.setFillColor(sf::Color(255,255,255,shockwave2Alpha));
        c_shockwave2.setOrigin(c_shockwave2.getLocalBounds().width/2,c_shockwave2.getLocalBounds().height/2);
        c_shockwave2.setPosition(x+cur_pattern["x"][pattern]*ratio_X,y+cur_pattern["y"][pattern]*ratio_Y);
    }

    s_drum.draw(window);
    s_flash.draw(window);
    window.draw(c_shockwave);
    window.draw(c_shockwave2);

    for(int i=0; i<c_particle.size(); i++)
    {
        ///Initialize first position
        if(particle_didStart[i] == false)
        {
            particle_x[i] = x+cur_pattern["x"][pattern]*ratio_X;
            particle_y[i] = y+cur_pattern["y"][pattern]*ratio_Y;
            particle_didStart[i] = true;
        }


        particle_x[i] += particle_speed[i] * cos(particle_angle[i]) / fps;
        particle_y[i] += particle_speed[i] * sin(particle_angle[i]) / fps;

        float distance = sqrt(2 * pow(particle_speed[i],2)) / fps;
        //cout << "i: " << i << " Distance speed: " << distance << " Cur distance: " << particle_curDistance[i] << " Max: " << particle_maxDistance[i] << endl;
        particle_curDistance[i] += distance;

        if(particle_curDistance[i] > particle_maxDistance[i])
        {
            particle_speed[i] -= float(3000) / fps;

            if(particle_speed[i] <= 0)
            particle_speed[i] = 0;


            float c_alpha = c_particle[i].getFillColor().a;
            c_alpha -= float(500) / fps;

            if(c_alpha <= 0)
            c_alpha = 0;

            c_particle[i].setFillColor(sf::Color(c_particle[i].getFillColor().r,c_particle[i].getFillColor().g,0,c_alpha));
        }

        c_particle[i].setRadius(particle_radius[i]);

        c_particle[i].setPosition(particle_x[i],particle_y[i]);
        window.draw(c_particle[i]);
    }
}
