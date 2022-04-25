#include <vector>
#include <Graphics/CurveSegment.h>
#include <SFML/Graphics/Color.hpp>
#include <nlohmann/json.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#ifndef CRV_SHP
#define CRV_SHP

class CurveShape 
{
public:
    std::vector<CurveSegment*> curves;
    std::vector<sf::Vector2f> points;
    sf::Color fillcolor;
    sf::Color outlinecolor;
    CurveShape(nlohmann::json data);
    CurveShape(std::vector<CurveSegment*> curves, sf::Color fillcolor);

    void Draw();
    void UpdatePoints();
    ~CurveShape();
};


#endif // CRV_SHP
