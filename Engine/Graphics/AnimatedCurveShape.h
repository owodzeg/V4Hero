#include <Graphics/CurveSegment.h>
#include <Graphics/CurveShape.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#ifndef ANIMCRV_SHP
#define ANIMCRV_SHP

class AnimatedCurveShape
{
    // a series of curveshapes for each frame, with a function to interpolate between two 
public:
    std::vector<CurveShape> frames;
    CurveShape* currentFrame = nullptr;

    float cur_pos = 0;
    float max_pos = 0;
    float fps = 60;
    float framerate = 5;
    bool loop = false;

    AnimatedCurveShape(std::string path);
    AnimatedCurveShape(std::vector<CurveShape> frames, sf::Color fillcolor);

    void Draw();
    ~AnimatedCurveShape();
};


#endif // ANIMCRV_SHP
