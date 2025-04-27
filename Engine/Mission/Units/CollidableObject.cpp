#include "CollidableObject.h"
#include "../../Func.h"
#include "math.h"
#include <fstream>
#include <iostream>

#include <sstream>
CollidableObject::CollidableObject()
{
}
void CollidableObject::OnCollide(CollidableObject* otherObject, int collidedWith, std::vector<std::string> collisionData)
{
    // this space intentionally left blank
    //cout << "[COLLISION_SYSTEM]: Class derived from CollidableObject has not overridden OnCollide() method" << endl;
}
