#ifndef HATAPON_H
#define HATAPON_H

#include <SFML/Graphics.hpp>
#include "../../Config.h"
#include "CollidableObject.h"

using namespace std;

class Hatapon : public CollidableObject
{
    public:
    Hatapon();
    void LoadConfig(Config *thisConfigs);
    void Draw(sf::RenderWindow& window);
    void OnCollide(CollidableObject* otherObject);
};

#endif // PATAPON_H
