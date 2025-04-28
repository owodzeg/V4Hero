#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "PataText.h"
#include <spdlog/spdlog.h>

#include "../CoreManager.h"

std::vector<sf::String> PataText::split_and_remove(const sf::String& s, const sf::String& delim)
{
    if (s.isEmpty())
    {
        return {};
    }

    const size_t delimSize = delim.getSize();
    if (delimSize == 0)
    {
        return {s};
    }

    std::vector<sf::String> result;
    size_t start = 0;
    size_t end = 0;

    while ((end = s.find(delim, start)) != sf::String::InvalidPos)
    {
        result.emplace_back(s.substring(start, end - start));
        start = end + delimSize;
    }

    result.emplace_back(s.substring(start));

    return result;
}

std::vector<std::string> PataText::split_fast(const std::string& s, char delim)
{
    std::vector<std::string> result;
    result.reserve(std::count(s.begin(), s.end(), delim) + 1); // Pre-allocate for best case

    size_t start = 0;
    size_t end = 0;

    while ((end = s.find(delim, start)) != std::string::npos)
    {
        if (end > start)
            result.emplace_back(s.substr(start, end - start));
        start = end + 1;
    }

    if (start < s.length())
        result.emplace_back(s.substr(start));

    return result;
}


// function for splitting sf::String by another sf::String, while KEEPING the delimeter
// Example: split_and_keep("{bold}{color 255 255 255}Hello{bold}", "{")
// Should return:
// {"{bold}", "{color 255 255 255}", "Hello", "{bold}"};

std::vector<sf::String> PataText::extract_tokens(const sf::String& str)
{
    std::vector<sf::String> result;

    size_t braceCount = 0;
    for (size_t i = 0; i < str.getSize(); ++i)
    {
        if (str[i] == '{')
            braceCount++;
    }
    result.reserve(braceCount * 2 + 1);

    std::size_t pos = 0;
    std::size_t prev = 0;
    const sf::String openBrace = '{';
    const sf::String closeBrace = '}';

    while (pos < str.getSize())
    {
        pos = str.find(openBrace, prev);

        if (pos != sf::String::InvalidPos)
        {
            if (pos > prev)
            {
                result.emplace_back(str.substring(prev, pos - prev));
            }

            std::size_t endPos = str.find(closeBrace, pos);
            if (endPos != sf::String::InvalidPos)
            {
                result.emplace_back(str.substring(pos, endPos - pos + 1));
                prev = endPos + 1;
            } else
            {
                result.emplace_back(str.substring(pos));
                return result;
            }
        } else
        {
            break;
        }
    }

    if (prev < str.getSize())
    {
        result.emplace_back(str.substring(prev));
    }

    return result;
}

PataText::PataText()
{
    default_style.bold = false;
    default_style.italic = false;
    default_style.underline = false;
    default_style.strike = false;
    default_style.additional_kerning = 0;
    default_style.x_offset = 0;
    default_style.y_offset = 0;
    default_style.c_red = 0;
    default_style.c_green = 0;
    default_style.c_blue = 0;
    default_style.c_alpha = 255;
    default_style.char_size = 72;
    default_style.shake = 0;
    default_style.hwave = 0;
    default_style.vwave = 0;
    default_style.ot_c_red = 0;
    default_style.ot_c_green = 0;
    default_style.ot_c_blue = 0;
    default_style.ot_c_alpha = 255;
    default_style.thickness = 0;

    styleResetAllStyles(m_marker);
    m_lines.push_back(std::vector<PTChar>());
}

PataText::~PataText()
{
    SPDLOG_DEBUG("Destroying PataText instance");
    m_lines.clear();
    SPDLOG_DEBUG("Destroying PataText instance.. completed");
}

void PataText::applyFadeIn()
{
}

void PataText::applyDialog()
{
}


// affects style
void PataText::styleSetAdditionalKerning(PTStyle& style, double newKerning)
{
    style.additional_kerning = newKerning;
}

