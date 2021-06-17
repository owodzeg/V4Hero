#include "Kirajin_Yari_1.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "../../../../Func.h"
#include "../../../../V4Core.h"

Kirajin_Yari_1::Kirajin_Yari_1()
{
    /// Kirajin to be placed on watchtowers, stones, etc.
    /// No significant AI other than occasionally throwing spears is necessary.
}

void Kirajin_Yari_1::LoadConfig(Config *thisConfigs)
{
    /// all (normal) kacheeks have the same animations, so we load them from a hardcoded file
    AnimatedObject::LoadConfig(thisConfigs,"resources/units/entity/kirajin.p4a");
    AnimatedObject::setAnimationSegment("idle_armed_focused");
}

void Kirajin_Yari_1::parseAdditionalData(nlohmann::json additional_data)
{
    action = IDLE;
    swap_layer = 0;
    talk = false;
    talk_id = "";
    custom_dmg = false;
    view_range = 750;

	if(additional_data.contains("forceSpawnOnLvl"))
	{
		force_spawn = true;
		force_spawn_lvl = additional_data["forceSpawnOnLvl"];
	}

	if(additional_data.contains("forceDropIfNotObtained"))
	{
		force_drop = true;
		force_drop_item = additional_data["forceDropIfNotObtained"][0];

		if(additional_data["forceDropIfNotObtained"][1] != "any")
		{
			force_drop_mission_lvl = additional_data["forceDropIfNotObtained"][1];
		}
	}

	if(additional_data.contains("hidden"))
	{
		action = HIDING;
		swap_layer = additional_data["hidden"];
	}

	if(additional_data.contains("talk"))
	{
		talk = true;
		talk_id = additional_data["talk"];
	}

	if(additional_data.contains("equip"))
	{
		for(int i = 0; i < additional_data["equip"].size(); i++)
		{
			if(additional_data["equip"][i] != "") // allows for overriding helmet but not spear etc.
			{
				applyEquipment(thisConfig->thisCore->saveReader.itemReg.getItemByName(additional_data["equip"][i])->order_id, i);
			}
		}
	}

	if(additional_data.contains("damage"))
	{
		custom_dmg = true;
		
		mindmg = additional_data["damage"][0];
		maxdmg = additional_data["damage"][1];
	}

	if(additional_data.contains("range"))
	{
		view_range = additional_data["range"];
	}
}

bool Kirajin_Yari_1::doAttack()
{
    if(threw)
    {
        threw = false;
        return true;
    }
    return false;
}

void Kirajin_Yari_1::doMessages(sf::RenderWindow& window, float fps, InputController& inputCtrl)
{
    vector<int> m_rm;

    for(int i=0; i<messageclouds.size(); i++)
    {
        messageclouds[i].startpos = sf::Vector2f(getGlobalPosition().x-5, getGlobalPosition().y-25);

        if(messageclouds[i].firstrender)
        messageclouds[i].Show();

        if(message_clock.getElapsedTime().asSeconds() >= 5)
        messageclouds[i].End();

        if((messageclouds[i].done) && (floor(messageclouds[i].xsize) == 0) && (floor(messageclouds[i].ysize) == 0))
        messageclouds[i].Hide();

        messageclouds[i].Draw(window, fps, inputCtrl);

        if((!messageclouds[i].active) && (messageclouds[i].done))
        m_rm.push_back(i);
    }

    for(int i=0; i<m_rm.size(); i++)
    {
        messageclouds.erase(messageclouds.begin()+m_rm[i]-i);
    }
}

