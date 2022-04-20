#include "Hatapon.h"
#include "../../../Func.h"
#include "../../../CoreManager.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include <sstream>

Hatapon::Hatapon()
{
}

void Hatapon::LoadConfig()
{
    /// all (normal) kacheeks have the same animations, so we load them from a hardcoded file
    AnimatedObject::LoadConfig("resources/units/unit/hatapon.p4a");

    hit_1.loadFromFile("resources/sfx/level/hit_1.ogg");
    hit_2.loadFromFile("resources/sfx/level/hit_2.ogg");
    hit_3.loadFromFile("resources/sfx/level/hit_3.ogg");

    cur_sound.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));

    isCollidable = true;
    isAttackable = true;
}

void Hatapon::Draw(sf::RenderWindow& window)
{
    /// call the parent function to draw the animations
    AnimatedObject::Draw();
}

void Hatapon::OnCollide(CollidableObject* otherObject, int collidedWith, vector<string> collisionData)
{
    /// note we don't call the parent function. It does nothing, it just serves
    /// as an incomplete function to be overridden by child classes.

    cout << "Hatapon::OnCollide" << endl;

    if (collisionData.size() > 0)
    {
        if (isCollidable)
        {
            ///collisionData received from Projectile, process it
            int dmgDealt = atoi(collisionData[0].c_str());

            if (defend)
            {
                if (charged)
                    dmgDealt = round(dmgDealt / 4);
                else
                    dmgDealt = round(dmgDealt / 2);
            }

            current_hp -= dmgDealt;

            cout << "I received " << to_string(dmgDealt) << "damage, my HP is " << current_hp << endl;

            cur_sound.stop();

            int a = rand() % 3;

            switch (a)
            {
                case 0:
                    cur_sound.setBuffer(hit_1);
                    break;

                case 1:
                    cur_sound.setBuffer(hit_2);
                    break;

                case 2:
                    cur_sound.setBuffer(hit_3);
                    break;
            }

            cur_sound.play();
        }
    }

    /// note we don't call the parent function. It does nothing, it just serves
    /// as an incomplete function to be overridden by child classes.
}

void Hatapon::doRhythm(std::string current_song, std::string current_drum, int combo)
{
    if ((current_song != "") && ((current_song != "dondon") && (current_song != "ponpata")))
    {
        setAnimationSegment("wave");
    } else if (current_song == "dondon")
    {
        setAnimationSegment("jump");
    } else if (current_song == "ponpata")
    {
        setAnimationSegment("flee");
    } else
    {
        setAnimationSegment("idle");
    }
}

void Hatapon::doMissionEnd()
{
    setAnimationSegment("wave");
}
