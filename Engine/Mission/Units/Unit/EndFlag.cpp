#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "EndFlag.h"

EndFlag::EndFlag()
{
    main.LoadConfig("resources/units/entity/end_flag.zip");
    main.setAnimation("idle");
}

void EndFlag::Draw()
{
    main.setGlobalPosition(sf::Vector2f(global_x, global_y));
    main.setLocalPosition(sf::Vector2f(local_x, local_y));
    main.Draw();
}