void PataText::styleSetXOffset(PTStyle& style, double newXOffset)
{
    style.x_offset = newXOffset;
}

void PataText::styleSetYOffset(PTStyle& style, double newYOffset)
{
    style.y_offset = newYOffset;
}

void PataText::styleSetFullColor(PTStyle& style, sf::Color newColor)
{
    style.c_red = newColor.r;
    style.c_green = newColor.g;
    style.c_blue = newColor.b;
}

void PataText::styleSetRedColor(PTStyle& style, double newRed)
{
    style.c_red = newRed;
}

void PataText::styleSetGreenColor(PTStyle& style, double newGreen)
{
    style.c_green = newGreen;
}

void PataText::styleSetBlueColor(PTStyle& style, double newBlue)
{
    style.c_blue = newBlue;
}

void PataText::styleSetAlpha(PTStyle& style, double newAlpha)
{
    style.c_alpha = newAlpha;
}

void PataText::styleSetCharSize(PTStyle& style, double newCharSize)
{
    style.char_size = newCharSize;
}

void PataText::styleSetFont(PTStyle& style, const std::string& fontStr)
{
    style.fontStr = fontStr;
    style.font = CoreManager::getInstance().getStrRepo()->fontStore[style.fontStr];
}

void PataText::styleSetBold(PTStyle& style, bool isBold)
{
    style.bold = isBold;
}

void PataText::styleSetItalic(PTStyle& style, bool isItalic)
{
    style.italic = isItalic;
}

void PataText::styleSetUnderline(PTStyle& style, bool isUnderline)
{
    style.underline = isUnderline;
}

void PataText::styleSetStrike(PTStyle& style, bool isStrike)
{
    style.strike = isStrike;
}

void PataText::styleToggleBold(PTStyle& style)
{
    style.bold = !style.bold;
}

void PataText::styleToggleItalic(PTStyle& style)
{
    style.italic = !style.italic;
}

void PataText::styleToggleUnderline(PTStyle& style)
{
    style.underline = !style.underline;
}

void PataText::styleToggleStrike(PTStyle& style)
{
    style.strike = !style.strike;
}

void PataText::styleSetShake(PTStyle& style, double newShake)
{
    style.shake = newShake;
}

void PataText::styleSetHorizontalWave(PTStyle& style, double newHWave, double newHWaveSpeed)
{
    style.hwave = newHWave;
    style.hwave_speed = newHWaveSpeed;
}

void PataText::styleSetVerticalWave(PTStyle& style, double newVWave, double newVWaveSpeed)
{
    style.vwave = newVWave;
    style.vwave_speed = newVWaveSpeed;
}

void PataText::styleSetOutlineFullColor(PTStyle& style, sf::Color newColor)
{
    style.ot_c_red = newColor.r;
    style.ot_c_green = newColor.g;
    style.ot_c_blue = newColor.b;
}

void PataText::styleSetOutlineRedColor(PTStyle& style, double newRed)
{
    style.ot_c_red = newRed;
}

void PataText::styleSetOutlineGreenColor(PTStyle& style, double newGreen)
{
    style.ot_c_green = newGreen;
}

void PataText::styleSetOutlineBlueColor(PTStyle& style, double newBlue)
{
    style.ot_c_blue = newBlue;
}

void PataText::styleSetOutlineAlpha(PTStyle& style, double newAlpha)
{
    style.ot_c_alpha = newAlpha;
}

void PataText::styleSetOutlineThickness(PTStyle& style, double newThickness)
{
    style.thickness = newThickness;
}

void PataText::styleSetSpeed(PTStyle& style, double newMsSpeed)
{
    style.nextCharTimeout = newMsSpeed;
}

void PataText::styleSetTimeout(PTStyle& style, double newMsTimeout)
{
    style.curCharTimeout = newMsTimeout;
}

void PataText::styleResetAllStyles(PTStyle& style)
{
    PTStyle def = default_style;
    style = def;

    auto strRepo = CoreManager::getInstance().getStrRepo();
    style.fontStr = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());
    style.font = strRepo->fontStore[style.fontStr];
}

