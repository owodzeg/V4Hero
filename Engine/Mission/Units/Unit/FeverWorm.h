#ifndef FEVERWORM_H
#define FEVERWORM_H

#include <Mission/Units/AnimatedObject.h>

class FeverWorm
{
public:
    AnimatedObject main;
    float local_x=0, local_y=0, local_x_dest=0;
    float global_x=0, global_y=0;

    bool fever = false;
    bool t_done = false;
    sf::Clock t_timer;

    FeverWorm();
    void PrefeverSpeedUp();
    void PrefeverSlowDown();
    void Transform();
    void Break();
    void Draw();
};

#endif FEVERWORM_H