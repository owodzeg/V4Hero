#ifndef CURVSEGMENT_H
#define CURVSEGMENT_H

#include <SFML/System/Vector2.hpp>
#include <vector>
enum CurveType
{
    QUAD_CURVE = 1,
    LIN_CURVE = 2,
    STRAIGHT_LINE = 3
};

class CurveSegment
{
public:
    std::vector<sf::Vector2f> points;
    const int NUM_INTERP = 15;
    sf::Vector2f point1;
    sf::Vector2f point2;
    sf::Vector2f control_point1;
    sf::Vector2f control_point2;
    CurveType type;

    std::vector<sf::Vector2f> CalculateCurvePoints(sf::Vector2f lpoint1, sf::Vector2f lpoint2, sf::Vector2f lcontrol_point1, sf::Vector2f lcontrol_point2);

    CurveSegment(CurveType c_type, sf::Vector2f lpoint1, sf::Vector2f lpoint2, sf::Vector2f lcontrol_point1, sf::Vector2f lcontrol_point2);
    ~CurveSegment();
};


#endif // CURVSEGMENT_H
