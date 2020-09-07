#include "TreasureChest.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "../../../../Func.h"
#include "../../../../V4Core.h"

TreasureChest::TreasureChest()
{

}

void TreasureChest::LoadConfig(Config *thisConfigs)
{
    /// all (normal) kacheeks have the same animations, so we load them from a hardcoded file
    AnimatedObject::LoadConfig(thisConfigs,"resources\\units\\entity\\treasure_chest.p4a");
    AnimatedObject::setAnimationSegment("idle");
}

void TreasureChest::Draw(sf::RenderWindow& window)
{
    if(dead)
    {
        if(death_timer.getElapsedTime().asSeconds() >= 3)
        {
            sf::Color c = AnimatedObject::getColor();
            float alpha = c.a;

            alpha -= 510.0 / fps;

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
void TreasureChest::OnCollide(CollidableObject* otherObject, int collidedWith, vector<string> collisionData)
{
    cout << "TreasureChest::OnCollide" << endl;

    if((AnimatedObject::getAnimationSegment() != "destroy") && (AnimatedObject::getAnimationSegment() != "destroyed"))
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

        if(curHP <= 0)
        {
            dead = true;

            isCollidable = false;
            isAttackable = false;

            if(!dropped_item)
            {
                ///Drop item mechanism! Really cool!
                cout << "Look at me! I'm a Treasure Chest and I'm dropping an item!" << endl;
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
                    ///check if grubby map was obtained, if not, force drop it
                    if(!thisConfig->thisCore->savereader.invdata.CheckItemObtained(23)) ///check for grubby map
                    id_picked = 23; ///override drop if grubby map not achieved

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

            AnimatedObject::setAnimationSegment("destroy", true);
            death_timer.restart();
        }
    }

    /// note we don't call the parent function. It does nothing, it just serves
    /// as an incomplete function to be overridden by child classes.
}
