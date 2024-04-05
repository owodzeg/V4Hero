#ifndef PNGANIMATION_H
#define PNGANIMATION_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>

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
            bool cached = false;
            std::vector<std::string> frame_paths;
            std::vector<std::string> spritesheet_paths;
        };

        std::vector<Animation> animations;
        std::unordered_map<std::string, int> animationIDtoName;

        // Animation controls
        float currentFrame = 0;
        float animationSpeed = 30; // frames per second - to be configured by animation param file
        unsigned int currentAnimation = 0;

        PNGAnimation();
        virtual void Load(const std::string& path);
        virtual int getIDfromShortName(const std::string& shortName);
        virtual void Draw();
};

#endif