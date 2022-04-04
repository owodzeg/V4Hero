
#include "CurveShape.h"



CurveShape::CurveShape(std::vector<CurveSegment> c_curves, sf::Color c_fillcolor, sf::Color c_outlinecolor)
{
    curves = c_curves;
    fillcolor = c_fillcolor;
    outlinecolor = c_outlinecolor;
    //ctor
}


void CurveShape::Draw(sf::RenderWindow*)
{
    std::vector<sf::Vector2f> points;

    for (size_t i = 0; i < curves.size(); i++)
    {
        CurveSegment thiscurve = curves[i];
        if (i == 0) {

        } 
        else if (i == curves.size() - 1)
        {
        
        } else
        {

        }
    }
}

CurveShape::~CurveShape()
{
    //dtor
}
