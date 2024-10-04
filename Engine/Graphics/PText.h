#ifndef PTEXT_H
#define PTEXT_H

#include <SFML/Graphics.hpp>
#include "RichText.hpp"
#include <memory>

///Class for automatic text resizing based on the current window resolution and quality options
// TODO: add text wrap functionality

class PText
{
public:
    std::string font = "fallback"; // font name to fetch from strRepo
    int characterSize = 28*3;

    sfe::RichText t;
    sf::String txt;

    std::string txt_color = "";

    bool forceColor = false;
    sf::Color color, oldColor;

    int qualitySetting, resSetting=3;

    float ratioX, ratioY;
    float resRatioX, resRatioY;

    float orX = 0, orY = 0;
    float scaleX = 1, scaleY = 1;
    float lx = 0, ly = 0;
    float angle = 0;

    bool wide = false;
    bool rendered = false;

    sf::String oldtxt = "";

    std::string currentKey;
    std::string oldKey;

    sf::Clock char_timeout;
    sf::Clock char_wait;
    int char_shown = 0;
    float char_speed = 20;
    float char_delay = 50;
    float char_wait_period = 0;
    int current_line = 0;
    int char_buffer = 0;
    bool speech = false;
    bool fadein = false;
    bool speech_done = false;
    bool force_nonspeech = false; //for pre-rendering text to get dimensions
    bool speedup = false; //for dialogue speeding up
    bool goback = false; //var exclusive for dialogues. it lets the MessageCloud class know that it should go back to first dialogue
    float fadein_length = 0;
    sf::Clock fadein_clock;

    struct TextSettings
    {
        int pos = 0;
        int speed = 0;
        int timeout = 0;
    };

    std::vector<TextSettings> textSettings;

    PText();
    std::vector<std::string> split(std::string const & s, char delim);
    void setFont(const std::string& dst_font);
    void setCharacterSize(int newCS);
    void setTextQuality(int quality);
    void processRichText();
    void addText(sf::String add_text);


    void setOrigin(float x, float y);
    sf::Vector2f getScale();
    void setScale(float x, float y);
    void setRotation(float a);
    void setColor(sf::Color color);
    void setOutlineColor(sf::Color color);
    void setOutlineThickness(int thick);
    void setPosition(float x, float y);
    void setStringKey(std::string text_key);
    void setString(std::string text_string);
    void setString(sf::String text_string);
    void setString(const char* text_string);
    void setWString(std::string text_string);
    sf::Vector2f getPosition();
    sf::FloatRect getLocalBounds();
    sf::FloatRect getGlobalBounds();
    sf::FloatRect getGlobalBoundsScaled();
    sf::FloatRect getTransformedBounds();
    void setScale(float s);

    void draw();
    sf::Text getText();
};

#endif // PTEXT_H