void PataText::defaultStyleSetColor(sf::Color newColor)
{
    default_style.c_red = newColor.r;
    default_style.c_green = newColor.g;
    default_style.c_blue = newColor.b;
    default_style.c_alpha = newColor.a;
    m_marker = default_style;
}

void PataText::defaultStyleSetCharSize(double newCharSize)
{
    default_style.char_size = newCharSize;
    m_marker = default_style;
}

void PataText::defaultStyleSetFont(const std::string& fontStr)
{
    default_style.fontStr = fontStr;
    default_style.font = CoreManager::getInstance().getStrRepo()->fontStore[fontStr];
    m_marker = default_style;
}

void PataText::defaultStyleSetOutlineColor(sf::Color newColor)
{
    default_style.ot_c_red = newColor.r;
    default_style.ot_c_green = newColor.g;
    default_style.ot_c_blue = newColor.b;
    default_style.ot_c_alpha = newColor.a;
    m_marker = default_style;
}

void PataText::defaultStyleSetOutlineThickness(double newThickness)
{
    default_style.thickness = newThickness;
    m_marker = default_style;
}

void PataText::append(std::string input_text)
{
    sf::String sf_input_text(input_text);
    append(sf_input_text);
}

void PataText::append(const char* input_text)
{
    sf::String sf_input_text(input_text);
    append(sf_input_text);
}

void PataText::append(sf::String& input_text)
{
    // Step 1: Split the input text by lines - {n} tokens
    
    // Input:
    // "Hello.{n}I like cheese."
    // Output:
    // "Hello."
    // "I like cheese."

    std::vector<sf::String> lines = split_and_remove(input_text, "{n}");

    // Step 2: Go over each input line, and split the input text by style {} tokens
    
    // Input:
    // {bold}Hello{bold}{italic} world! I like cheese.{italic}
    
    // Output:
    // "{bold}"
    // "Hello"
    // "{bold}"
    // "{italic}"
    // " world! I like cheese."
    // "{italic}"

    for (auto line : lines)
    {
        if (lines.size() > 1)
            m_lines.push_back(std::vector<PTChar>());

        std::vector<sf::String> tokens = extract_tokens(line);

        // Step 3: Distinguish tokens and regular text.
        // If style token is found, apply the style to the marker.
        // If regular text is found, split it to PTChars and apply current marker style, then append PTChars to the appropriate m_lines item.

        for (auto token : tokens)
        {
            if (token[0] == '{')
            {
                // token is a style token
                ProcessStyleToken(token);
            } else
            {
                // token is a regular text
                ProcessRegularText(token);
            }
        }
    }

    // Step 4: Apply kerning from font data.
    applyDefaultKerning(); 

    // Step 5: Calculate positions before rendering
    ProcessPositioning();
    refreshPositioning = true;
}

void PataText::reset()
{
    m_lines.clear();
    styleResetAllStyles(m_marker);
    max_width = 0;
    max_height = 0;
    m_lines.push_back(std::vector<PTChar>());
}

