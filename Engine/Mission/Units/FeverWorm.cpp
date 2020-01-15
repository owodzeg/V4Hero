#include "FeverWorm.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include "../../Func.h"
#include <sstream>

FeverWorm::FeverWorm()
{

}

void FeverWorm::LoadConfig(Config *thisConfigs)
{
    /// all (normal) kacheeks have the same animations, so we load them from a hardcoded file
    AnimatedObject::LoadConfig(thisConfigs,"resources\\units\\fever_worm.p4a");
}

void FeverWorm::Draw(sf::RenderWindow& window)
{
    if(worm_fever)
    {
        global_x = -250;
        next_x = 130;
        speed = 450;

        worm_fever = false;
    }

    if(global_x > next_x)
    global_x -= speed / fps;

    if(global_x < next_x)
    global_x += speed / fps;

    auto view = window.getView();
    window.setView(window.getDefaultView());

    /// call the parent function to draw the animations
    AnimatedObject::Draw(window);

    window.setView(view);
}
