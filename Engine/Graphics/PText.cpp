#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "PText.h"
#include <regex>
#include <iostream>
#include <spdlog/spdlog.h>
#include "../CoreManager.h"
#include "../Func.h"

using namespace std;

PText::PText()
{
}

std::vector<std::string> PText::split(std::string const & s, char delim)
{
    std::vector<std::string> result;
    std::istringstream iss(s);

    for (std::string token; std::getline(iss, token, delim); )
    {
        result.push_back(std::move(token));
    }

    return result;
}

void PText::setFont(const std::string& dst_font)
{
    font = dst_font;
}

void PText::setColor(sf::Color c)
{
    if(color != c)
    {
        txt_color = "{color "+to_string(c.r)+" "+to_string(c.g)+" "+to_string(c.b)+"}";
        color = c;

        forceColor = true;
    }
}

void PText::setCharacterSize(int newCS)
{
    characterSize = newCS*3;
}

void PText::setTextQuality(int quality)
{
    qualitySetting = quality;
}

void PText::processRichText()
{
    /* 
    Patafour's Rich Text implementation for better dialogues:

    # text style
    {outline 2 0-255 0-255 0-255} = sets text outline to 2 pixel thick, r g b color
    {color 0-255 0-255 0-255} = changes color to r g b values
    {regular} = disables bold/italic/underlined/striked
    {bold} = sets text to be bold
    {italic} = sets text to be italic
    {underline} = sets text to be underlined
    {strike} = sets text to be striked (crossed out)

    # text behavior
    {speed 10} = sets text speed to 10 letters per sec
    {wait 500} = waits for 500 milliseconds before putting next characters

    # animations
    {wave h 4} = sets the text to be wavy, horizontally, with wave power = 4
    {wave v 4} = sets the text to be wavy, vertically, with wave power = 4
    {wave hv 4} = sets the text to be wavy, both horizontally and vertically, with wave power = 4

    # technical
    {global 1} = displays global text with id 1
    {n} = new line

    # dialogue
    {goback} = lets the message cloud know to revert to the first text in dialogue (for patapolis npc's to return to their greeting)

    # single-use at the beginning
    {font Name.ttf} = will try to load Name.ttf from game resources/or mod resources and change the font of the text to it
    {fadein 1000} = text will start at 0% opacity and fade to 100% opacity during 1000 milliseconds. !!!will collide with {speech}!!!
    {size 16} = sets text size to 16. single-use due to RichText limitations (possibly will be expanded in future)
    {speech} = makes the text appear letter by letter, use if you dont want the text to appear instantly.

    # notes
    all letters will have their respective clock, aka their timeout
    when timeout is passed, next letter can appear

    */

    StringRepository* strRepo = CoreManager::getInstance().getStrRepo();

    //restart settings
    char_shown = 0;
    char_speed = 20;
    char_delay = 50;
    char_wait_period = 0;
    current_line = 0;
    char_buffer = 0;
    speech = false;
    fadein = false;
    fadein_length = 0;
    speech_done = false;
    goback = false;

    if(disable_processing)
    {
        t = sfe::RichText(*strRepo->fontStore[font]);
        t.setCharacterSize(characterSize);
        t << sf::Color(0,0,0,255);
        if(txt_color != "")
            t << color;
        t << txt;
        return;
    }

    t = sfe::RichText(*strRepo->fontStore[font]);
    t << sf::Color(0,0,0,0);
    if(txt_color != "")
        t << color;

    
    rt_string.clear();
    textSettings.clear();

    int i=0;
    int seek = 0;

    int br_open = -1;
    int br_close = -1;

    sf::String buffer;

    sf::String newtxt = txt;

    for(auto a:additional)
    {
        if(a.find("key~") != std::string::npos)
        {
            newtxt += Func::ConvertToUtf8String(strRepo->GetString(a.substring(a.find("~")+1)));
        } else
        {
            newtxt += a;
        }
    }

    for(auto x : txt)
    {
        if(seek == 0)
        {
            if(x == '{')
            {
                br_open = i;
                seek = 1;

                ++i;

                if(buffer.getSize() > 0)
                    rt_string.push_back(buffer);
                
                buffer.clear();
                continue;
            }
        }
        else if(seek == 1)
        {
            if(x == '}')
            {
                br_close = i;
                seek = 0;

                int br_size = br_close - br_open + 1;
                sf::String keyword = txt.substring(br_open, br_size);

                rt_string.push_back(keyword);
                buffer.clear();

                ++i;
                continue;
            }
        }

        buffer += x;
        ++i;
    }

    if(seek == 1)
    {
        SPDLOG_ERROR("Something went wrong while processing the string. Closing bracket not found.");
        return;
    }
    else
    {
        if(buffer.getSize() > 0)
            rt_string.push_back(buffer);
    }

    int currentStyle = 0;
    int processedChars = 0;

    for(auto x : rt_string)
    {
        if(x.find("{") != sf::String::InvalidPos)
        {
            //keyword
            sf::String keyword = x.substring(1, x.getSize()-2);
            SPDLOG_TRACE("Parsing keyword {}", std::string(keyword));

            std::vector<std::string> args = split(std::string(keyword), ' ');
            
            if(args.size() > 0)
            {
                SPDLOG_TRACE("Found keyword {}, {} arguments", args[0], args.size()-1);
                
                if(args[0] == "n")
                {
                    t << "\n";
                }
                else if(args[0] == "speech")
                {
                    if(!force_nonspeech)
                    speech = true;
                }
                else if(args[0] == "goback")
                {
                    goback = true;
                }
                else if(args[0] == "color")
                {
                    if(args.size()-1 == 3)
                    {
                        int r = atoi(args[1].c_str());
                        int g = atoi(args[2].c_str());
                        int b = atoi(args[3].c_str());

                        t << sf::Color(r,g,b,0);
                    }
                    else
                    {
                        SPDLOG_ERROR("Something went wrong while processing the string. Invalid number of arguments for keyword 'color'");
                        return;
                    }
                }
                else if(args[0] == "outline")
                {
                    if(args.size()-1 == 4)
                    {
                        int th = atoi(args[1].c_str());
                        int r = atoi(args[2].c_str());
                        int g = atoi(args[3].c_str());
                        int b = atoi(args[4].c_str());

                        sfe::Outline outline;
                        outline.outline = sf::Color(r,g,b,255);
                        outline.thickness = th*3;

                        t << outline;
                    }
                    else
                    {
                        SPDLOG_ERROR("Something went wrong while processing the string. Invalid number of arguments for keyword 'outline'");
                        return;
                    }
                }
                else if(args[0] == "bold")
                {
                    currentStyle += 1;
                    t << sf::Text::Style(currentStyle);
                }
                else if(args[0] == "italic")
                {
                    currentStyle += 2;
                    t << sf::Text::Style(currentStyle);
                }
                else if(args[0] == "strike")
                {
                    currentStyle += 8;
                    t << sf::Text::Style(currentStyle);
                }
                else if(args[0] == "underline")
                {
                    currentStyle += 4;
                    t << sf::Text::Style(currentStyle);
                }
                else if(args[0] == "regular")
                {
                    currentStyle = 0;
                    t << sf::Text::Style(currentStyle);
                }
                else if(args[0] == "speed")
                {
                    if(args.size()-1 == 1)
                    {
                        TextSettings setting;
                        setting.pos = processedChars;
                        setting.speed = atoi(args[1].c_str());
                        textSettings.push_back(setting);

                        SPDLOG_TRACE("Added text setting 'speed', pos: {}, value {}", setting.pos, setting.speed);
                    }
                    else
                    {
                        SPDLOG_ERROR("Something went wrong while processing the string. Invalid number of arguments for keyword 'speed'");
                        return;
                    }
                }
                else if(args[0] == "fadein")
                {
                    if(args.size()-1 == 1)
                    {
                        fadein_length = atoi(args[1].c_str());
                        fadein = true;

                        SPDLOG_TRACE("Added text setting 'fadein', length {}ms", fadein_length);
                    }
                    else
                    {
                        SPDLOG_ERROR("Something went wrong while processing the string. Invalid number of arguments for keyword 'fadein'");
                        return;
                    }
                }
                else if(args[0] == "wait")
                {
                    if(args.size()-1 == 1)
                    {
                        TextSettings setting;
                        setting.pos = processedChars;
                        setting.timeout = atoi(args[1].c_str());
                        textSettings.push_back(setting);

                        SPDLOG_TRACE("Added text setting 'wait', pos: {}, value {}", setting.pos, setting.timeout);
                    }
                    else
                    {
                        SPDLOG_ERROR("Something went wrong while processing the string. Invalid number of arguments for keyword 'wait'");
                        return;
                    }
                }
                else if(args[0] == "global")
                {
                    if(args.size()-1 == 1)
                    {
                        int key = atoi(args[1].c_str());
                        std::string out = "";

                        auto globals = CoreManager::getInstance().getGlobals();

                        if(globals->getType(key) == typeid(int))
                        {
                            out = to_string(globals->get<int>(key));
                        } else if(globals->getType(key) == typeid(float))
                        {
                            out = to_string(globals->get<float>(key));
                        } else if(globals->getType(key) == typeid(double))
                        {
                            out = to_string(globals->get<double>(key));
                        } else if(globals->getType(key) == typeid(std::string))
                        {
                            out = globals->get<std::string>(key);
                        } else
                        {
                            SPDLOG_WARN("Unknown type spotted when trying to process global {}", key);
                        }

                        t << out;
                    }
                    else
                    {
                        SPDLOG_ERROR("Something went wrong while processing the string. Invalid number of arguments for keyword 'global'");
                        return;
                    }
                }
                else
                {
                    SPDLOG_ERROR("Something went wrong while processing the string. Unknown keyword found: {}", args[0]);
                    return;
                }
            }
            else
            {
                SPDLOG_ERROR("Something went wrong while processing the string. Invalid keyword found: {}", std::string(keyword));
                return;
            }
        }
        else
        {
            //regular text
            t << x;
            processedChars += x.getSize();
        }
    }

    if(!speech && !fadein)
    {
        int lines = t.getLines().size();

        for(int x=0; x<lines; x++)
        {
            int len = t.getLines()[x].getLength();
            
            for(int y=0; y<len; y++)
            {
                sf::Color c = t.getCharacterColor(x, y);
                if (c.a != 255)
                {
                    c.a = 255;
                    t.setCharacterColor(x, y, c);
                }
            }
        }

        //we put it here, in case we want to have some dialogues instantly show up
        speech_done = true;
    }

    if (characterSize != t.getCharacterSize())
        t.setCharacterSize(characterSize);
}

