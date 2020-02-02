#include "Projectile.h"
#include "../../Item/Item.h"
#include "CollidableObject.h"
#include <math.h>
#include "HitboxFrame.h"

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

Projectile::Projectile(PSprite& tsprite){
    sprite=&tsprite;
}
void Projectile::Draw(sf::RenderWindow &window, float fps){

    /*HitboxFrame tmp;
    tmp.time = 0;
    tmp.g_x = 0;
    tmp.g_y = 0;
    tmp.clearVertices();
    tmp.addVertex(-15,-2); /// "top left"
    tmp.addVertex(15,-2); /// "top right"
    tmp.addVertex(-15,2); /// "bottom left"
    tmp.addVertex(15,2); /// "bottom right"
    tmp.rotation = -angle;
    std::vector<sf::Vector2f> currentVertices = tmp.getCurrentVertices();

    sf::CircleShape shape(3);
    shape.setFillColor(sf::Color(100, 250, 50));
    shape.setPosition(xPos-1.5,yPos-1.5);
    window.draw(shape);

    shape.setFillColor(sf::Color(50, 50, 50));
    for (int i=0;i<currentVertices.size();i++){
        shape.setPosition(currentVertices[i].x+xPos-1.5,currentVertices[i].y+yPos-1.5);
        window.draw(shape);
    }*/

    sprite->setPosition(xPos,yPos);
    sprite->draw(window,3.14159265358/2+angle);
}
