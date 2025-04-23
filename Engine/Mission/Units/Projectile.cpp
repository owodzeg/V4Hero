#include "Projectile.h"
#include "../../Item/Item.h"
#include "CollidableObject.h"
#include "HitboxFrame.h"
#include <math.h>
#include "../../CoreManager.h"

Projectile::Projectile(std::string path, float x, float y, float thspeed, float tvspeed, bool evil)
{
    sprite.load(path);
    xPos = x;
    yPos = y;
    hspeed = thspeed;
    vspeed = tvspeed;
    fromEnemy = evil;
}

float Projectile::GetXSpeed()
{
    /// get the x speed of the projectile
    //return speed*cos(angle);
    return hspeed;
}
float Projectile::GetYSpeed()
{
    /// get the y speed of the projectile
    //return speed*sin(angle);
    return vspeed;
}
void Projectile::SetNewSpeedVector(float xSpeed, float ySpeed)
{
    /// set the speed and angle of the projectile

    speed = sqrt(pow(xSpeed, 2) + pow(ySpeed, 2));
    //angle = asin(ySpeed/speed);
    angle = atan2(ySpeed, xSpeed);
}
//void Projectile::OnCollide(CollidableObject* otherObject)
//{
    // this space intentionally left blank
    // SPDLOG_INFO("[COLLISION_SYSTEM]: Class derived from Projectile has not overridden OnCollide() method");
//}
void Projectile::Update()
{
    float fps = CoreManager::getInstance().getCore()->getFPS();

    vspeed += float(3300) / fps;

    /// FOR REWORK: projectile should not be dependant on enemy whether it goes to left or to right.
    /// make a better unified system for projectile flight
    /// this is just a temporary workaround

    if (!fromEnemy)
    {
        hspeed -= float(600) / fps;

        if (hspeed < 0)
            hspeed = 0;
    } else
    {
        hspeed += float(600) / fps;

        if (hspeed > 0)
            hspeed = 0;
    }

    SetNewSpeedVector(hspeed, vspeed);

    hPos += GetXSpeed() / fps;
    vPos += GetYSpeed() / fps;
}

void Projectile::Draw()
{
    sprite.setPosition(xPos + hPos, yPos + vPos + cam_offset);
    sprite.setRotation(3.14159265358 / 2 + angle);
    sprite.draw();

    sf::Vector2f origin = sprite.getPosition();

    // Get the rotation angle in radians (SFML gives angle in degrees)
    float rotationInDegrees = sprite.getRotation();
    float rotationInRadians = rotationInDegrees * (M_PI / 180.0f);

    // Half of the spear length
    float halfLength = sprite.getLocalBounds().size.x / 2.0f;

    // Calculate the tip position using trigonometry
    tipX = origin.x + halfLength * std::cos(rotationInRadians);
    tipY = origin.y + halfLength * std::sin(rotationInRadians);

    if(yPos > 3840)
    {
        finished = true;
    }
}
