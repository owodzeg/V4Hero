#include "Hatapon.h"

Hatapon::Hatapon()
{
    main.LoadConfig("resources/units/unit/hatapon.zip");
    main.setAnimation("idle");
}

void Hatapon::Advance()
{
    main.setAnimation("wave");
}

void Hatapon::StopAll()
{
    main.setAnimation("idle");
}

void Hatapon::Draw()
{
    main.setGlobalPosition(sf::Vector2f(global_x, global_y));
    main.setLocalPosition(sf::Vector2f(local_x, local_y));
    main.Draw();
}