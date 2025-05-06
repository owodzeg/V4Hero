#include "HitboxFrame.h"
#include "../../Math/PVector.h"
#include "../../Constants.h"
#include <cmath>

HitboxFrame::HitboxFrame()
{
}

HitboxFrame::~HitboxFrame()
{
}

float HitboxFrame::minProjection(float axisAngle, float object_x, float object_y)
{
    float projectionLength = 99999999;
    std::vector<sf::Vector2f> currentVertices = getCurrentVertices();

    //cout<<" - SEARCHING FOR MIN PROJECTION"<<endl;
    /// go through each vertex (corner) and find the shortest projection from the origin to that vertex along the axis aligned with axisAngle
    for (int i = 0; i < currentVertices.size(); i++)
    {
        sf::Vector2f currentVertex = currentVertices.at(i);
        PVector cornerVector = PVector::getVectorCartesian(0, 0, currentVertex.x + g_x + object_x, currentVertex.y + g_y + object_y);
        cornerVector.angle = -atan2(currentVertex.y + g_y + object_y, currentVertex.x + g_x + object_x);
        float currentProjectionLength = cornerVector.GetScalarProjectionOntoAxis(axisAngle);

        //cout<<"# currentProjectionLength: "<<i<<"axisAngle: "<<axisAngle<<" val: "<<currentProjectionLength<<" gx: "<<object_x<<" gy: "<<object_y<<endl;
        if (currentProjectionLength < projectionLength)
        {
            projectionLength = currentProjectionLength;
        }
    }
    //cout<<"Min projection hitbox: "<<projectionLength<<endl;
    return projectionLength;
}

float HitboxFrame::maxProjection(float axisAngle, float object_x, float object_y)
{
    float projectionLength = 0;

    std::vector<sf::Vector2f> currentVertices = getCurrentVertices();
    float angled = PI / 2;
    if (axisAngle == angled)
    {
        //cout<<" "<<endl;
    }
    //cout<<" - SEARCHING FOR MAX PROJECTION"<<endl;
    /// go through each vertex (corner) and find the longest projection from the origin to that vertex along the axis aligned with axisAngle
    for (int i = 0; i < currentVertices.size(); i++)
    {
        sf::Vector2f currentVertex = currentVertices.at(i);
        PVector cornerVector = PVector::getVectorCartesian(0, 0, currentVertex.x + g_x + object_x, currentVertex.y + g_y + object_y);
        cornerVector.angle = -atan2(currentVertex.y + g_y + object_y, currentVertex.x + g_x + object_x);
        float currentProjectionLength = cornerVector.GetScalarProjectionOntoAxis(axisAngle);
        if (axisAngle == angled)
        {
            //cout<<"angle: "<<cornerVector.angle<<" axisAngle: "<<axisAngle<<" angleDiff: "<<(cornerVector.angle)-axisAngle<<endl;
        }
        //cout<<"# currentProjectionLength: "<<i<<"axisAngle: "<<axisAngle<<" val: "<<currentProjectionLength<<" gx: "<<object_x<<" gy: "<<object_y<<endl;
        if (currentProjectionLength > projectionLength)
        {
            projectionLength = currentProjectionLength;
        }
    }

    //cout<<"Max projection hitbox: "<<projectionLength<<endl;
    return projectionLength;
}
std::vector<sf::Vector2f>* HitboxFrame::getBaseVerticiesDontUseThisUnlessYouKnowWhy()
{
    return &vertices;
}
/// <summary>
/// calculate the maximum width
/// </summary>
/// <param name="maxWidth">Optional - provide a precalculated max width. Used for projectiles</param>
void HitboxFrame::calcMaxWidth(float maxWidth, bool forceRecalc)
{
    // default argument maxWidth has a value of -1 if not provided. If it's not -1, it has been precalculated
    if (maxWidth != -1)
    {
        max_width = maxWidth;
        return;
    }
    float maxlength = 0;
    if (max_width > 0 && !forceRecalc)
    {
        // it has been calculated before: skip calculating it again
        return;
    }
    // check distance between each pair of vertices. on a rectangle shape, both diagonals are identical, so we can check distance from each vertex to the first vertex
    // note i=1 - we skip checking the distance from the first vertex to itself.
    sf::Vector2f firstVertex = vertices[0];
    for (int i = 1; i < vertices.size(); i++) 
    {
        sf::Vector2f currentVertex = vertices[i];
        float dist = sqrt(pow(currentVertex.x - firstVertex.x, 2) + pow(currentVertex.y - firstVertex.y, 2) * 1.0);
        if (dist>maxlength) 
        {
            maxlength = dist;
        }
    }
    max_width = maxlength;

    // bugfix for hitboxes where all the vertices are on one side of the origin (0,0)
    // we gonna calc distance from origin to each vertex and if the biggest is larger than maxlength, use that as our max_width instead
    float maxlength_origin = 0;
    for (int i = 1; i < vertices.size(); i++)
    {
        sf::Vector2f currentVertex = vertices[i];
        float dist = sqrt(pow(currentVertex.x - 0, 2) + pow(currentVertex.y - 0, 2) * 1.0);
        if (dist > maxlength_origin)
        {
            maxlength_origin = dist;
        }
    }
    if (maxlength_origin > maxlength) 
    {
        max_width = maxlength_origin;
    }

    return;
}
std::vector<sf::Vector2f> HitboxFrame::getCurrentVertices()
{
    /// oh man.... we need to apply the transformation matrix to this bad boy;
    /// transformation matrix is 4x4 so we need a 4x1 std::vector for each point
    /// yes, it is a 3d transformation even though we are in 2d, thats just how the math goes because a 2D rotation is a rotation about the z-axis
    /// it will be as follows:
    ///     -----       ---------------------------------------------------------
    ///     | x |       | cos(angle)*scaleX    -sin(angle)          0   scaleX  |
    ///     | y |       | sin(angle)            cos(angle)*scaleY   0   scaleY  |
    ///     | 0 |   *   | 0                     0                   1   0       |
    ///     | 1 |       | g_xpos                g_ypos              0   1       |
    ///     -----       ---------------------------------------------------------
    /// result: a 1x4 std::vector

    /// Apply changes only if rotation has changed, if rotation havent changed, theres no need to recalculate the vertices
    if (lastRotation != rotation)
    {
        /// helper values, as they never change
        float c = cos(rotation);
        float s = sin(rotation);

        for (int i = 0; i < vertices.size(); i++)
        {
            sf::Vector2f currentVertex = vertices[i];

            /// I have worked through the matrix maths and calculated the following results, which have some minor optimisations

            /// scaleX*(gx + x*cos(angle) + y*sin(angle))
            float resultX = scaleX * (g_x + currentVertex.x * c + currentVertex.y * s);

            /// scaleY*(gy + y*cos(angle) - x*sin(angle))
            float resultY = scaleY * (g_y + currentVertex.y * c - currentVertex.x * s);

            vertices[i] = sf::Vector2f(resultX, resultY);
        }
    }

    lastRotation = rotation;

    return vertices;
}
void HitboxFrame::clearVertices()
{
    vertices.clear();
}
void HitboxFrame::addVertex(float relx, float rely)
{
    vertices.push_back(sf::Vector2f(relx, rely));
}
