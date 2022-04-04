
#include "CurveSegment.h"


std::vector<sf::Vector2f> CurveSegment::CalculateCurvePoints(sf::Vector2f point1, sf::Vector2f point2, sf::Vector2f control_point1, sf::Vector2f control_point2)
{
    std::vector<sf::Vector2f>* result = new std::vector<sf::Vector2f>();
    float b0x = point1.x;
    float b0y = point1.y;

    float b1x = control_point1.x;
    float b1y = control_point1.y;

    float b2x = control_point2.x;
    float b2y = control_point2.y;

    float b3x = point2.x;
    float b3y = point2.y;

    /* float ax = (-point1.x + 3 * control_point1.x) + (-3 * control_point2.x + point2.x);
    float ay = (-point1.y + 3 * control_point1.y) + (-3 * control_point2.y + point2.y);*/

    float ax = -b0x + 3 * b1x + -3 * b2x + b3x;
    float ay = -b0y + 3 * b1y + -3 * b2y + b3y;

    float bx = 3 * b0x + -6 * b1x + 3 * b2x;
    float by = 3 * b0y + -6 * b1y + 3 * b2y;

    float cx = -3 * b0x + 3 * b1x;
    float cy = -3 * b0y + 3 * b1y;

    float dx = b0x;
    float dy = b0y;

    //Set up the number of steps and step size
    int numPoints = 50;
    int numSteps = numPoints - 1;
    float h = 1.0 / numSteps;

    float pointX = dx;
    float pointY = dy;


    float firstFDX = ax * (h * h * h) + bx * (h * h) + cx * h;
    float firstFDY = ay * (h * h * h) + by * (h * h) + cy * h;

    float secondFDX = 6 * ax * (h * h * h) + 2 * bx * (h * h);
    float secondFDY = 6 * ay * (h * h * h) + 2 * by * (h * h);

    float thirdFDX = 6 * ax * (h * h * h);
    float thirdFDY = 6 * ay * (h * h * h);

    result->push_back(sf::Vector2f(int(pointX), int(pointY)));

    for (size_t i = 0; i < numSteps; i++)
    {

        pointX += firstFDX;
        pointY += firstFDY;

        firstFDX += secondFDX;
        firstFDY += secondFDY;

        secondFDX += thirdFDX;
        secondFDY += thirdFDY;

        result->push_back(sf::Vector2f(int(pointX), int(pointY)));
    }
    return *result;
    /*ay = -b0y + 3 * b1y + -3 * b2y + b3y

    bx = 3 * b0x + -6 * b1x + 3 * b2x
    by = 3 * b0y + -6 * b1y + 3 * b2y

    cx = -3 * b0x + 3 * b1x
    cy = -3 * b0y + 3 * b1y

    dx = b0x
    dy = b0y

    # Set up the number of steps and step size
    numSteps = numPoints - 1  # arbitrary choice
    h = 1.0 / numSteps  # compute our step size

    # Compute forward differences from Bezier points and "h"
    pointX = dx
    pointY = dy

    firstFDX = ax * (h * h * h) + bx * (h * h) + cx * h
    firstFDY = ay * (h * h * h) + by * (h * h) + cy * h

    secondFDX = 6 * ax * (h * h * h) + 2 * bx * (h * h)
    secondFDY = 6 * ay * (h * h * h) + 2 * by * (h * h)

    thirdFDX = 6 * ax * (h * h * h)
    thirdFDY = 6 * ay * (h * h * h)

    # Compute points at each step
    result.append((int(pointX), int(pointY)))

    for i in range(numSteps):
        pointX += firstFDX
        pointY += firstFDY

        firstFDX += secondFDX
        firstFDY += secondFDY

        secondFDX += thirdFDX
        secondFDY += thirdFDY

        result.append((int(pointX), int(pointY)))*/
    //result->push_back();
}

CurveSegment::CurveSegment(CurveType c_type, sf::Vector2f point1, sf::Vector2f point2, sf::Vector2f control_point1 = sf::Vector2f(-1, -1), sf::Vector2f control_point2 = sf::Vector2f(-1, -1))
{
    type = c_type;
    if (c_type == CurveType::QUAD_CURVE)
    {
        points = CalculateCurvePoints(point1, point2, control_point1, control_point2);
    } 
    else if (c_type == CurveType::LIN_CURVE)
    {
        points = CalculateCurvePoints(point1, point2, control_point1, control_point1);
    } 
    else if (c_type == CurveType::LIN_CURVE)
    {
        points.clear();
        points.push_back(point1);
        points.push_back(point2);
    }
    //ctor
}

CurveSegment::~CurveSegment()
{
    //dtor
}
