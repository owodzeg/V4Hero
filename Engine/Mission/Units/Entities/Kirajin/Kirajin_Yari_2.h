#ifndef KIRAJIN_YARI_2_H
#define KIRAJIN_YARI_2_H

#include <SFML/Graphics.hpp>
#include "../../../../Config.h"
#include "../../Entity.h"

using namespace std;

class Kirajin_Yari_2 : public Entity
{
    public:
    sf::Clock death_timer;
    sf::Clock attack_timer;

    bool canThrow = true;
    bool threw = false;
    bool enemy_in_range = false;

    enum ACTIONS
    {
        HIDING = 0,
        IDLE = 1,
        ATTACK = 2,
        WALK = 3
    };

    int action = 1;
    int attackmode = -1;

    sf::Clock attack_clock;
    float attack_speed = 2.0;

    float hspeed = 0;
    float vspeed = 0;
    float gravity = 981;

    ///distance to take away from patapons
    float dest_distance = 450;

    bool jumped = false;
    bool talk = false;
    string talk_id = "";

    sf::Clock message_clock;
    sf::Clock walk_clock; ///position updates

    Kirajin_Yari_2();
    void LoadConfig(Config *thisConfigs);
    void Draw(sf::RenderWindow& window);
    bool doAttack();
    void doMessages(sf::RenderWindow& window, float fps, InputController& inputCtrl);
    void die();
    void OnCollide(CollidableObject* otherObject, int collidedWith=-1, vector<string> collisionData = {});
};

#endif // KIRAJIN_YARI_2
