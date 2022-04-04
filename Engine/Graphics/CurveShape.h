#include <vector>
#include <Graphics/CurveSegment.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#ifndef CRV_SHP
#define CRV_SHP

class CurveShape 
{
public:
    std::vector<CurveSegment> curves;
    sf::Color fillcolor;
    sf::Color outlinecolor;
    CurveShape(std::vector<CurveSegment> curves, sf::Color fillcolor, sf::Color outlinecolor);

    void Draw(sf::RenderWindow*);
    ~CurveShape();
};


#endif // CRV_SHP
