#ifndef ENDFLAG_H
#define ENDFLAG_H

#include <Mission/Units/AnimatedObject.h>

class EndFlag
{
public:
    AnimatedObject main;
    float local_x=0, local_y=0;
    float global_x=0, global_y=0;

    EndFlag();
    void Draw();
};

#endif ENDFLAG_H