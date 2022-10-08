#ifndef PNGANIMATION_H
#define PNGANIMATION_H

#include <SFML/Graphics.hpp>
#include <vector>

class PNGAnimation
{
    public:
        struct Animation {
            std::string name;
            std::vector<std::string> frame_paths;
            std::vector<sf::Image> spritesheets;
        };

        std::vector<Animation> animations;

        PNGAnimation();
        virtual void Load(const std::string& path);
        virtual void Draw();
};

#endif