#ifndef WALL_H
#define WALL_H

#include "../../../../Config.h"
#include "../../Entity.h"
#include <SFML/Graphics.hpp>

using namespace std;

class Wall : public Entity
{
public:
    Wall();
    void LoadConfig();
    void Draw();
    void OnCollide(CollidableObject otherObject);
};

#endif // WALL_H
