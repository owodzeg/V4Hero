#ifndef KIRAJIN_YARI_1_H
#define KIRAJIN_YARI_1_H

#include <SFML/Graphics.hpp>
#include "../../../../Config.h"
#include "../../Entity.h"

using namespace std;

class Kirajin_Yari_1 : public Entity
{
    public:
    sf::Clock death_timer;
    sf::Clock attack_timer;
    bool dead = false;

    bool canThrow = true;
    bool threw = false;

    float hspeed = 0;
    float vspeed = 0;
    float gravity = 981;

    Kirajin_Yari_1();
    void LoadConfig(Config *thisConfigs);
    void Draw(sf::RenderWindow& window);
    bool doAttack();
    void OnCollide(CollidableObject* otherObject, int collidedWith=-1, vector<string> collisionData = {});
};

#endif // KIRAJIN_YARI_1