void Kirajin_Yari_1::Draw(sf::RenderWindow& window)
{
    if(dead)
    {
        cout << "I'm dead now" << endl;

        if(getAnimationSegment() == "stagger")
        {
            cout << "Animation segment is stagger " << cur_pos << " " << anim_end << endl;

            if(cur_pos >= anim_end)
            {
                cout << "Setting death animation" << endl;

                setAnimationSegment("death", true);
            }
        }

        if(death_timer.getElapsedTime().asSeconds() > 5)
        {
            cout << "Death clock passed 3 seconds. Time to bury into the ground" << endl;

            if(getAnimationSegment() == "death")
            {
                cout << "I am despawning" << endl;
                setAnimationSegment("death_despawn", true);

                dropItem();
            }

            if(getAnimationSegment() == "death_despawn")
            {
                if(cur_pos >= anim_end)
                {
                    ready_to_erase = true;
                }
            }
        }
    }
    else
    {
        ///Not dead
        if(getAnimationSegment() == "stagger")
        {
            if(cur_pos >= anim_end)
            {
                setAnimationSegment("idle_armed_focused", true);
            }
        }
        else
        {
            if(enemy_in_range)
            {
                if(local_x > 5)
                {
                    if(getAnimationSegment() != "walk_yari_focused")
                    setAnimationSegment("walk_yari_focused");

                    local_x -= float(100) / fps;
                }

                if(local_x < -5)
                {
                    if(getAnimationSegment() != "walk_yari_focused")
                    setAnimationSegment("walk_yari_focused");

                    local_x += float(100) / fps;
                }

                if((local_x <= 5) && (local_x >= -5))
                {
                    if(attack_timer.getElapsedTime().asSeconds() >= 6)
                    {
                        if(getAnimationSegment() != "attack_prefever")
                        setAnimationSegment("attack_prefever", true);

                        if(getAnimationSegment() == "attack_prefever")
                        {
                            if(getAnimationPos() >= 0.4666)
                            {
                                if(canThrow)
                                {
                                    threw = true;
                                    canThrow = false;
                                }
                            }

                            if(cur_pos >= anim_end)
                            {
                                canThrow = true;
                                threw = false;
                                attack_timer.restart();
                            }
                        }
                    }
                    else
                    {
                        if(getAnimationSegment() != "idle_armed_focused")
                        setAnimationSegment("idle_armed_focused");
                    }
                }
            }
        }

        if(enemy_in_range)
        {
            if(action != HIDING)
            {
                if(talk)
                {
                    MessageCloud tmp;
                    tmp.Create(20, sf::Vector2f(getGlobalPosition().x-5, getGlobalPosition().y-25), sf::Color(222,102,102,255), false, thisConfig->GetInt("textureQuality"), thisConfig->fontPath);
                    tmp.AddDialog(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(talk_id)), false);
                    messageclouds.push_back(tmp);

                    message_clock.restart();

                    talk = false;
                }
            }
        }
    }

    if(distance_to_unit <= view_range)
    {
        enemy_in_range = true;
    }
    else
    {
        enemy_in_range = false;
    }

    vspeed += gravity / fps;

    if(hspeed > 0)
    hspeed -= 230.0 / fps;
    else
    hspeed = 0;

    if(vspeed >= 0)
    {
        if(local_y >= 0)
        {
            vspeed = 0;
            local_y = 0;
        }
    }

    local_x += hspeed / fps;
    local_y += vspeed / fps;

    /// call the parent function to draw the animations
    AnimatedObject::Draw(window);
}

void Kirajin_Yari_1::die()
{
    if(!dead)
    {
        //cout << "Kirajin_Yari_1::die()" << endl;

        dead = true;
        hspeed = 340;
        vspeed = -250;

        //cout << global_y << " " << floorY << endl;
        //cout << "Set local_y = " << global_y - floorY << endl;
        local_y = global_y - floorY;
        //cout << "Set global_y = " << global_y + floorY << endl;
        global_y -= local_y;

        isCollidable = false;
        isAttackable = false;

        AnimatedObject::setAnimationSegment("stagger", true);
        death_timer.restart();
    }
}

void Kirajin_Yari_1::OnCollide(CollidableObject* otherObject, int collidedWith, vector<string> collisionData)
{
    cout << "Kirajin_Yari_1::OnCollide" << endl;

    if(AnimatedObject::getAnimationSegment() != "death")
    {
        if(collisionData.size() > 0)
        {
            if(isCollidable)
            {
                ///collisionData received from Projectile, process it
                int dmgDealt = atoi(collisionData[0].c_str());
                curHP -= dmgDealt;

                cout << "I received " << to_string(dmgDealt) << "damage, my HP is " << curHP << "/10" << endl;

                //hspeed = 120 + (rand() % 50);
                //vspeed = -170;
                AnimatedObject::setAnimationSegment("stagger", true);
            }
        }

        if(curHP <= 0)
        {
            die();
        }
    }

    /// note we don't call the parent function. It does nothing, it just serves
    /// as an incomplete function to be overridden by child classes.
}
