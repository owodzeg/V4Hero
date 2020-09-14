#include "KirajinPoweredTowerBig.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "../../../../Func.h"
#include "../../../../V4Core.h"

KirajinPoweredTowerBig::KirajinPoweredTowerBig()
{

}

void KirajinPoweredTowerBig::LoadConfig(Config *thisConfigs)
{
    /// all (normal) kacheeks have the same animations, so we load them from a hardcoded file
    AnimatedObject::LoadConfig(thisConfigs,"resources\\units\\entity\\kirajin_poweredtower_big.p4a");
    AnimatedObject::setAnimationSegment("idle");
}

void KirajinPoweredTowerBig::Draw(sf::RenderWindow& window)
{
    if(dead)
    {
        shake += 2/fps;

        if(shake >= 15)
        shake = 15;

        if(shake_timer.getElapsedTime().asMilliseconds() > shake_delay)
        {
            negative = !negative;

            if(negative)
            local_x = shake * (-1);
            else
            local_x = shake;

            shake_timer.restart();
        }

        if(death_timer.getElapsedTime().asSeconds() >= 1.5)
        {
            local_y += 50.0 / fps;
        }

        if(death_timer.getElapsedTime().asSeconds() >= 3)
        {
            sf::Color c = AnimatedObject::getColor();
            float alpha = c.a;

            alpha -= 250.0 / fps;

            if(alpha <= 0)
            {
                alpha = 0;
                ready_to_erase = true;
            }

            AnimatedObject::setColor(sf::Color(c.r,c.g,c.b,alpha));
        }
    }

    /// call the parent function to draw the animations
    AnimatedObject::Draw(window);
}
void KirajinPoweredTowerBig::OnCollide(CollidableObject* otherObject, int collidedWith, vector<string> collisionData)
{
    cout << "KirajinPoweredTowerBig::OnCollide" << endl;

    if(!dead)
    {
        if(collisionData.size() > 0)
        {
            if(isCollidable)
            {
                ///collisionData received from Projectile, process it
                int dmgDealt = atoi(collisionData[0].c_str());
                curHP -= dmgDealt;

                cout << "I received " << to_string(dmgDealt) << "damage, my HP is " << curHP << endl;
            }
        }

        if((curHP > 0) && (curHP <= maxHP/4))
        {
            if(AnimatedObject::getAnimationSegment() != "idle_damaged")
            AnimatedObject::setAnimationSegment("idle_damaged", true);
        }

        if(curHP <= 0)
        {
            dead = true;

            isCollidable = false;
            isAttackable = false;

            if(!dropped_item)
            {
                ///Drop item mechanism! Really cool!
                cout << "Look at me! I'm a Kirajin Powered Tower Big and I'm dropping an item!" << endl;
                int rng = rand() % 100 + 1; ///select 1 - 100;
                int total_rng = 0;
                int id_picked = 0;

                cout << "Rng: " << rng << endl;

                for(int i=0; i<loot_table.size(); i++)
                {
                    total_rng += loot_table[i].item_chance;

                    if(rng <= total_rng)
                    {
                        id_picked = loot_table[i].item_id;
                        cout << "Picked id: " << id_picked << endl;

                        break;
                    }
                }

                if(id_picked != 0)
                {
                    auto item = thisConfig->thisCore->savereader.itemreg.GetItemByID(id_picked);
                    vector<string> data = {item->spritesheet, to_string(item->spritesheet_id), to_string(id_picked)};

                    thisConfig->thisCore->currentController.spawnEntity("droppeditem",5,0,getGlobalPosition().x-140,0,getGlobalPosition().y+10,0,0,1,sf::Color::White,0,0,0,-1,0,0,vector<Entity::Loot>(), data);

                    dropped_item = true;
                }
                else
                {
                    cout << "No item dropped :(" << endl;
                }
            }

            //AnimatedObject::setAnimationSegment("destroy", true);
            death_timer.restart();
        }
    }

    /// note we don't call the parent function. It does nothing, it just serves
    /// as an incomplete function to be overridden by child classes.
}