void PataText::ProcessStyleToken(const sf::String& token)
{
    const sf::String keyword = token.substring(1, token.getSize() - 2);
    const std::string keywordStr = keyword.toAnsiString();
    
    SPDLOG_TRACE("Parsing keyword {}", keywordStr);

    if (keywordStr == "bold")
    {
        styleToggleBold(m_marker);
        SPDLOG_TRACE("Set bold style to {}", m_marker.bold);
    } 
    else if (keywordStr == "italic")
    {
        styleToggleItalic(m_marker);
        SPDLOG_TRACE("Set italic style to {}", m_marker.italic);
    } 
    else if (keywordStr == "underline")
    {
        styleToggleUnderline(m_marker);
        SPDLOG_TRACE("Set underline style to {}", m_marker.underline);
    } 
    else if (keywordStr == "strike")
    {
        styleToggleStrike(m_marker);
        SPDLOG_TRACE("Set strike style to {}", m_marker.strike);
    } 
    else if (keywordStr == "regular")
    {
        styleSetBold(m_marker, false);
        styleSetItalic(m_marker, false);
        styleSetUnderline(m_marker, false);
        styleSetStrike(m_marker, false);
        SPDLOG_TRACE("Set bold, italic, underline and strike to 0");
    } 
    else if (keywordStr.find("color") != std::string::npos)
    {
        // extract color values
        std::vector<std::string> args = split_fast(keywordStr, ' ');
        if (args.size() == 4)
        {
            int r = atoi(args[1].c_str());
            int g = atoi(args[2].c_str());
            int b = atoi(args[3].c_str());
            styleSetFullColor(m_marker, sf::Color(r, g, b));
            SPDLOG_TRACE("Set color to {}, {}, {}", r, g, b);
        } 
        else
        {
            SPDLOG_ERROR("Something went wrong while processing the string. Invalid number of arguments for keyword 'color'");
            return;
        }
    } 
    else if (keyword.find("outline") != std::string::npos)
    {
        // extract outline values
        std::vector<std::string> args = split_fast(keywordStr, ' ');
        if (args.size() == 5)
        {
            int th = atoi(args[1].c_str());
            int r = atoi(args[2].c_str());
            int g = atoi(args[3].c_str());
            int b = atoi(args[4].c_str());
            styleSetOutlineThickness(m_marker, th);
            styleSetOutlineFullColor(m_marker, sf::Color(r, g, b));
            SPDLOG_TRACE("Set outline thickness to {}, color to {}, {}, {}", th, r, g, b);
        } 
        else
        {
            SPDLOG_ERROR("Something went wrong while processing the string. Invalid number of arguments for keyword 'outline'");
            return;
        }
    } 
    else if (keyword.find("size") != std::string::npos)
    {
        // extract values
        std::vector<std::string> args = split_fast(keywordStr, ' ');
        if (args.size() == 2)
        {
            int size = atoi(args[1].c_str());
            styleSetCharSize(m_marker, size);
            SPDLOG_TRACE("Set char size to {}", size);
        } 
        else
        {
            SPDLOG_ERROR("Something went wrong while processing the string. Invalid number of arguments for keyword 'size'");
            return;
        }
    } else if (keyword.find("speed") != std::string::npos)
    {
        // Speed parameter is in "characters per second" so we need to convert it to milliseconds per character
        std::vector<std::string> args = split_fast(keywordStr, ' ');
        if (args.size() == 2)
        {
            double cps = atoi(args[1].c_str());
            double speed;

            if (cps == 0)
            {
                speed = 0;
            } else
            {
                speed = 1000 / cps;
            }
            
            styleSetSpeed(m_marker, speed);
            SPDLOG_TRACE("Set speed to {}", speed);
        } 
        else
        {
            SPDLOG_ERROR("Something went wrong while processing the string. Invalid number of arguments for keyword 'speed'");
            return;
        }
    } 
    else if (keyword.find("wait") != std::string::npos)
    {
        std::vector<std::string> args = split_fast(keywordStr, ' ');
        if (args.size() == 2)
        {
            double timeout = atoi(args[1].c_str());
            styleSetTimeout(m_marker, timeout);
            SPDLOG_TRACE("Set timeout to {}", timeout);
        } else
        {
            SPDLOG_ERROR("Something went wrong while processing the string. Invalid number of arguments for keyword 'wait'");
            return;
        }
    } 
    else if (keyword.find("font") != std::string::npos)
    {
        std::vector<std::string> args = split_fast(keywordStr, ' ');
        if (args.size() == 2)
        {
            std::string fontStr = args[1];
            styleSetFont(m_marker, fontStr);
            SPDLOG_TRACE("Set font to {}", fontStr);
        } else
        {
            SPDLOG_ERROR("Something went wrong while processing the string. Invalid number of arguments for keyword 'font'");
            return;
        }
    } else if (keyword.find("shake") != std::string::npos)
    {
        std::vector<std::string> args = split_fast(keywordStr, ' ');
        if (args.size() == 2)
        {
            double shake = atoi(args[1].c_str());
            styleSetShake(m_marker, shake);
            SPDLOG_TRACE("Set shake to {}", shake);
        }
    } else if (keyword.find("hwave") != std::string::npos)
    {
        std::vector<std::string> args = split_fast(keywordStr, ' ');
        if (args.size() == 3)
        {
            double hwave = atoi(args[1].c_str());
            double hwave_speed = atoi(args[2].c_str());
            styleSetHorizontalWave(m_marker, hwave, hwave_speed);
            SPDLOG_TRACE("Set horizontal wave to {} speed {}", hwave, hwave_speed);
        }
    } else if (keyword.find("vwave") != std::string::npos)
    {
        std::vector<std::string> args = split_fast(keywordStr, ' ');
        if (args.size() == 3)
        {
            double vwave = atoi(args[1].c_str());
            double vwave_speed = atoi(args[2].c_str());
            styleSetVerticalWave(m_marker, vwave, vwave_speed);
            SPDLOG_TRACE("Set vertical wave to {} speed {}", vwave, vwave_speed);
        }
    }
    else
    {
        SPDLOG_ERROR("Something went wrong while processing the string. Unknown keyword found: {}", std::string(keyword));
        return;
    }
}

