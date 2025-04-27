
#include "CurveShape.h"
#include "ConcaveShape.h"
#include "CurveSegment.h"
#include <iostream>
#include <CoreManager.h>


CurveShape::CurveShape(nlohmann::json data)
{
    curves.clear();
    fillcolor = sf::Color(data["fillcolor"][0], data["fillcolor"][1], data["fillcolor"][2], data["fillcolor"][3]);
    for (size_t i = 0; i < data["curves"].size(); i++)
    {
        std::cout << "one curveSegment" << std::endl;
        sf::Vector2f startpoint = sf::Vector2f(data["curves"][i]["point1"][0], 
                                               data["curves"][i]["point1"][1]);
        sf::Vector2f startcontrolpoint = sf::Vector2f(data["curves"][i]["controlpoint1"][0], 
                                                      data["curves"][i]["controlpoint1"][1]);

        sf::Vector2f endpoint = sf::Vector2f(data["curves"][i]["point2"][0], 
                                             data["curves"][i]["point2"][1]);
        sf::Vector2f endcontrolpoint = sf::Vector2f(data["curves"][i]["controlpoint2"][0], 
                                                    data["curves"][i]["controlpoint2"][1]);

        CurveType type = CurveType(data["curves"][i]["curvetype"]);
        CurveSegment* crv = new CurveSegment(type, startpoint, endpoint, startcontrolpoint, endcontrolpoint);
        curves.push_back(crv);
    }

    UpdatePoints();
}
//
//  It is very important curves are in order, and each curve's points are in order such that last point of last curve is the same as first point of first curve
//
CurveShape::CurveShape(std::vector<CurveSegment*> c_curves, sf::Color c_fillcolor)
{
    curves = c_curves;
    fillcolor = c_fillcolor;
    UpdatePoints();
    //ctor
}


void CurveShape::Draw()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    if (CoreManager::getInstance().getConfig()->GetInt("strictAnim")) {

        sfml::ConcaveShape polygon = sfml::ConcaveShape(points, fillcolor);
        window->draw(polygon);
    } else
    {
        try
        {
            sfml::ConcaveShape polygon = sfml::ConcaveShape(points, fillcolor);
            window->draw(polygon);
        } 
        catch (std::exception)
        {
            // not in strict anim mode so let error pass
        }
    }
}

void CurveShape::UpdatePoints()
{
    points = *(new std::vector<sf::Vector2f>());
    for (size_t i = 0; i < curves.size(); i++)
    {
        CurveSegment* thiscurve = curves[i];
        if (i == 0)
        {
            std::vector<sf::Vector2f> curv_points = thiscurve->points;
            points.insert(points.end(), curv_points.begin(), curv_points.end());
        } else if (i == curves.size() - 1)
        {
            std::vector<sf::Vector2f> curv_points = thiscurve->points;
            curv_points.erase(curv_points.begin());
            curv_points.pop_back();
            points.insert(points.end(), curv_points.begin(), curv_points.end());
        } else
        {
            std::vector<sf::Vector2f> curv_points = thiscurve->points;
            curv_points.erase(curv_points.begin());
            points.insert(points.end(), curv_points.begin(), curv_points.end());
        }
    }
}

CurveShape::~CurveShape()
{
    //dtor
}
