#ifndef DRUM_H
#define DRUM_H

#include <SFML/Graphics.hpp>
#include "../Graphics/PSprite.h"

class Drum
{
    public:
    PSprite s_drum;
    PSprite s_flash;

    sf::CircleShape c_shockwave;
    sf::CircleShape c_shockwave2;

    sf::Clock drumClock;
    float alpha = 255;

    float x=0,y=0;
    float x_scale=1,y_scale=1;

    float x_flashscale=0.01,y_flashscale=0.01;
    float flashalpha = 125;

    float shockwaveSize = 100;
    float shockwaveAlpha = 20;

    float shockwave2Size = 30;
    float shockwave2Alpha = 40;


    float rotation = 0;
    float fps=60;
    bool isBest = false;
    bool isDon = false;

    std::vector<sf::CircleShape> c_particle;
    int max_radius = 12;
    std::vector<bool> particle_didStart;
    std::vector<float> particle_x;
    std::vector<float> particle_y;
    std::vector<float> particle_angle;
    std::vector<float> particle_radius;
    std::vector<float> particle_curDistance;
    std::vector<float> particle_maxDistance;
    std::vector<float> particle_speed;

    int pattern = 0;
    std::string cur_drum;

    ///pattern["pata"]["x"] = vector<float>
    std::map<std::string, std::map<std::string, std::vector<float>>> patterns;

    Drum();
    void Load(std::string drum, int perfection, sf::Texture& drum_texture, sf::Texture& flash_texture);
    void Draw(sf::RenderWindow& window);
};

#endif // DRUM_H
