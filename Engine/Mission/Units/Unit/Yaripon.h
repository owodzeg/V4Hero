#ifndef YARIPON_H
#define YARIPON_H

#include "../AnimatedObject.h"

class Yaripon {
public:
    AnimatedObject main;
    float local_x=0, local_y=0;
    float global_x=0, global_y=0;

    int order = 0;
    int maxp = 0;

    Yaripon(int which, int maxpons);
    void Advance();
    void StopAll();
    void Drum(std::string drum);
    void Draw();
};

#endif //YARIPON_H
