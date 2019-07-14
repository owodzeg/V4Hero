#include "Wall.h"
#include "math.h"
#include <fstream>
#include <iostream>

Wall::Wall()
{

}
void Wall::LoadConfig(Config *thisConfigs)
{
 thisConfig = thisConfigs;
 ifstream param("resources/graphics/units/wall/param.dat");

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
            temp.loadFromFile("resources/graphics/units/wall/"+animation_name[i]+"/"+to_string(a)+".png");
            temp.setSmooth(true);

            animation_textures[animation_name[i]].push_back(temp);
        }
    }
}
void Wall::Draw(sf::RenderWindow& window)
{
    s_wall.setTexture(animation_textures[current_animation][floor(current_frame)]);

    /// walls won't be making walking animations but ill leave this here for now
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

    /// walls won't be making pata animations but ill leave this here for now
        if((current_animation == "pata") or (current_animation == "pon") or (current_animation == "don") or (current_animation == "chaka"))
        {
            current_animation = "idle";
        }
    }


    s_wall.setScale(0.6*scaleX,0.6*scaleY);

    s_wall.setPosition(x,y);
    width = s_wall.getGlobalBounds().width;
    sf::Rect<float> spriteSize = s_wall.getGlobalBounds();
    hitBox = sf::Rect<float>(x,y,spriteSize.width,spriteSize.height);

    window.draw(s_wall);

    /// walls won't be making pata animations but ill leave this here for now
    if(!((current_animation == "pata") or (current_animation == "pon") or (current_animation == "don") or (current_animation == "chaka")))
    current_animation = "idle";
}
