#ifndef HATAPON_H
#define HATAPON_H

#include "../PlayableUnit.h"
#include <SFML/Graphics.hpp>

using namespace std;

class Hatapon : public PlayableUnit
{
public:
    sf::SoundBuffer hit_1, hit_2, hit_3;

    Hatapon();
    void LoadConfig();
    void Draw(sf::RenderWindow& window);
    void OnCollide(CollidableObject* otherObject, int collidedWith = -1, vector<string> collisionData = {});
    void doRhythm(std::string current_song = "", std::string current_drum = "", int combo = 0);
    void doMissionEnd();
};

#endif // PATAPON_H
