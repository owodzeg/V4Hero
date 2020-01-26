#include "Projectile.h"
#include "../../Item/Item.h"
#include "CollidableObject.h"
#include <math.h>


using namespace std;

float Projectile::GetXSpeed(){
    /// get the x speed of the projectile
    return speed*cos(angle);
}
float Projectile::GetYSpeed(){
    /// get the y speed of the projectile
    return speed*sin(angle);
}
void Projectile::SetNewSpeedVector(float xSpeed, float ySpeed){
    /// set the speed and angle of the projectile
    speed = sqrt(pow(xSpeed,2)+pow(ySpeed,2));
    angle = asin(ySpeed/speed);
}
void Projectile::OnCollide(CollidableObject* otherObject){
    // this space intentionally left blank
    cout<<"[COLLISION_SYSTEM]: Class derived from Projectile has not overridden OnCollide() method"<<endl;
}
void Projectile::Update(sf::RenderWindow &window, float fps){
    xPos+=GetXSpeed()/fps;
    yPos+=GetYSpeed()/fps;
}
void Projectile::Draw(sf::RenderWindow &window, float fps){
    sf::CircleShape shape(5);
    shape.setFillColor(sf::Color(100, 250, 50));
    shape.setPosition(xPos-2.5,yPos-2.5);
    window.draw(shape);
}
