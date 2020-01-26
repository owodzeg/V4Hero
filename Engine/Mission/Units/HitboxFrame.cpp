#include "HitboxFrame.h"
#include <iostream>
#include "../../Math/PVector.h"
#include <cmath>
using namespace std;

HitboxFrame::HitboxFrame()
{

}

HitboxFrame::~HitboxFrame()
{

}

float HitboxFrame::minProjection(float axisAngle){
    float projectionLength = 99999999;
    vector<sf::Vector2f> currentVertices = getCurrentVertices();
    /// go through each vertex (corner) and find the shortest projection from the origin to that vertex along the axis aligned with axisAngle
    for (int i=0;i<currentVertices.size()-1;i++){
        sf::Vector2f currentVertex = vertices[i];
        PVector cornerVector = PVector::getVectorCartesian(0,0,currentVertex.x,currentVertex.y);
        float currentProjectionLength = cornerVector.GetScalarProjectionOntoAxis(axisAngle);
        if (currentProjectionLength<projectionLength){
            projectionLength=currentProjectionLength;
        }
    }
    return projectionLength;
}

float HitboxFrame::maxProjection(float axisAngle){
    float projectionLength = 0;
    vector<sf::Vector2f> currentVertices = getCurrentVertices();
    /// go through each vertex (corner) and find the longest projection from the origin to that vertex along the axis aligned with axisAngle
    for (int i=0;i<currentVertices.size()-1;i++){
        sf::Vector2f currentVertex = vertices[i];
        PVector cornerVector = PVector::getVectorCartesian(0,0,currentVertex.x,currentVertex.y);
        float currentProjectionLength = cornerVector.GetScalarProjectionOntoAxis(axisAngle);
        if (currentProjectionLength>projectionLength){
            projectionLength=currentProjectionLength;
        }
    }
    return projectionLength;
}
vector<sf::Vector2f> HitboxFrame::getCurrentVertices(){
    /// oh boy.... we need to apply the transformation matrix to this bad boy;
    /// transformation matrix is 4x4 so we need a 4x1 vector for each point
    /// yes, it is a 3d transformation even though we are in 2d, thats just how the math goes because a 2Drotation is a rotation about the z-axis
    /// it will be as follows:
    ///     -----       ----------------------------------------------------
    ///     | x |       | cos(angle)*scaleX    -sin(angle)          0   1  |
    ///     | y |       | sin(angle)            cos(angle)*scaleY   0   0  |
    ///     | 0 |   *   | 0                     0                   1   0  |
    ///     | 1 |       | g_xpos                g_ypos              0   1  |
    ///     -----       ----------------------------------------------------
    /// result: a 1x4 vector
    vector<sf::Vector2f> newVertices;
    for (int i=0;i<vertices.size()-1;i++){
        sf::Vector2f currentVertex = vertices[i];
        /// scaleX*(gx + x*cos(angle) + y*sin(angle))
        float resultX = scaleX*(g_x+currentVertex.x*cos(rotation)+currentVertex.y*sin(rotation));
        /// scaleY*(gy + y*cos(angle) - x*sin(angle))
        float resultY = scaleY*(g_y + currentVertex.y*cos(rotation) - currentVertex.x*sin(rotation));
        newVertices.push_back(sf::Vector2f(resultX,resultY));
    }


}
void HitboxFrame::addVertex(float relx,float rely){
    vertices.push_back(sf::Vector2f(relx,rely));
}
