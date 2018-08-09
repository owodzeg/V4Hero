#ifndef DRUM_H
#define DRUM_H

#include <SFML/Graphics.hpp>

class Drum
{
    public:
    sf::Texture t_drum;
    sf::Sprite s_drum;

    sf::Clock drumClock;
    float alpha = 255;

    float x,y;
    float fps;

    Drum();
    void Load(std::string drum, sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);
};

#endif // DRUM_H
