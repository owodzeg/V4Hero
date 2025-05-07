#ifndef OBJECT_H
#define OBJECT_H

#include "../../Graphics/PSprite.h"
#include <SFML/Graphics.hpp>

class Object
{
private:
    bool debug = false;

public:
    std::string texture_path;
    std::string object_name;

    sf::Texture tex_obj;
    PSprite s_obj;

    float g_x = 0, g_y = 0;   ///global position
    float gl_x = 0, gl_y = 0; ///global-local position
    float g_sx = 1, g_sy = 1; ///global scale

    float x = 0, y = 0, r = 0, rs = 0; ///local position
    float old_x = 0, old_y = 0, old_r = 0; ///old position
    float or_x = 0, or_y = 0;          ///origin (for rotation)
    float s_x = 1, s_y = 1;    ///scale

    float g_r = 0; ///global rotation

    int layer = 0;
    int parent = 0;

    bool first_set = false;

    struct Frame {
        float time = 0;     ///timestamp
        float pos_x = 0;    ///x position
        float pos_y = 0;    ///y position
        float or_x = 0;     ///x origin
        float or_y = 0;     ///y origin
        float rotation = 0; ///rotation
        float scale_x = 1;  ///x scale
        float scale_y = 1;  ///y scale
    };

    std::vector<Frame> frames; ///frames

    struct Pixel {
        uint16_t x = 0;
        uint16_t y = 0;
        sf::Color color;
    };

    sf::Texture spritesheet; ///current spritesheet
    sf::Color color;

    ///disable drawing object when its out of bounds
    bool disable = true;
    bool exported = false;

    // wip - rework
    std::string tex_path = "";

    Object();
    Object(std::string mem, int xpos, int ypos, int l, int p);
    void Load(std::string filename, int xpos, int ypos);
    void Load(sf::Texture& texture, int xpos, int ypos);
    void LoadFromMemory(std::string filename, int xpos, int ypos);
    void swapTexture(sf::Image first, std::vector<Pixel> px);
    void swapTexture(std::string img_key);
    void SetFrame(float time);
    void SetCustomFrame(float in_time, float in_pos_x, float in_pos_y, float in_or_x, float in_or_y, float in_rotation, float in_scale_x, float in_scale_y);
    void SetPos(float time);
    void SetPosFrame(float time, int frame); //Try to find frames from already prepared frame
    void Draw(int orx, int ory);
    void Draw(int x1, int y1, int x2, int y2, int orx, int ory);
};

#endif // OBJECT_H
