#ifndef WALL_H
#define WALL_H

#include "../../Entity.h"
#include <SFML/Graphics.hpp>

class Wall : public Entity
{
public:
    Wall();
    void LoadConfig();
    void Draw();
    void OnCollide(CollidableObject otherObject);
};

#endif // WALL_H