void PataText::ProcessRegularText(const sf::String& token)
{
    size_t tokenSize = token.getSize();
    if (tokenSize > 0)
    {
        std::vector<PTChar>& currentLine = m_lines.back();
        currentLine.reserve(currentLine.size() + tokenSize);

        for (size_t i = 0; i < tokenSize; ++i)
        {
            // Reset single-use timeout
            m_marker.curCharTimeout = 0;
            PTChar newChar(token[i], m_marker);
            newChar.text = std::make_unique<sf::Text>(*newChar.style.font, token[i], newChar.style.char_size);
            currentLine.emplace_back(newChar);
        }
    }
}

void PataText::applyDefaultKerning()
{
    auto strRepo = CoreManager::getInstance().getStrRepo();

    for (auto& line : m_lines)
    {
        if (line.size() < 2)
            continue;

        for (auto it = line.begin(); it+1 != line.end(); ++it)
        {
            auto& currentCharacter = *it;
            auto& nextCharacter = *(it + 1);

            const std::string& fontStr = currentCharacter.style.fontStr;

            std::pair<char32_t, char32_t> p = {currentCharacter.character, nextCharacter.character};
            currentCharacter.style.kerning = strRepo->GetKerningForFont(fontStr, p, currentCharacter.style.char_size);
        }
    }
}

void PataText::ProcessPositioning()
{
    auto strRepo = CoreManager::getInstance().getStrRepo();

    m_position.x = 0;
    m_position.y = 0;

    for (auto& line : m_lines)
    {
        double largest_height = 0;
        double largest_charSize = 0;

        m_counter_line = 0;

        // x positioning
        for (auto& character : line)
        {
            character.position = m_position;
            const std::string& fontStr = character.style.fontStr;

            auto height = strRepo->GetHeightForFont(fontStr, character.character, character.style.char_size, character.style.bold);
            auto advance = strRepo->GetAdvanceForFont(fontStr, character.character, character.style.char_size, character.style.bold); // width

            m_position.x += advance + character.style.kerning + character.style.additional_kerning;
            largest_height = std::fmax(largest_height, height);
            largest_charSize = std::fmax(largest_charSize, character.style.char_size);

            character.style.offset = m_counter;
            character.style.line_offset = m_counter_line;

            m_counter++;
            m_counter_line++;
        }

        // y positioning (after gathering largest height, try to center the characters)
        for (auto& character : line)
        {
            const std::string& fontStr = character.style.fontStr;

            auto glyphHeight = strRepo->GetHeightForFont(fontStr, character.character, character.style.char_size, character.style.bold);
            character.position.y = m_position.y + (largest_charSize - character.style.char_size);
        }

        max_width = std::fmax(max_width, m_position.x);
        max_height = std::fmax(max_height, m_position.y + largest_charSize);

        m_position.y += largest_charSize + m_interline;
        m_position.x = 0;
    }

    refreshPositioning = false;
}

