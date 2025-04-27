#ifndef PSPRITE_H
#define PSPRITE_H

#include <SFML/Graphics.hpp>
#include <memory>    // Include for std::unique_ptr
#include <stdexcept> // For potential exceptions
#include <string>    // Include for std::string
#include <utility>   // Include for std::swap, std::move

/// Class for automatic sprite resizing based on the current window resolution and quality options
class PSprite
{
public:
    std::unique_ptr<sf::Sprite> s;
    std::string texturePath;
    int qualitySetting = -1, oldQualitySetting = -999;
    int resSetting = 3, oldResSetting = -999;
    float ratioX = 1, ratioY = 1;
    float scaleX = 1, scaleY = 1;
    float orX = 0, orY = 0;
    float baseX = -999, baseY = -999;
    float lx = 0, ly = 0;
    float angle = 0;
    bool exported = false;

    // Default constructor
    PSprite() = default;

    // Copy constructor
    PSprite(const PSprite& other)
        : texturePath(other.texturePath), 
          qualitySetting(other.qualitySetting),
          oldQualitySetting(other.oldQualitySetting),
          resSetting(other.resSetting),
          oldResSetting(other.oldResSetting),
          ratioX(other.ratioX),
          ratioY(other.ratioY),
          scaleX(other.scaleX),
          scaleY(other.scaleY),
          orX(other.orX),
          orY(other.orY),
          baseX(other.baseX),
          baseY(other.baseY),
          lx(other.lx),
          ly(other.ly),
          angle(other.angle),
          exported(other.exported)
    {
        if (other.s)
        {
            s = std::make_unique<sf::Sprite>(*other.s);
        } else
        {
            s = nullptr;
        }
    }

    PSprite& operator=(const PSprite& other)
    {
        PSprite temp(other);
        swap(temp);
        return *this;
    }

    PSprite(PSprite&& other) noexcept = default;
    PSprite& operator=(PSprite&& other) noexcept = default;

    void swap(PSprite& other) noexcept
    {
        using std::swap;
        swap(s, other.s);
        swap(texturePath, other.texturePath);
        swap(qualitySetting, other.qualitySetting);
        swap(oldQualitySetting, other.oldQualitySetting);
        swap(resSetting, other.resSetting);
        swap(oldResSetting, other.oldResSetting);
        swap(ratioX, other.ratioX);
        swap(ratioY, other.ratioY);
        swap(scaleX, other.scaleX);
        swap(scaleY, other.scaleY);
        swap(orX, other.orX);
        swap(orY, other.orY);
        swap(baseX, other.baseX);
        swap(baseY, other.baseY);
        swap(lx, other.lx);
        swap(ly, other.ly);
        swap(angle, other.angle);
        swap(exported, other.exported);
    }

    void loadFromFile(std::string file, int q, bool downscale = true);
    template<typename T>
    void loadFromFile(std::string file, int q, T) = delete;
    void setRepeated(bool r);
    void setTextureRect(sf::IntRect rect);
    sf::IntRect getTextureRect();
    void setOrigin(float x, float y);
    void setScale(float x, float y);
    void setRotation(float a);
    void setColor(sf::Color color);
    sf::Color getColor();
    void setTexture(sf::Texture& texture);
    void applyTexture();
    void setPosition(float x, float y);
    sf::Vector2f getPosition();
    sf::FloatRect getLocalBounds();
    sf::FloatRect getGlobalBounds();
    sf::FloatRect getGlobalBoundsScaled();
    void setScale(float ss);
    void setSmooth(bool smooth);
    void drawShader(sf::RenderWindow* window, sf::Shader& shader);
    void draw();
    void update(sf::RenderWindow& window);
    void saveToFile(std::string file);

    sf::Sprite* get() const
    {
        return s.get();
    }
};

// Allows std::swap(psprite1, psprite2);
inline void swap(PSprite& a, PSprite& b) noexcept
{
    a.swap(b);
}

#endif // PSPRITE_H
