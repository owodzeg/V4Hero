#include "Yaripon.h"

Yaripon::Yaripon(int which, int maxpons)
{
    main.LoadConfig("resources/units/unit/yaripon.zip");
    main.setAnimation("Units_Patapon_Idle_2");

    order = which;
    maxp = maxpons;

    float gap = 120 - (5*maxp);
    local_x = -gap + order*gap;
}

void Yaripon::Advance()
{
    main.setAnimation("Units_Patapon_Walk");
}

void Yaripon::StopAll()
{
    main.setAnimation("Units_Patapon_Idle_2");
}

void Yaripon::Drum(std::string drum)
{
    drum[0] = toupper(drum[0]);
    main.setAnimation("Units_Patapon_"+drum);
    main.restartAnimation();
}

void Yaripon::Draw()
{
    main.setGlobalPosition(sf::Vector2f(global_x, global_y));
    main.setLocalPosition(sf::Vector2f(local_x, local_y));
    main.Draw();
}