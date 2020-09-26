#include "DroppedItem.h"
#include "../../../../V4Core.h"

DroppedItem::DroppedItem()
{

}

void DroppedItem::LoadConfig(Config *thisConfigs)
{
    AnimatedObject::LoadConfig(thisConfigs,"resources\\units\\entity\\droppeditem.p4a");

    /// You need to load the appropriate spritesheet for given item and quality settings.
    float rand_hs = (rand() % 500) / float(10);
    float rand_vs = (rand() % 500) / float(10);

    hspeed = -200+rand_hs;
    vspeed = -600+rand_vs;

    cout << "DroppedItem spawned with hspeed " << hspeed << " and vspeed " << vspeed << endl;

    manual_spritesheet = true;

    s_item.loadFromFile("resources/sfx/level/picked_item.ogg");
    s_keyitem.loadFromFile("resources/sfx/level/picked_keyitem.ogg");

    cur_sound.setVolume(float(thisConfigs->GetInt("masterVolume"))*(float(thisConfigs->GetInt("sfxVolume"))/100.f));
}

void DroppedItem::Draw(sf::RenderWindow& window)
{
    /// Draw the DroppedItem, however! Lock the animation status to make it appear inanimate
    if(!pickedup)
    {
        resRatioX = window.getSize().x / float(1280);
        resRatioY = window.getSize().y / float(720);

        vspeed += float(981) / fps;
        hspeed += float(100) / fps;

        int q = objects[0].s_obj.qualitySetting;

        ///i want: 20
        ///it can be:
        ///quality 0: 10
        ///quality 2: 30
        ///quality 3: 60

        float multiplier = 1;
        switch(q)
        {
            case 0:
            {
                multiplier = 0.5;
                break;
            }
            case 2:
            {
                multiplier = 1.5;
                break;
            }
            case 3:
            {
                multiplier = 3;
                break;
            }
        }

        int ground = 610 - (animation_bounds[index][curFrame].height / 2 / multiplier);

        if(vspeed > 0)
        {
            if(global_y >= ground)
            {
                vspeed = 0;
                global_y = ground;
            }
        }

        if(hspeed >= 0)
        hspeed = 0;

        AnimatedObject::moveGlobalPosition(sf::Vector2f(hspeed/fps,vspeed/fps));

        if(glowTimer.getElapsedTime().asMilliseconds() > 250)
        {
            GlowCircle tmp;
            tmp.circle.setFillColor(sf::Color(255,255,255,tmp.alpha));
            tmp.circle.setRadius(tmp.radius*resRatioX);
            tmp.circle.setPosition(global_x*resRatioX,global_y*resRatioY);
            glow.push_back(tmp);

            glowTimer.restart();
        }

        vector<int> rm;

        for(int i=0; i<glow.size(); i++)
        {
            glow[i].alpha -= 64 / fps;
            glow[i].radius += (50*resRatioY) / fps;

            if(glow[i].alpha <= 0)
            glow[i].alpha = 0;

            glow[i].circle.setFillColor(sf::Color(255,255,255,glow[i].alpha));
            glow[i].circle.setRadius(glow[i].radius);
            glow[i].circle.setOrigin(glow[i].circle.getLocalBounds().width/2, glow[i].circle.getLocalBounds().height/2);

            window.draw(glow[i].circle);

            if(glow[i].alpha <= 0)
            rm.push_back(i);
        }

        for(int i=0; i<rm.size(); i++)
        {
            glow.erase(glow.begin() + (rm[i] - i));
        }
    }
    else
    {
        if(anim_state == 0)
        {
            destXscale = 0.6;
            destYscale = 1.5;
            local_yPosDest = -250;

            if(curXscale > destXscale)
            curXscale -= abs(curXscale - destXscale)*12 / fps;
            if(curYscale < destYscale)
            curYscale += abs(curYscale - destYscale)*12 / fps;
            if(local_y > local_yPosDest)
            local_y -= abs(local_y - local_yPosDest)*12 / fps;

            if(pickupClock.getElapsedTime().asMilliseconds() >= 210)
            {
                curXscale = 0.6;
                curYscale = 1.5;
                local_yPosDest = -250;

                anim_state = 1;
            }
        }

        if(anim_state == 1)
        {
            destXscale = 1;
            destYscale = 1;
            local_yPosDest = -50;

            if(curXscale < destXscale)
            curXscale += abs(curXscale - destXscale)*12 / fps;
            if(curYscale > destYscale)
            curYscale -= abs(curYscale - destYscale)*12 / fps;
            if(local_y < local_yPosDest)
            local_y += abs(local_y - local_yPosDest)*12 / fps;

            if(pickupClock.getElapsedTime().asMilliseconds() >= 330)
            {
                curXscale = 1;
                curYscale = 1;
                local_yPosDest = -50;
                hspeed2 = 0.1;

                anim_state = 2;
            }
        }

        if(anim_state == 2)
        {
            if(pickupClock.getElapsedTime().asMilliseconds() >= 380)
            {
                vspeed2 -= 8 / fps;
                hspeed2 += (hspeed2*4.5) / fps;

                if(vspeed2 <= -8)
                vspeed2 = -8;

                if(hspeed2 >= 8)
                alpha -= 500.0 / fps;

                if(alpha <= 0)
                alpha = 0;

                local_y += vspeed2 / fps * 60;

                local_x += hspeed2 / fps * 60;

                curXscale -= 0.1 / fps;
                curYscale -= 0.1 / fps;

                if(curXscale <= -1)
                curXscale = -1;
                if(curYscale <= -1)
                curYscale = -1;

                AnimatedObject::setColor(sf::Color(255,255,255,alpha));

                if((hspeed2 >= 1000) || (alpha <= 0))
                ready_to_erase = true;
            }
        }
    }

    force_origin_null = true;
    manual_spritesheet = true;

    animation_origins[index][curFrame].x = animation_bounds[index][curFrame].width/2;
    animation_origins[index][curFrame].y = animation_bounds[index][curFrame].height/2;

    scaleX = curXscale;
    scaleY = curYscale;

    AnimatedObject::Draw(window);
}
void DroppedItem::OnCollide(CollidableObject* otherObject, int collidedWith, vector<string> collisionData)
{
    /// Animate the item obtaining and save the obtained item to a temporary list in MissionController
    /// Then, when the mission is properly passed, add the items from the list to the save data.

    if(collisionData.size() == 0) ///collision data needs to be empty so it will not be executed when spears touch items
    {
        if(!pickedup)
        {
            if((picked_item != 33) && (picked_item != 34))
            {
                if(picked_item == 23)
                {
                    cur_sound.stop();
                    cur_sound.setBuffer(s_keyitem);
                    cur_sound.play();
                }
                else
                {
                    cur_sound.stop();
                    cur_sound.setBuffer(s_item);
                    cur_sound.play();
                }

                ///add to the missioncontroller list so it can be displayed in the upper right corner VERY COOL!
                thisConfig->thisCore->currentController.addPickedItem(spritesheet, spritesheet_id, picked_item);

                ///do visuals
                pickedup = true;
                anim_state = 0;
                pickupClock.restart();
            }
            else
            {
                //cur_sound.stop();
                //cur_sound.setBuffer(s_heal);
                //cur_sound.play();

                thisConfig->thisCore->currentController.projectile_sounds.emplace_back();

                thisConfig->thisCore->currentController.projectile_sounds[thisConfig->thisCore->currentController.projectile_sounds.size()-1].setBuffer(thisConfig->thisCore->currentController.s_heal);

                thisConfig->thisCore->currentController.projectile_sounds[thisConfig->thisCore->currentController.projectile_sounds.size()-1].setVolume(float(thisConfig->GetInt("masterVolume"))*(float(thisConfig->GetInt("sfxVolume"))/100.f));
                thisConfig->thisCore->currentController.projectile_sounds[thisConfig->thisCore->currentController.projectile_sounds.size()-1].play();

                thisConfig->thisCore->currentController.addPickedItem(spritesheet, spritesheet_id, picked_item);
                ready_to_erase = true;
            }
        }
    }
}
