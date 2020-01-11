#ifndef FEVERWORM_H
#define FEVERWORM_H

#include <SFML/Graphics.hpp>
#include "../../Config.h"
#include "CollidableObject.h"

using namespace std;

class FeverWorm : public CollidableObject
{
    public:
    float next_x = -250;
    float speed = 120;

    FeverWorm();
    void LoadConfig(Config *thisConfigs);
    void Draw(sf::RenderWindow& window);
};

#endif // FEVERWORM_H
