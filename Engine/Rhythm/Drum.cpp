#include "Drum.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "../CoreManager.h"
#include "../Constants.h"

Drum::Drum()
{

}

void Drum::Load(std::string drum, int perfection, std::string& drum_texture)
{
    SPDLOG_DEBUG("Loading drum {}, perfection {}, drum_texture {}", drum, perfection, drum_texture);

    s_flash.load("resources/graphics/rhythm/drums/flash.png");
    s_flash.setOrigin(s_flash.getLocalBounds().size.x / 2, s_flash.getLocalBounds().size.y / 2);

    cur_drum = drum;

    if (drum == "pata")
    {
        drumPatterns.push_back(DrumPattern(135, 295, -20,  -8, -200,  40));
        drumPatterns.push_back(DrumPattern(100, 410, -20,  -8,  140, -40));
        drumPatterns.push_back(DrumPattern(80,  340,   3, -40,  -20,  60));
        drumPatterns.push_back(DrumPattern(95,  220,  -7,  20,  -20, -60));
        drumPatterns.push_back(DrumPattern(100, 410, -15,  -8,  140, -40));
        drumPatterns.push_back(DrumPattern(135, 295, -23,  -8, -200,  40));
        drumPatterns.push_back(DrumPattern(80,   80,   3, -40,  -20,  60));
        drumPatterns.push_back(DrumPattern(95,   95,  -7,  20,  -20, -60));
    } else if (drum == "pon")
    {
        drumPatterns.push_back(DrumPattern(1140, 290, 0, -40, -10, -60));
        drumPatterns.push_back(DrumPattern(1165, 410, 20, -40, -40, 30));
        drumPatterns.push_back(DrumPattern(1110, 340, 10, 20, 20, -30));
        drumPatterns.push_back(DrumPattern(1135, 220, 10, 10, 100, 50));
        drumPatterns.push_back(DrumPattern(1165, 410, 20, -40, -40, 30));
        drumPatterns.push_back(DrumPattern(1140, 290, 0, -40, -10, -60));
        drumPatterns.push_back(DrumPattern(1110, 340, 10, 20, 20, -30));
        drumPatterns.push_back(DrumPattern(1135, 220, 10, 10, 100, 50));
    }
    else if (drum == "chaka")
    {
        drumPatterns.push_back(DrumPattern(635, 105, 0, 10, 20, 40));
        drumPatterns.push_back(DrumPattern(460, 70, 10, -70, 30, -60));
        drumPatterns.push_back(DrumPattern(550, 75, -10, -40, 40, 20));
        drumPatterns.push_back(DrumPattern(715, 85, -10, 40, -60, 60));
        drumPatterns.push_back(DrumPattern(460, 70, 10, -70, 30, -60));
        drumPatterns.push_back(DrumPattern(635, 105, 0, 10, 20, 40));
        drumPatterns.push_back(DrumPattern(550, 75, -10, -40, 40, 20));
        drumPatterns.push_back(DrumPattern(715, 85, -10, 40, -60, 60));
    }
    else if (drum == "don")
    {
        drumPatterns.push_back(DrumPattern(690, 650, 0, -30, -30, 40));
        drumPatterns.push_back(DrumPattern(515, 660, 10, -50, -50, 60));
        drumPatterns.push_back(DrumPattern(605, 665, -10, -50, -50, -60));
        drumPatterns.push_back(DrumPattern(780, 670, -10, 90, -30, -60));
        drumPatterns.push_back(DrumPattern(515, 660, 10, -50, -50, 60));
        drumPatterns.push_back(DrumPattern(690, 650, 0, -30, -30, 40));
        drumPatterns.push_back(DrumPattern(605, 665, -10, -50, -50, -60));
        drumPatterns.push_back(DrumPattern(780, 670, -10, -90, -30, -60));
        isDon = true;
    }

    if (perfection == 0) ///BEST beat
    {
        if (!isDon)
        {
            x_scale = 1.2f;
            y_scale = 1.2f;
        }

        isBest = true;
    }

    s_drum.load(drum_texture);
    s_drum.setOrigin(s_drum.getLocalBounds().size.x / 2, s_drum.getLocalBounds().size.y / 2);

    int particle_amount = 14;

    for (int i = 0; i < particle_amount; i++)
    {
        sf::CircleShape temp;
        int radius = Func::rand_range(0, max_radius) + 1;
        int angle = Func::rand_range(0, 360);

        float cur_affection = 300;

        float power = log(300) / log(max_radius);

        if (radius <= max_radius)
        {
            cur_affection = pow(radius, power);
        }

        float total_color = 510;
        float color_value = total_color - cur_affection;

        int red = 0;
        int green = 0;

        if (color_value <= 255)
        {
            red = color_value;
            green = 0;
        }

        if (color_value > 255)
        {
            red = 255;
            green = color_value - 255;
        }

        temp.setFillColor(sf::Color(red, green, 0, 170));
        temp.setRadius(radius);
        temp.setPosition(sf::Vector2f(-1000, -1000));

        int distance = Func::rand_range(0, 200) + 50;

        particles.push_back(Particle(temp, false, 0, 0, angle, radius, distance, 0, 1800));
    }

    drumClock.restart();
}

