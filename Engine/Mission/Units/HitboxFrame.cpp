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

float HitboxFrame::minProjection(float axisAngle,float object_x,float object_y){
    float projectionLength = 99999999;
    vector<sf::Vector2f> currentVertices = getCurrentVertices();

    //cout<<" - SEARCHING FOR MIN PROJECTION"<<endl;
    /// go through each vertex (corner) and find the shortest projection from the origin to that vertex along the axis aligned with axisAngle
    for (int i=0;i<currentVertices.size();i++){
        sf::Vector2f currentVertex = currentVertices.at(i);
        PVector cornerVector = PVector::getVectorCartesian(0,0,currentVertex.x+g_x+object_x,currentVertex.y+g_y+object_y);
        cornerVector.angle =-atan2(currentVertex.y+g_y+object_y, currentVertex.x+g_x+object_x);
        float currentProjectionLength = cornerVector.GetScalarProjectionOntoAxis(axisAngle);

        //cout<<"# currentProjectionLength: "<<i<<"axisAngle: "<<axisAngle<<" val: "<<currentProjectionLength<<" gx: "<<object_x<<" gy: "<<object_y<<endl;
        if (currentProjectionLength<projectionLength){
            projectionLength=currentProjectionLength;
        }
    }
    //cout<<"Min projection hitbox: "<<projectionLength<<endl;
    return projectionLength;
}

float HitboxFrame::maxProjection(float axisAngle,float object_x,float object_y){
    float projectionLength = 0;

    vector<sf::Vector2f> currentVertices = getCurrentVertices();
    float angled = 3.14159265358/2;
    if(axisAngle==angled){
        //cout<<" "<<endl;
    }
    //cout<<" - SEARCHING FOR MAX PROJECTION"<<endl;
    /// go through each vertex (corner) and find the longest projection from the origin to that vertex along the axis aligned with axisAngle
    for (int i=0;i<currentVertices.size();i++){
        sf::Vector2f currentVertex = currentVertices.at(i);
        PVector cornerVector = PVector::getVectorCartesian(0,0,currentVertex.x+g_x+object_x,currentVertex.y+g_y+object_y);
        cornerVector.angle =-atan2(currentVertex.y+g_y+object_y, currentVertex.x+g_x+object_x);
        float currentProjectionLength = cornerVector.GetScalarProjectionOntoAxis(axisAngle);
        if (axisAngle==angled){
            //cout<<"angle: "<<cornerVector.angle<<" axisAngle: "<<axisAngle<<" angleDiff: "<<(cornerVector.angle)-axisAngle<<endl;
        }
        //cout<<"# currentProjectionLength: "<<i<<"axisAngle: "<<axisAngle<<" val: "<<currentProjectionLength<<" gx: "<<object_x<<" gy: "<<object_y<<endl;
        if (currentProjectionLength>projectionLength){
            projectionLength=currentProjectionLength;
        }
    }

    //cout<<"Max projection hitbox: "<<projectionLength<<endl;
    return projectionLength;
}
vector<sf::Vector2f>* HitboxFrame::getBaseVerticiesDontUseThisUnlessYouKnowWhy(){
    return &vertices;
}
vector<sf::Vector2f> HitboxFrame::getCurrentVertices()
{
    /// oh boy.... we need to apply the transformation matrix to this bad boy;
    /// transformation matrix is 4x4 so we need a 4x1 vector for each point
    /// yes, it is a 3d transformation even though we are in 2d, thats just how the math goes because a 2Drotation is a rotation about the z-axis
    /// it will be as follows:
    ///     -----       ----------------------------------------------------
    ///     | x |       | cos(angle)*scaleX    -sin(angle)          0   0  |
    ///     | y |       | sin(angle)            cos(angle)*scaleY   0   0  |
    ///     | 0 |   *   | 0                     0                   1   0  |
    ///     | 1 |       | g_xpos                g_ypos              0   1  |
    ///     -----       ----------------------------------------------------
    /// result: a 1x4 vector

    /// Apply changes only if rotation has changed, if rotation havent changed, theres no need to recalculate the vertices
    if(lastRotation != rotation)
    {
        /// helper values, as they never change
        float c = cos(rotation);
        float s = sin(rotation);

        for (int i=0; i<vertices.size(); i++)
        {
            sf::Vector2f currentVertex = vertices[i];

            /// I have worked through the matrix maths and calculated the following results, which have some minor optimisations

            /// scaleX*(gx + x*cos(angle) + y*sin(angle))
            float resultX = scaleX * (g_x + currentVertex.x*c + currentVertex.y*s);

            /// scaleY*(gy + y*cos(angle) - x*sin(angle))
            float resultY = scaleY * (g_y + currentVertex.y*c - currentVertex.x*s);

            vertices[i] = sf::Vector2f(resultX, resultY);
        }
    }

    lastRotation = rotation;

    return vertices;
}
void HitboxFrame::clearVertices(){
    vertices.clear();
}
void HitboxFrame::addVertex(float relx,float rely){
    vertices.push_back(sf::Vector2f(relx,rely));
}