void PText::setOrigin(float x, float y)
{
    orX = x;
    orY = y;
}

sf::Vector2f PText::getScale()
{
    return sf::Vector2f(scaleX, scaleY);
}

void PText::setScale(float x, float y)
{
    scaleX = x;
    scaleY = y;
}

void PText::setRotation(float a)
{
    angle = a;
}

/*
void PText::setColor(sf::Color color)
{
    std::vector<sfe::RichText::Line> lines = t.getLines();
    for(int x=0; x<lines.size(); x++)
    {
        int len = lines[x].getLength();
        
        for(int y=0; y<len; y++)
        {
            t.setCharacterColor(x, y, color);
        }
    }
}
*/

void PText::setOutlineColor(sf::Color color)
{
    //t.setOutlineColor(color);
}

void PText::setOutlineThickness(int thick)
{
    //t.setOutlineThickness(thick);
}

void PText::setPosition(float x, float y)
{
    //s.setPosition(x*ratioX,y*ratioY);
    lx = x;
    ly = y;

    //std::cout << x << " " << y << "  " << lx << " " << ly << std::endl;
}

void PText::setStringKey(std::string text_key)
{
    if(currentKey != text_key)
    {
        currentKey = text_key;
    }
}

void PText::setString(std::string text_string)
{
    // check if text is not the same, otherwise we dont have to process it again
    if(txt != sf::String(text_string))
    {
        txt = sf::String(text_string);
    }
}

