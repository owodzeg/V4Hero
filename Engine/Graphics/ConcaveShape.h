#ifndef CONCAVESHAPE_HPP
#define CONCAVESHAPE_HPP
//---------------------------------------------------------------------------//
#include <vector>
//-----------------------------------//
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
//---------------------------------------------------------------------------//

namespace sfml
{

    //-----------------------------------//
    class ConcaveShape : public sf::Drawable, public sf::Transformable
    {
    private:
        const sf::Texture* texture;
        sf::Color color;
        std::vector<sf::VertexArray> triangles;
        void Triangulate(const std::vector<sf::Vector2f>& points);
        float Area(const std::vector<sf::Vector2f>& points) const;
        bool PointOnLine(sf::Vector2f pt, sf::Vector2f v1, sf::Vector2f v2) const;
        bool PointInTriangle(sf::Vector2f p, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3) const;
        void CreateTriangle(unsigned int a, unsigned int b, unsigned int c, const std::vector<sf::Vector2f>& points);

    public:
        explicit ConcaveShape(const std::vector<sf::Vector2f>& points, sf::Color c = sf::Color::Black);
        ConcaveShape(const std::vector<sf::Vector2f>& points, const sf::Texture& t, sf::Color c = sf::Color::Black);
        virtual ~ConcaveShape();
        void setColor(sf::Color c);
        void setTexture(const sf::Texture& t);
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    };
    //-----------------------------------//
} // namespace sfml
#endif //CONCAVESHAPE_HPP------------//