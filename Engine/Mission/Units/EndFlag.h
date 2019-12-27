#ifndef ENDFLAG_H
#define ENDFLAG_H

#include <SFML/Graphics.hpp>
#include "../../Config.h"
#include "CollidableObject.h"

using namespace std;

class EndFlag : public CollidableObject
{
    public:

    EndFlag();
    void LoadConfig(Config *thisConfigs);
    void Draw(sf::RenderWindow& window);
    void OnCollide(CollidableObject* otherObject);
};

#endif // ENDFLAG_H
