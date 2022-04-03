#ifndef SCREENFADE_H
#define SCREENFADE_H

#include "../Config.h"
#include <SFML/Graphics.hpp>

class ScreenFade
{
public:
    sf::RectangleShape r_rect;
    int cur_mode = 0; ///0 - fade in, 1 - fade out
    float cur_speed = 512;
    float alpha = 0;
    float dest_alpha = 0;

    ScreenFade();
    void Create(Config* config, int mode, float speed);
    void Create(int mode, float speed);
    void FadeIn();
    void FadeOut();
    bool checkFinished();
    void draw(sf::RenderWindow& window, float fps);
    void draw();
};

#endif // SCREENFADE_H