void PText::setString(sf::String text_string)
{
    // check if text is not the same, otherwise we dont have to process it again
    if(txt != text_string)
    {
        txt = text_string;
    }
}

void PText::setString(const char* text_string)
{
    // check if text is not the same, otherwise we dont have to process it again
    if(txt != sf::String(text_string))
    {
        txt = sf::String(text_string);
    }
}

sf::Vector2f PText::getPosition()
{
    return sf::Vector2f(lx, ly);
}

void PText::setScale(float s)
{
    scaleX = s;
    scaleY = s;
}

sf::FloatRect PText::getLocalBounds()
{
    t.setFont(*CoreManager::getInstance().getStrRepo()->fontStore[font]);
    t.setCharacterSize(characterSize);
    //t.setFillColor(c);
    //t.setString(txt);
    return t.getLocalBounds();
}

sf::FloatRect PText::getGlobalBounds()
{
    t.setFont(*CoreManager::getInstance().getStrRepo()->fontStore[font]);
    t.setCharacterSize(characterSize);
    //t.setFillColor(c);
    //t.setString(txt);
    return t.getGlobalBounds();
}

sf::FloatRect PText::getTransformedBounds()
{
    //refer to spritewrapper

    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    sf::Vector2u windowSize = window->getSize();

    std::vector<float> x = {640, 1280, 1920, 3840};
    std::vector<float> y = {360, 720, 1080, 2160};

    sf::Vector2f resRatio(windowSize.x / float(3840), windowSize.y / float(2160));

    int quality = ResourceManager::getInstance().getCurrentQuality();

    sf::Vector2f ratio(windowSize.x / x[quality], windowSize.y / y[quality]);
    sf::Vector2f r((fabs(scaleX) / resRatio.x), (fabs(scaleY) / resRatio.y));

    sf::FloatRect l_bounds = getLocalBounds();

    return sf::FloatRect(l_bounds.top * r.x, l_bounds.left * r.y, l_bounds.width * r.x, l_bounds.height * r.y);
}

