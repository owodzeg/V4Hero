#ifndef HATAPON_H
#define HATAPON_H

#include <SFML/Graphics.hpp>
#include "../../../Config.h"
#include "../PlayableUnit.h"

using namespace std;

class Hatapon : public PlayableUnit
{
    public:
    Hatapon();
    void LoadConfig(Config *thisConfigs);
    void Draw(sf::RenderWindow& window);
    void OnCollide(CollidableObject* otherObject);
    void doRhythm(std::string current_song, std::string current_drum);
    void doMissionEnd();
};

#endif // PATAPON_H
