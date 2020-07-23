#include "CollidableObject.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include "../../Func.h"

#include <sstream>
CollidableObject::CollidableObject()
{

}
void CollidableObject::OnCollide(CollidableObject* otherObject, int collidedWith, vector<std::string> collisionData){
    // this space intentionally left blank
    cout<<"[COLLISION_SYSTEM]: Class derived from CollidableObject has not overridden OnCollide() method"<<endl;

}
