#ifndef HATAPON_H
#define HATAPON_H

#include <SFML/Graphics.hpp>
#include "../../../Config.h"
#include "../PlayableUnit.h"

using namespace std;

class Hatapon : public PlayableUnit
{
    public:
    sf::SoundBuffer hit_1, hit_2, hit_3;

    Hatapon();
    void LoadConfig(Config *thisConfigs);
    void Draw(sf::RenderWindow& window);
    void OnCollide(CollidableObject* otherObject, int collidedWith=-1, vector<string> collisionData = {});
    void doRhythm(std::string current_song="", std::string current_drum="", int combo=0);
    void doMissionEnd();
};

#endif // PATAPON_H
