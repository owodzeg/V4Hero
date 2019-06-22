#include "Patapon.h"
#include "math.h"
#include <fstream>
#include <iostream>

Patapon::Patapon()
{

}
void Patapon::LoadConfig(Config *thisConfigs)
{
 thisConfig = thisConfigs;
 ifstream param("resources/graphics/units/patapon/param.dat");

    string buff;
    while(getline(param,buff))
    {
        string name = buff.substr(0,buff.find_first_of(","));
        string frames = buff.substr(buff.find_first_of(",")+1);

        animation_name.push_back(name);
        animation_frames.push_back(atoi(frames.c_str()));
    }

    param.close();

    for(int i=0; i<animation_name.size(); i++)
    {
        thisConfig->debugOut->DebugMessage("Loading animation '" + animation_name[i] + "'...\n");

        for(int a=1; a<=animation_frames[i]; a++)
        {
            sf::Texture temp;
            temp.loadFromFile("resources/graphics/units/patapon/"+animation_name[i]+"/"+to_string(a)+".png");
            temp.setSmooth(true);

            animation_textures[animation_name[i]].push_back(temp);
        }
    }
}
void Patapon::Draw(sf::RenderWindow& window)
{
    s_patapon.setTexture(animation_textures[current_animation][floor(current_frame)]);

    if(current_animation == "walk")
    {
        current_frame += float(24) / fps;
    }
    else
    {
        current_frame += float(30) / fps;
    }

    if(current_frame >= animation_textures[current_animation].size())
    {
        current_frame = 0;

        if((current_animation == "pata") or (current_animation == "pon") or (current_animation == "don") or (current_animation == "chaka"))
        {
            current_animation = "idle";
        }
    }


    s_patapon.setScale(0.6*scaleX,0.6*scaleY);

    s_patapon.setPosition(x,y);

    window.draw(s_patapon);

    if(!((current_animation == "pata") or (current_animation == "pon") or (current_animation == "don") or (current_animation == "chaka")))
    current_animation = "idle";
}
