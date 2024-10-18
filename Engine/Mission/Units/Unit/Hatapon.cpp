#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "Hatapon.h"

Hatapon::Hatapon()
{
    main.LoadConfig("resources/units/unit/hatapon.zip");
    main.setAnimation("idle");
}

void Hatapon::Wave()
{
    main.setAnimation("wave");
}

void Hatapon::StopAll()
{
    main.setAnimation("idle");
}

std::vector<sf::FloatRect> Hatapon::getHitbox()
{
    return main.animation.animations[main.animation.currentAnimation].hitboxes;
}

void Hatapon::Draw()
{
    main.setGlobalPosition(sf::Vector2f(global_x, global_y + cam_offset));
    main.setLocalPosition(sf::Vector2f(local_x, local_y));
    main.Draw();
}