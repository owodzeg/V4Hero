#ifndef PNGANIMATION_H
#define PNGANIMATION_H

#include <SFML/Graphics.hpp>
#include <Graphics/SpriteWrapper.h>
#include <vector>
#include <unordered_map>

class PNGAnimationException : public std::exception {
private:
    std::string message;

public:
    PNGAnimationException(std::string msg) : message(msg) {}
    char* what () {
        return message.data();
    }
};

class PNGAnimation
{
    public:
        std::string model_name;

        struct Animation {
            std::string name;
            std::string shortName;
            unsigned int img_x = 0;
            unsigned int img_y = 0;
            unsigned int frames = 0;
            unsigned int maxCols = 0;
            unsigned int maxRows = 0;
            unsigned int origin_x = 0;
            unsigned int origin_y = 0;
            std::vector<sf::FloatRect> hitboxes;
            bool cached = false;
            bool zip = false;
            bool customOrigin = false;
            std::vector<std::string> frame_paths;
            std::vector<std::string> spritesheet_paths;
        };

        std::vector<Animation> animations;
        std::unordered_map<std::string, int> animationIDtoName;

        // Animation controls
        float currentFrame = 0;
        float animationSpeed = 30; // frames per second - to be configured by animation param file
        unsigned int currentAnimation = 0;
        bool isPlaying = true;
        bool isLooping = true;

        // Object controls
        sf::Vector2f position = {0, 0};
        sf::Vector2f scale = {1, 1};
        float rotation = 0;
        sf::Color color = sf::Color(255, 255, 255);

        // static stuff
        unsigned int maxSize = 512;

        std::vector<std::pair<int,int>> animationStalls;
        std::vector<std::pair<int,int>> animationSwitchTo;
        std::vector<int> animationPause;

        int q = -1;
        int qscale = 1;

        std::vector<std::pair<std::string, SpriteWrapper>> extra;
        struct ExtraFrame
        {
            float x=0, y=0, r=0;
        };

        // ex_frames[extra_name][animation][frame] = ExtraFrame
        std::unordered_map<std::string, std::unordered_map<int, std::unordered_map<int, ExtraFrame>>> ex_frames;

        PNGAnimation();
        virtual sf::Image getAnimationImage(const std::string& anim_path, const std::string& image_path, bool zipped = false);
        virtual void loadCacheFile(Animation& anim);
        virtual void generateSpritesheet(Animation& anim, const std::string& anim_path);
        virtual bool getAnimationCache(Animation& anim);
        virtual void Load(const std::string& path);
        virtual int getIDfromShortName(const std::string& shortName);
        virtual std::string getShortNameFromID(int ID);
        virtual void setAnimation(const std::string& animShortName);
        virtual std::string getAnimation();
        virtual void addAnimationStalling(const std::string& animShortName, int frame, int duration);

        virtual void Draw();
};

#endif