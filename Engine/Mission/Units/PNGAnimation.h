#ifndef PNGANIMATION_H
#define PNGANIMATION_H

#include <SFML/Graphics.hpp>
#include <vector>

class PNGAnimation
{
    public:
        struct Animation {
            std::string name;
            std::string shortName;
            unsigned int img_x = 0;
            unsigned int img_y = 0;
            unsigned int frames = 0;
            unsigned int maxCols = 0;
            unsigned int maxRows = 0;
            unsigned int sheetsNeeded = 0;
            bool cached = false;
            std::vector<std::string> frame_paths;
            std::vector<std::string> spritesheet_paths;
        };

        std::vector<Animation> animations;

        PNGAnimation();
        virtual void Load(const std::string& path);
        virtual void Draw();
};

#endif