void Drum::Draw()
{
    //TO-DO: overhaul needed here. make it more optimized and less weighty

    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    float fps = CoreManager::getInstance().getCore()->getFPS();

    if (isDon)
    {
        if (isBest)
        {
            x_scale += 1 / fps;
            y_scale += 1 / fps;
            rotation += drumPatterns[pattern].rotSpeed / fps;
            x += drumPatterns[pattern].xSpeed / fps;
            y += drumPatterns[pattern].ySpeed / fps;

            alpha -= float(510) / fps;

            if (alpha <= 0)
                alpha = 0;
        } else
        {
            if (drumClock.getElapsedTime().asSeconds() > 0.5)
            {
                alpha -= float(510) / fps;

                if (alpha <= 0)
                    alpha = 0;
            }
        }
    } else
    {
        if (drumClock.getElapsedTime().asSeconds() > 0.5)
        {
            if (isBest)
            {
                x_scale += 1 / fps;
                y_scale += 1 / fps;
                rotation += drumPatterns[pattern].rotSpeed / fps;
                x += drumPatterns[pattern].xSpeed / fps;
                y += drumPatterns[pattern].ySpeed / fps;
            }

            alpha -= float(510) / fps;

            if (alpha <= 0)
                alpha = 0;
        } else
        {
            if (isBest)
            {
                if (drumClock.getElapsedTime().asMilliseconds() <= 25)
                {
                    if ((x_scale >= 1.2) && (y_scale >= 1.2))
                    {
                        x_scale += 1 / fps;
                        y_scale += 1 / fps;
                    }
                } else
                {
                    if ((x_scale > 1) && (y_scale > 1))
                    {
                        x_scale -= 3 / fps;
                        y_scale -= 3 / fps;
                    } else
                    {
                        x_scale = 1;
                        y_scale = 1;
                    }
                }
            }
        }
    }

    s_drum.setScale(x_scale, y_scale);
    s_drum.setColor(sf::Color(255, 255, 255, alpha));
    s_drum.setRotation((rotation + drumPatterns[pattern].angle) * PI / 180.f);
    s_drum.setPosition(x + drumPatterns[pattern].x*3, y + drumPatterns[pattern].y*3);

    if (drumClock.getElapsedTime().asMilliseconds() < 200)
    {
        x_flashscale += float(12) / fps;
        y_flashscale += float(12) / fps;
        flashalpha -= float(800) / fps;
    } else
    {
        flashalpha = 0;
    }

    if (flashalpha <= 0)
        flashalpha = 0;

    s_flash.setColor(sf::Color(255, 255, 255, flashalpha));
    s_flash.setScale(x_flashscale, y_flashscale);
    s_flash.setPosition(x + drumPatterns[pattern].x*3, y + drumPatterns[pattern].y*3);

    if (shockwaveAlpha <= 0)
    {
        shockwaveAlpha = 0;
    } else
    {
        shockwaveSize += float(1300) / fps;
        shockwaveAlpha -= float(100) / fps;
    }

    if (shockwave2Alpha <= 0)
    {
        shockwave2Alpha = 0;
    } else
    {
        shockwave2Size += float(1300) / fps;
        shockwave2Alpha -= float(200) / fps;
    }

    if ((shockwaveAlpha > 0) && (shockwave2Alpha > 0))
    {
        c_shockwave.setRadius(shockwaveSize);
        c_shockwave.setFillColor(sf::Color(255, 255, 255, shockwaveAlpha));
        c_shockwave.setOrigin(sf::Vector2f(c_shockwave.getLocalBounds().size.x / 2, c_shockwave.getLocalBounds().size.y / 2));
        c_shockwave.setPosition(sf::Vector2f(x + drumPatterns[pattern].x * 3 * CoreManager::getInstance().getCore()->resRatio, y + drumPatterns[pattern].y * 3 * CoreManager::getInstance().getCore()->resRatio));

        c_shockwave2.setRadius(shockwave2Size);
        c_shockwave2.setFillColor(sf::Color(255, 255, 255, shockwave2Alpha));
        c_shockwave2.setOrigin(sf::Vector2f(c_shockwave2.getLocalBounds().size.x / 2, c_shockwave2.getLocalBounds().size.y / 2));
        c_shockwave2.setPosition(sf::Vector2f(x + drumPatterns[pattern].x * 3 * CoreManager::getInstance().getCore()->resRatio, y + drumPatterns[pattern].y * 3 * CoreManager::getInstance().getCore()->resRatio));
    }

    s_drum.draw();
    s_flash.draw();
    window->draw(c_shockwave);
    window->draw(c_shockwave2);

    for (int i = 0; i < particles.size(); i++)
    {
        ///Initialize first position
        if (particles[i].didStart == false)
        {
            particles[i].x = x + drumPatterns[pattern].x * 3 * CoreManager::getInstance().getCore()->resRatio;
            particles[i].y = y + drumPatterns[pattern].y * 3 * CoreManager::getInstance().getCore()->resRatio;
            particles[i].didStart = true;
        }

        particles[i].x += particles[i].speed * cos(particles[i].angle) / fps;
        particles[i].y += particles[i].speed * sin(particles[i].angle) / fps;

        float distance = sqrt(2 * pow(particles[i].speed, 2)) / fps;
        //cout << "i: " << i << " Distance speed: " << distance << " Cur distance: " << particles[i].curDistance << " Max: " << particles[i].maxDistance << endl;
        particles[i].curDistance += distance;

        if (particles[i].curDistance > particles[i].maxDistance)
        {
            particles[i].speed -= float(9000) / fps;

            if (particles[i].speed <= 0)
                particles[i].speed = 0;


            float c_alpha = particles[i].c_particle.getFillColor().a;
            c_alpha -= float(500) / fps;

            if (c_alpha <= 0)
                c_alpha = 0;

            particles[i].c_particle.setFillColor(sf::Color(particles[i].c_particle.getFillColor().r, particles[i].c_particle.getFillColor().g, 0, c_alpha));
        }

        particles[i].c_particle.setPosition(sf::Vector2f(particles[i].x, particles[i].y));
        window->draw(particles[i].c_particle);
    }
}
