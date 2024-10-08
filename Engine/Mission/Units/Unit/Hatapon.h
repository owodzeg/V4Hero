#ifndef HATAPON_H
#define HATAPON_H

#include "../AnimatedObject.h"

class Hatapon {
public:
    AnimatedObject main;
    float local_x=0, local_y=0;
    float global_x=0, global_y=0;

    Hatapon();
    void Wave();
    void StopAll();
    void Draw();
};

#endif //HATAPON_H
