#ifndef PATATEXT_H
#define PATATEXT_H

#include <SFML/Graphics.hpp>

class PataText
{
    /* 
    Patafour's Rich Text implementation for better dialogues:

    # text style
    {outline 2 0-255 0-255 0-255} = sets text outline to 2 pixel thick, r g b color
    {color 0-255 0-255 0-255} = changes color to r g b values
    {regular} = resets bold, italic, underline, strike at once
    {bold} = Toggles bold on/off
    {italic} = Toggles italic on/off
    {underline} = Toggles underline on/off
    {strike} = Toggles strike-through on/off
    {size 16} = sets text size to 16.
    {font Name} = will try to load font with name "Name" according to resources/lang/fonts.txt

    # animations
    {hwave 4 10} = sets the text to be wavy, horizontally, with wave power = 4, speed = 10
    {vwave 4 10} = sets the text to be wavy, vertically, with wave power = 4, speed = 10

    # technical
    {global 1} = displays global text with id 1
    {n} = new line

    # dialogue
    {goback} = lets the message cloud know to revert to the first text in dialogue (for patapolis npc's to return to their greeting)
    {speed 10} = sets text speed to 10 letters per sec
    {wait 500} = waits for 500 milliseconds before putting next characters

    # single-use at the beginning
    {fadein 1000} = text will start at 0% opacity and fade to 100% opacity during 1000 milliseconds.
    {speech} = makes the text appear letter by letter, use if you dont want the text to appear instantly.

    # notes
    all letters will have their respective clock, aka their timeout
    when timeout is passed, next letter can appear

    */

	public:
        struct PTStyle {
            // font kerning
            double kerning = 0;            // regular
            double additional_kerning = 0; // if we want to extend the gaps

            // rotation
            double rotation = 0;

            // manual offset
            double x_offset = 0, y_offset = 0, r_offset = 0;

            // color
            double c_red = 0, c_green = 0, c_blue = 0, c_alpha = 255;

            // size
            double char_size = 72;

            // font
            std::shared_ptr<sf::Font> font;
            std::string fontStr = "fallback";

            // visual settings
            bool display = false;
            bool fadein = false;

            // style
            bool bold = false, italic = false, underline = false, strike = false;

            // movement style
            double shake = 0, hwave = 0, hwave_speed = 0, vwave = 0, vwave_speed = 0;

            // outline
            double thickness = 0;
            double ot_c_red = 0, ot_c_green = 0, ot_c_blue = 0, ot_c_alpha = 255;

            // display settings
            double nextCharTimeout = 0; // milliseconds to wait before showing next character
                                        // 0 is default, which will make the text appear instantly

            double curCharTimeout = 0; // milliseconds to wait before showing current character
                                       // 0 is default, which will make the text appear instantly

            // misc
            int offset = 0; // which character (in total)
            int line_offset = 0; // which character (in specific line)
        };

        struct PTChar {
            // base obj for storing the character and render object

            // Default constructor
            PTChar() : character(0)
            {
            }

            // Constructor with parameters
            PTChar(char32_t newChar, const PTStyle& newStyle) : character(newChar), style(newStyle)
            {
            }

            char32_t character;
            sf::Text* text = nullptr;
            sf::Vector2f position;
            PTStyle style;
        };

        // constructor
        PataText();
        ~PataText();
        PTStyle default_style; // default style set

        // affects whole text
        void applyDefaultKerning(); // auto applies kerning data from the font file
        void applyFadeIn(); //sets alpha to 0 so the whole text can fade in
        void applyDialog(); //sets display to 0, inits reading marker
        
        // affects style
        void styleSetAdditionalKerning(PTStyle& style, double newKerning);
        void styleSetXOffset(PTStyle& style, double newXOffset);
        void styleSetYOffset(PTStyle& style, double newYOffset);
        void styleSetFullColor(PTStyle& style, sf::Color newColor);
        void styleSetRedColor(PTStyle& style, double newRed);
        void styleSetGreenColor(PTStyle& style, double newGreen);
        void styleSetBlueColor(PTStyle& style, double newBlue);
        void styleSetAlpha(PTStyle& style, double newAlpha);
        void styleSetCharSize(PTStyle& style, double newCharSize);
        void styleSetFont(PTStyle& style, const std::string& fontStr);
        void styleSetBold(PTStyle& style, bool isBold);
        void styleSetItalic(PTStyle& style, bool isItalic);
        void styleSetUnderline(PTStyle& style, bool isUnderline);
        void styleSetStrike(PTStyle& style, bool isStrike);
        void styleToggleBold(PTStyle& style);
        void styleToggleItalic(PTStyle& style);
        void styleToggleUnderline(PTStyle& style);
        void styleToggleStrike(PTStyle& style);
        void styleSetShake(PTStyle& style, double newShake);
        void styleSetHorizontalWave(PTStyle& style, double newHWave, double newHWaveSpeed);
        void styleSetVerticalWave(PTStyle& style, double newVWave, double newVWaveSpeed);
        void styleSetOutlineFullColor(PTStyle& style, sf::Color newColor);
        void styleSetOutlineRedColor(PTStyle& style, double newRed);
        void styleSetOutlineGreenColor(PTStyle& style, double newGreen);
        void styleSetOutlineBlueColor(PTStyle& style, double newBlue);
        void styleSetOutlineAlpha(PTStyle& style, double newAlpha);
        void styleSetOutlineThickness(PTStyle& style, double newThickness);
        void styleSetSpeed(PTStyle& style, double newMsSpeed);
        void styleSetTimeout(PTStyle& style, double newMsTimeout); // SINGLE USE! Only for the current character.
        void styleResetAllStyles(PTStyle& style);

        void defaultStyleSetColor(sf::Color newColor);
        void defaultStyleSetCharSize(double newCharSize);
        void defaultStyleSetFont(const std::string& fontStr);
        void defaultStyleSetOutlineColor(sf::Color newColor);
        void defaultStyleSetOutlineThickness(double newThickness);

        // PataText functions
        void append(sf::String& input_text);
        void append(std::string input_text);
        void append(const char* input_text);
        void reset();

        void setGlobalPosition(double new_x, double new_y);
        sf::Vector2f getGlobalPosition();

        void setGlobalOrigin(double new_x, double new_y);
        sf::Vector2f getGlobalOrigin();

        sf::FloatRect getGlobalBounds();

        void draw();

	private:
        std::vector<sf::String> split_and_remove(const sf::String& s, const sf::String& delim); // split string by delimiter (remove delimeter)
        std::vector<std::string> split_fast(const std::string& s, char delim); // fast split (std::string)
        std::vector<sf::String> extract_tokens(const sf::String& str);

        void ProcessStyleToken(const sf::String& token);
        void ProcessRegularText(const sf::String& token);
        void ProcessPositioning();

        sf::Clock effect;
        sf::Clock dialogue_clock;
        double random = 0;

        PTStyle m_marker; // current text style
        sf::Vector2f m_position; // current character position
        double m_interline = 4; // pixels between lines
        double global_x = 0, global_y = 0; // global position
        double origin_x = 0, origin_y = 0; // origin position
        double max_width = 0, max_height = 0; // bounds
        int m_counter = 0, m_counter_line = 0; // counters
        bool refreshPositioning = true; // do it only when we're moving somewhere/changing something
        std::vector<std::vector<PTChar>> m_lines; // text data. vector consists of lines (separated by {n}), lines consist of PTChar objects. 
};

#endif
