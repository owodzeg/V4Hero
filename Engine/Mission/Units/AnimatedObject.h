#ifndef ANIMATEDOBJECT_H
#define ANIMATEDOBJECT_H

#include <SFML/Graphics.hpp>
#include "../../Config.h"
#include "../../P4A.h"
#include "Object.h"
#include "Hitbox.h"

using namespace std;

class AnimatedObject
{
    public:
    vector<Object> objects;
    vector<Hitbox> hitboxes;
    float max_time = 0;
    float cur_pos = 0;

    ///OLD
    sf::Sprite s_wall;
    string current_animation = "";
    float anim_begin = 0;
    float anim_end = 0;

    float current_frame = 0;
    ///depracated
    //float x = 0;
    //float y = 520;
    float global_x = 0;
    float global_y = 0;
    float fps = 60;
    float width = 0;
    float scaleX = 1, scaleY = 1; ///TEMPORARY
    sf::Rect<float> hitBox;

    ///temporary
    bool worm_fever = false;

    bool loopable = true;

    map<string,vector<sf::Texture>> animation_textures;

    vector<float> animation_begin;
    vector<float> animation_end;
    vector<string> animation_names;


    vector<int> animation_frames;
    Config *thisConfig;
    AnimatedObject();
    void loadAnim(std::string data, P4A handle);
    void setAnimationSegment(std::string new_segment_name);
    std::string getAnimationSegment();
    void setGlobalPosition(sf::Vector2f pos);
    sf::Vector2f getGlobalPosition();
    void moveGlobalPosition(sf::Vector2f pos);
    void setLoop(bool loop);
    virtual void LoadConfig(Config *thisConfigs,std::string unitParamPath);
    virtual void Draw(sf::RenderWindow& window);
};

#endif // ANIMATEDOBJECT_H
