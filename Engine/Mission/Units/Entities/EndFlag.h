#ifndef ENDFLAG_H
#define ENDFLAG_H

#include <SFML/Graphics.hpp>
#include "../../../Config.h"
#include "../Entity.h"

using namespace std;

class EndFlag : public Entity
{
    public:
    sf::Clock endMissionClock;
    bool missionEnd = false;

    EndFlag();
    void LoadConfig(Config *thisConfigs);
    void Draw(sf::RenderWindow& window);
    void OnCollide(CollidableObject* otherObject, int collidedWith=-1, vector<string> collisionData = {});
};

#endif // ENDFLAG_H
