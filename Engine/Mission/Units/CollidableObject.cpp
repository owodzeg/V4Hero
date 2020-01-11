#include "CollidableObject.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include "../../Func.h"

#include <sstream>
CollidableObject::CollidableObject()
{

}
void CollidableObject::OnCollide(CollidableObject* otherObject){
    // this space intentionally left blank
    cout<<"[COLLISION_SYSTEM]: Class derived from CollidableObject has not overridden OnCollide() method"<<endl;

}