void PataText::setGlobalPosition(double new_x, double new_y)
{
    global_x = new_x;
    global_y = new_y;
}

sf::Vector2f PataText::getGlobalPosition()
{
    return sf::Vector2f(global_x, global_y);
}

void PataText::setGlobalOrigin(double new_x, double new_y)
{
    origin_x = new_x;
    origin_y = new_y;
}

sf::Vector2f PataText::getGlobalOrigin()
{
    return sf::Vector2f(origin_x, origin_y);
}

sf::FloatRect PataText::getGlobalBounds()
{
    return sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(max_width, max_height));
}

void PataText::draw()
{
    auto window = CoreManager::getInstance().getWindow();

    double windowScale = window->getSize().x / 3840.f;

    // Step 5: Apply position based on letter width and line height.
    if (refreshPositioning)
    {
        ProcessPositioning();

        // Step 6: For additional performance benefits, reapply the settings only once when needed.
        for (auto& line : m_lines)
        {
            for (auto& character : line)
            {
                if (character.text)
                {
                    character.text->setFont(*character.style.font);
                    character.text->setCharacterSize(character.style.char_size * windowScale);
                    character.text->setFillColor(sf::Color(character.style.c_red, character.style.c_green, character.style.c_blue, character.style.c_alpha));
                    character.text->setOutlineColor(sf::Color(character.style.ot_c_red, character.style.ot_c_green, character.style.ot_c_blue, character.style.ot_c_alpha));
                    character.text->setOutlineThickness(character.style.thickness * windowScale);
                    character.text->setStyle(character.style.bold * sf::Text::Bold + character.style.italic * sf::Text::Italic + character.style.underline * sf::Text::Underlined + character.style.strike * sf::Text::StrikeThrough);
                    character.text->setString(character.character);
                }
            }
        }

        dialogue_clock.restart();
    }

    double timeRequired = 0;

    for (auto& line : m_lines)
    {
        for (auto& character : line)
        {
            // effects! :D
            double total_x_offset = 0;
            double total_y_offset = 0;

            //character.style.y_offset = sin(effect.getElapsedTime().asSeconds() * 10 + character.style.line_offset) * 4;

            random = (rand() % 10000) / 1000.f;

            if (character.style.shake > 0)
            {
                total_x_offset += sin(effect.getElapsedTime().asSeconds() * 20 * character.style.shake + character.style.offset + random) * character.style.shake;
                total_y_offset += cos(effect.getElapsedTime().asSeconds() * 34 * character.style.shake + character.style.offset + random) * character.style.shake;
            }

            if (character.style.hwave > 0)
            {
                total_x_offset += sin(effect.getElapsedTime().asSeconds() * character.style.hwave_speed + character.style.line_offset) * character.style.hwave;
            }

            if (character.style.vwave > 0)
            {
                total_y_offset += cos(effect.getElapsedTime().asSeconds() * character.style.vwave_speed + character.style.line_offset) * character.style.vwave;
            }

            character.style.x_offset = total_x_offset;
            character.style.y_offset = total_y_offset;

            character.text->setRotation(sf::degrees(character.style.rotation + character.style.r_offset));

            double final_x = windowScale * (global_x + character.position.x - origin_x + character.style.x_offset);
            double final_y = windowScale * (global_y + character.position.y - origin_y + character.style.y_offset);

            character.text->setPosition(sf::Vector2f(final_x, final_y));

            timeRequired += character.style.curCharTimeout;
            if (timeRequired <= dialogue_clock.getElapsedTime().asMilliseconds())
                window->draw(*character.text);
            timeRequired += character.style.nextCharTimeout;
        }
    }
}
