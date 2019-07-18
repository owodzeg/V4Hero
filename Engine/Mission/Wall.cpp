#include "Wall.h"
#include "math.h"
#include <fstream>
#include <iostream>

#include <sstream>
Wall::Wall()
{

}
std::vector<std::string> splitp(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}
void Wall::LoadConfig(Config *thisConfigs)
{
 thisConfig = thisConfigs;
 ifstream param("resources/graphics/units/wall/param.dat");

    string buff;
    getline(param,buff);
    string name = buff.substr(0,buff.find_first_of(":"));
    string coords = buff.substr(buff.find_first_of(":")+1);

    std::vector<std::string> results = splitp(coords,',');


 float ratioX, ratioY;
    switch(thisConfigs->GetInt("textureQuality"))
    {
        case 0: ///low
        {
            ratioX = thisConfig->GetInt("resX") / float(640);
            ratioY = thisConfig->GetInt("resY") / float(360);
            break;
        }

        case 1: ///med
        {
            ratioX = thisConfig->GetInt("resX") / float(1280);
            ratioY = thisConfig->GetInt("resY") / float(720);
            break;
        }

        case 2: ///high
        {
            ratioX = thisConfig->GetInt("resX") / float(1920);
            ratioY = thisConfig->GetInt("resY") / float(1080);
            break;
        }

        case 3: ///ultra
        {
            ratioX = thisConfig->GetInt("resX") / float(3840);
            ratioY = thisConfig->GetInt("resY") / float(2160);
            break;
        }
    }

    hitBox =  sf::Rect<float>(atof(results[0].c_str())*ratioX,atof(results[1].c_str())*ratioY,atof(results[2].c_str())*ratioX,atof(results[3].c_str())*ratioY);
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
    current_frame += float(60) / fps;

    if(current_animation == "walk")
    {
        x += float(140) / fps;

        if(walk_timer.getElapsedTime().asSeconds() >= 2)
        {
            current_animation = "idle";
        }
    }

    if(current_frame >= animation_textures[current_animation].size())
    {
        current_frame = 0;
    }


    s_wall.setScale(scaleX,scaleY);

    s_wall.setPosition(x,y);
    width = s_wall.getGlobalBounds().width;
    sf::Rect<float> spriteSize = s_wall.getGlobalBounds();
    //cout<< spriteSize.left<<" "<<spriteSize.top<<" "<<spriteSize.width<<" "<<spriteSize.height<<endl;
    //cout<< hitBox.left<<" "<<hitBox.top<<" "<<hitBox.width<<" "<<hitBox.height<<endl;
    //hitBox = sf::Rect<float>(x,y,spriteSize.width,spriteSize.height);

    window.draw(s_wall);
}