sf::FloatRect PText::getGlobalBoundsScaled()
{
    float nw = 1;
    float nh = 1;

    t.setFont(*CoreManager::getInstance().getStrRepo()->fontStore[font]);
    if (t.getGlobalBounds().width > 0)
        nw = t.getGlobalBounds().width / resRatioX;

    if (t.getGlobalBounds().height > 0)
        nh = t.getGlobalBounds().height / resRatioY;

    return sf::FloatRect(t.getGlobalBounds().left, t.getGlobalBounds().top, nw, nh);
    //return t.getGlobalBounds();
}

void PText::draw(bool noRender)
{
    StringRepository* strRepo = CoreManager::getInstance().getStrRepo();
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();

    t.setCharacterSize(characterSize);

    if(currentKey != "")
    {
        if(currentKey == "no message")
            setString("no message");
        else
            setString(Func::ConvertToUtf8String(strRepo->GetString(currentKey)));
    }

    if(oldtxt != txt || oldKey != currentKey || oldColor != color)
    {
        processRichText();
    }



    switch (qualitySetting)
    {
        case 0: ///low
        {
            ratioX = window->getSize().x / float(640);
            ratioY = window->getSize().y / float(360);
            break;
        }

        case 1: ///med
        {
            ratioX = window->getSize().x / float(1280);
            ratioY = window->getSize().y / float(720);
            break;
        }

        case 2: ///high
        {
            ratioX = window->getSize().x / float(1920);
            ratioY = window->getSize().y / float(1080);
            break;
        }

        case 3: ///ultra
        {
            ratioX = window->getSize().x / float(3840);
            ratioY = window->getSize().y / float(2160);
            break;
        }
    }

    resRatioX = window->getSize().x / float(3840);
    resRatioY = window->getSize().y / float(2160);

    t.setFont(*CoreManager::getInstance().getStrRepo()->fontStore[font]);
    t.setCharacterSize(characterSize);
    t.setScale(resRatioX * scaleX, resRatioY * scaleY);
    t.setOrigin(orX, orY);
    t.setPosition(lx * resRatioX, ly * resRatioY);
    t.setRotation(angle * (180 / 3.14159265358));

    if(fadein)
    {
        fadein = false;
        fadein_set = true;
        fadein_clock.restart();
    }

    if(fadein_set)
    {
        if(fadein_clock.getElapsedTime().asMilliseconds() < fadein_length)
        {
            int lines = t.getLines().size();
            float alpha = float(fadein_clock.getElapsedTime().asMilliseconds()) / float(fadein_length) * 255.f;

            if(alpha < 0)
                alpha = 0;

            if(alpha >= 255)
                alpha = 255;

            for(int x=0; x<lines; x++)
            {
                int len = t.getLines()[x].getLength();

                for(int y=0; y<len; y++)
                {
                    sf::Color c = t.getCharacterColor(x, y);
                    if (c.a != alpha)
                    {
                        c.a = alpha;
                        t.setCharacterColor(x, y, c);
                    }
                }
            }
        }
    }

    if(speech)
    {
        for(unsigned int i=0; i<textSettings.size(); i++)
        {
            if(char_shown == textSettings[i].pos)
            {
                SPDLOG_TRACE("Processing setting pos {} speed {} timeout {}", textSettings[i].pos, textSettings[i].speed, textSettings[i].timeout);

                if(textSettings[i].speed > 0)
                {
                    char_speed = textSettings[i].speed;
                    char_delay = 1000.f / char_speed;
                }

                if(textSettings[i].timeout > 0)
                {
                    char_wait_period = textSettings[i].timeout;
                    char_wait.restart();
                }

                textSettings.erase(textSettings.begin() + i);
                break;
            }
        }

        if((char_timeout.getElapsedTime().asMilliseconds() >= char_delay && char_wait.getElapsedTime().asMilliseconds() > char_wait_period) || speedup)
        {
            // remove timeout if any
            char_wait_period = 0;
            int all_lines = t.getLines().size();

            if(current_line != all_lines)
                SPDLOG_TRACE("current_line: {}, all_lines: {}", current_line, all_lines);

            if(current_line < all_lines)
            {
                int len = t.getLines()[current_line].getLength();
                SPDLOG_TRACE("line length: {}, char_buffer: {}", len, char_buffer);

                if(char_buffer >= len)
                {
                    SPDLOG_TRACE("Advance to the next line");
                    char_buffer -= len;
                    current_line += 1;
                }
                else
                {
                    SPDLOG_TRACE("char shown {} char buffer {} len {}", char_shown, char_buffer, len);

                    sf::Color c = t.getCharacterColor(current_line, char_buffer);
                    c.a = 255;
                    t.setCharacterColor(current_line, char_buffer, c);

                    char_buffer += 1;
                    char_shown += 1;
                }
            }

            if(current_line == all_lines)
            {
                SPDLOG_TRACE("Text completed");
                speech = false;
                speech_done = true;
            }

            char_timeout.restart();
        }
    }

    if (rendered && !noRender)
        window->draw(t);

    if (!rendered)
        rendered = true;

    oldtxt = txt;
    oldKey = currentKey;
    oldColor = color;
}

sf::Text PText::getText()
{
    return sf::Text(sf::String("unknown text"), *CoreManager::getInstance().getStrRepo()->fontStore[font], 30);
    //return t;
}

void PText::addText(sf::String add_text)
{
    additional.push_back(add_text);
}