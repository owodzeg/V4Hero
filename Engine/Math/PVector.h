#ifndef PVECTOR_H
#define PVECTOR_H
#include "SFML/Graphics.hpp"
///Class for automatic sprite resizing based on the current window resolution and quality options

class PVector
{
public:
    float distance = 0;
    float angle = 0;
    sf::Vector2f GetStartPoint();
    sf::Vector2f GetEndPoint();
    float GetScalarProjectionOntoAxis(float axisAngle);
    static PVector& getVectorCartesian(float x1, float y1, float x2, float y2);
    static PVector& getVectorDistanceAngle(float l, float theta);
};

#endif // PVECTOR_H
