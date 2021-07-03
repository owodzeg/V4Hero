#ifndef OBJECT_H
#define OBJECT_H

#include "../../Graphics/PSprite.h"
#include <SFML/Graphics.hpp>

using namespace std;

class Object
{
private:
    bool debug = false;

public:
    std::string texture_path;
    std::string object_name;

    sf::Texture tex_obj;
    PSprite s_obj;

    float g_x, g_y;   ///global position
    float gl_x, gl_y; ///global-local position
    float g_sx, g_sy; ///global scale

    float x, y, r = 0, rs = 0; ///local position
    float old_x, old_y, old_r; ///old position
    float or_x, or_y;          ///origin (for rotation)
    float s_x = 1, s_y = 1;    ///scale

    float g_r = 0; ///global rotation

    int layer = 0;
    int parent = 0;

    bool first_set = false;

    struct Frame {
        float time;     ///timestamp
        float pos_x;    ///x position
        float pos_y;    ///y position
        float or_x;     ///x origin
        float or_y;     ///y origin
        float rotation; ///rotation
        float scale_x;  ///x scale
        float scale_y;  ///y scale
    };

    vector<Frame> frames; ///frames

    struct Pixel {
        uint16_t x;
        uint16_t y;
        sf::Color color;
    };

    sf::Texture spritesheet; ///current spritesheet
    sf::Color color;

    ///disable drawing object when its out of bounds
    bool disable = true;
    bool exported = false;

    Object();
    Object(std::string mem, int xpos, int ypos, int l, int p);
    void Load(string filename, int xpos, int ypos);
    void Load(sf::Texture& texture, int xpos, int ypos);
    void LoadFromMemory(string filename, int xpos, int ypos);
    void swapTexture(sf::Image img);
    void swapTexture(sf::Image first, vector<Pixel> px);
    void SetFrame(float time);
    void SetCustomFrame(float in_time, float in_pos_x, float in_pos_y, float in_or_x, float in_or_y, float in_rotation, float in_scale_x, float in_scale_y);
    void SetPos(float time);
    void SetPosFrame(float time, int frame); //Try to find frames from already prepared frame
    void Draw(sf::RenderWindow& window, int orx, int ory);
    void Draw(sf::RenderWindow& window, int x1, int y1, int x2, int y2, int orx, int ory);
};

#endif // OBJECT_H
