
#include "ConcaveShape.h"
#include <math.h>

namespace sfml 
{
    float epsilon = 0.000000001f;

    ConcaveShape::ConcaveShape(const std::vector<sf::Vector2f>& points, sf::Color c)
        : texture(nullptr), color(c)
    {
        Triangulate(points);
        triangles.shrink_to_fit();
    }

    ConcaveShape::ConcaveShape(const std::vector<sf::Vector2f>& points, const sf::Texture& t, sf::Color c)
        : texture(&t), color(c)
    {
        Triangulate(points);
        triangles.shrink_to_fit();
    }

    ConcaveShape::~ConcaveShape()
    {
        std::vector<sf::VertexArray>().swap(triangles);
        triangles.shrink_to_fit();
        texture = nullptr;
    }

    float ConcaveShape::Area(const std::vector<sf::Vector2f>& points) const
    {
        float A = 0.f;
        unsigned int n = points.size(), p = n - 1;
        for (unsigned int q = 0; q != n; ++q)
        {
            A += points[p].x * points[q].y - points[q].x * points[p].y;
            p = q;
        }
        return A * 0.5f;
    }

    bool ConcaveShape::PointOnLine(sf::Vector2f pt, sf::Vector2f v1, sf::Vector2f v2) const
    {
        if (fabs(v2.x - v1.x) > 0)
        {
            if (fabs(pt.y - v1.y - (v2.y - v1.y) * (pt.x - v1.x) / (v2.x - v1.x)) < epsilon)
            {
                return true;
            } else
            {
                return false;
            }
        }
        return false;
    }

    bool ConcaveShape::PointInTriangle(sf::Vector2f pt, sf::Vector2f v1, sf::Vector2f v2, sf::Vector2f v3) const
    {
        float denominator = (v2.x - v3.x) * (v1.y - v3.y) - (v2.y - v3.y) * (v1.x - v3.x);
        if (fabs(denominator) < epsilon)
        {
            return false;
        }
        float a = ((pt.y - v3.y) * (v2.x - v3.x) - (pt.x - v3.x) * (v2.y - v3.y)) / denominator;
        float b = ((pt.x - v3.x) * (v1.y - v3.y) - (pt.y - v3.y) * (v1.x - v3.x)) / denominator;
        if ((!std::isfinite(a)) || (!std::isfinite(b)))
        {
            return true;
        }
        if ((!std::signbit(a)) && (!std::signbit(b)) && (std::signbit(a + b - 1.0f)))
        {
            return true;
        } else
        {
            if ((!std::signbit(a + epsilon)) && (!std::signbit(b + epsilon)) && (std::signbit(a + b - 1.0f)))
            {
                if (PointOnLine(pt, v3, v1))
                {
                    return true;
                }
            }
            return false;
        }
    }

    void ConcaveShape::CreateTriangle(unsigned int a, unsigned int b, unsigned int c, const std::vector<sf::Vector2f>& points)
    {
        if ((a < points.size()) && (b < points.size()) && (c < points.size()))
        {
            triangles.push_back(sf::VertexArray(sf::PrimitiveType::Triangles, 3));
            triangles.back()[0].position = points[a];
            triangles.back()[1].position = points[b];
            triangles.back()[2].position = points[c];

            triangles.back()[0].color = color;
            triangles.back()[1].color = color;
            triangles.back()[2].color = color;

            triangles.back()[0].texCoords = points[a];
            triangles.back()[1].texCoords = points[b];
            triangles.back()[2].texCoords = points[c];
        }
    }

    void ConcaveShape::Triangulate(const std::vector<sf::Vector2f>& points)
    {
        register unsigned int n = points.size();
        std::vector<unsigned int> p;
        if (0.f < Area(points))
        {
            for (unsigned int i = 0; i != n; ++i)
            {
                p.push_back(i);
            }
        } else
        {
            for (unsigned int i = 0; i != n; ++i)
            {
                p.push_back((n - 1) - i);
            }
        }
        if (p.size() > 3)
        {
            register unsigned int next = 0, next2 = 0, start = 0, check = 0;
            while ((p.size() > 3) && (check < (2 * p.size())))
            {
            Label:
                start++;
                check++;

                if (start >= p.size())
                {
                    start = 0;
                }
                next = start + 1;
                if (next >= p.size())
                {
                    next = 0;
                }
                next2 = next + 1;
                if (next2 >= p.size())
                {
                    next2 = 0;
                }
                sf::Vector2f a = points[p[start]];
                sf::Vector2f b = points[p[next]];
                sf::Vector2f c = points[p[next2]];
                if (check > 100) {
                    // this is added to catch the rendering program if the algorithm gets into an infinite loop.
                    // throw an error - the concave shape will error if the border crosses itself. A simple example would be a shape with corners like 
                    //                  [0,0] [1,0] [0,1] [1,1] (makes hourglass shape with border crossing in the middle)
                    // sorry about the dirty inline class definition it was getting upset when defining it in the header or outside the function
                    class ShapeSelfIntersectError : public std::exception{public:ShapeSelfIntersectError(){}};
                    throw ShapeSelfIntersectError(); 
                }

                if (-epsilon > ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)))
                {
                    goto Label;
                } else
                {
                    for (unsigned int i = 0; i != n; ++i)
                    {
                        if ((i != p[start]) && (i != p[next]) && (i != p[next2]))
                        {
                            if (PointInTriangle(points[i], a, b, c))
                            {
                                goto Label;
                            }
                        }
                    }
                }
                check = 0;
                CreateTriangle(p[start], p[next], p[next2], points);
                p.erase(p.begin() + next);
            }
        }
        if (p.size() == 3)
        {
            CreateTriangle(p[0], p[1], p[2], points);
        }
    }

    void ConcaveShape::setColor(sf::Color c)
    {
        color = c;
        for (auto& i : triangles)
        {
            i[0].color = color;
            i[1].color = color;
            i[2].color = color;
        }
    }

    void ConcaveShape::setTexture(const sf::Texture& t)
    {
        texture = &t;
    }

    void ConcaveShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.texture = texture;
        for (auto& i : triangles)
        {
            target.draw(i, states);
        }
    }

} // namespace sfml