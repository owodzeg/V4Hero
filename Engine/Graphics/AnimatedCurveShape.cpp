#include "ConcaveShape.h"
#include "CurveSegment.h"
#include "CurveShape.h"
#include <fstream>
#include <iostream>
#include <Graphics/AnimatedCurveShape.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

using namespace std;
using json = nlohmann::json; // Convenience recommended by the library

//
//  It is very important curves are in order, and each curve's points are in order such that last point of last curve is the same as first point of first curve
//  This version of the constructor loads an animated curve definition file
AnimatedCurveShape::AnimatedCurveShape(std::string path)
{

    SPDLOG_INFO("Loading AnimatedCurveShape at " + path);
    frames.clear();
    ifstream t("resources/data/anim_data.json", std::ios::in);
    nlohmann::json frame_data;
    t >> frame_data;
    for (int i = 0; i < frame_data["frames"].size(); i++)
    {
        SPDLOG_INFO("Inside");
        frames.push_back(CurveShape(frame_data["frames"][i]));
    }

    
    framerate = frame_data["framerate"];

    cur_pos = 0;
    max_pos = frames.size();

}

//
//  It is very important curves are in order, and each curve's points are in order such that last point of last curve is the same as first point of first curve
//  This version of the constructor is for manually defining an animation out of a series of curve "frames". Use the other one preferably, so its moddable
AnimatedCurveShape::AnimatedCurveShape(std::vector<CurveShape> c_frames, sf::Color c_fillcolor)
{
    int size = c_frames[0].curves.size();
    for (size_t i = 0; i < c_frames.size(); i++)
    {
        int this_curve_size = c_frames[i].curves.size();
        if (this_curve_size != size)
        {
            throw std::invalid_argument("c_frames must all have the same number of curve segments");
        }
    }
    framerate = 5;

    cur_pos = 0;
    frames = c_frames;
    max_pos = c_frames.size();

    //ctor
}
void Draw(std::vector<CurveShape*> c_frames, sf::Color c_fillcolor);

void AnimatedCurveShape::Draw()
{
    // cur_pos is the index of the current frame, but it can be fractional to represent partway between frames. Consider interpolating
    cur_pos += framerate / float(fps);
    if (cur_pos > max_pos-1)
    {
        cur_pos = cur_pos - (max_pos-1);
    }
    CurveShape precurve = frames[floor(cur_pos)];
    CurveShape postcurve =  frames[ceil(cur_pos)];

    // fraction is how far to the next frame has progressed. to get interp value we multiply old one by "postfraction" and new one by "fraction"
    float fraction = cur_pos - floor(cur_pos);
    float postfraction = 1 - fraction;

    std::vector<CurveSegment*> interp_curves;
    for (size_t i = 0; i < precurve.curves.size(); i++) // it is assumed all shape frames have same number of sides/curve segments
    {
        CurveSegment* precrv_seg = precurve.curves[i];
        CurveSegment* postcrv_seg = postcurve.curves[i];
        
        sf::Vector2<float> point1 = sf::Vector2(precrv_seg->point1.x * postfraction + postcrv_seg->point1.x * fraction, 
                                                precrv_seg->point1.y * postfraction + postcrv_seg->point1.y * fraction);

        sf::Vector2<float> point2 = sf::Vector2(precrv_seg->point2.x * postfraction + postcrv_seg->point2.x * fraction,
                                                precrv_seg->point2.y * postfraction + postcrv_seg->point2.y * fraction);

        sf::Vector2<float> control_point1 = sf::Vector2(precrv_seg->control_point1.x * postfraction + postcrv_seg->control_point1.x * fraction,
                                                        precrv_seg->control_point1.y * postfraction + postcrv_seg->control_point1.y * fraction);

        sf::Vector2<float> control_point2 = sf::Vector2(precrv_seg->control_point2.x * postfraction + postcrv_seg->control_point2.x * fraction,
                                                        precrv_seg->control_point2.y * postfraction + postcrv_seg->control_point2.y * fraction);

        CurveSegment* interpolatedSegment = new CurveSegment(precrv_seg->type, point1, point2, control_point1, control_point2);
        interp_curves.push_back(interpolatedSegment);
    }


    float red = precurve.fillcolor.r * postfraction + postcurve.fillcolor.r * fraction;
    float green = precurve.fillcolor.g * postfraction + postcurve.fillcolor.g * fraction;
    float blue = precurve.fillcolor.b * postfraction + postcurve.fillcolor.b * fraction;
    float alpha = precurve.fillcolor.a * postfraction + postcurve.fillcolor.a * fraction;
    sf::Color fillcolor = sf::Color(red, green, blue, alpha);

    CurveShape* interp_curve = new CurveShape(interp_curves, fillcolor);
    interp_curve->Draw();
    delete interp_curve;
}