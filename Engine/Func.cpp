#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "Func.h"
#include <codecvt>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <spdlog/spdlog.h>

#include "CoreManager.h"

namespace fs = std::filesystem;

std::vector<std::string> Func::Split(const std::string& s, char delim)
{
    std::vector<std::string> elems;

    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }

    return elems;
}

sf::String Func::ConvertToUtf8String(const std::string& s)
{
    return sf::String::fromUtf8(s.begin(), s.end());
}
std::string Func::trim(const std::string& str, const std::string& whitespace = " \t")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

std::string Func::wrap_text(std::string input, int box_width, std::string font, int character_size)
{
    //cout << "wrap_text(" << input << ", " << box_width << ")" << endl;

    if (input.size() <= 0)
        return "";

    std::string temp = "";
    std::vector<std::string> words;

    for (unsigned long i = 0; i < input.size(); i++)
    {
        if ((input[i] == ' ') || (input[i] == '\n') || (i == input.size() - 1))
        {
            if (i == input.size() - 1)
                temp += input[i];

            if (input[i] == '\n')
                temp += input[i];

            //cout << "Registered word: " << temp << endl;
            words.push_back(temp);
            temp = "";
        } else
        {
            temp += input[i];
        }
    }

    std::string full = "";

    std::string prevtemp = "";
    int wordcount = 0;
    temp = "";

    for (unsigned long i = 0; i < words.size(); i++)
    {
        prevtemp = temp;

        if (words[i].find("\n") != std::string::npos)
        {
            //cout << "String found to contain a new line character" << endl;
            std::string prefull = prevtemp + " " + words[i];

            //TODO: check if it wraps text correctly
            sf::Text t_temp;
            t_temp.setFont(CoreManager::getInstance().getStrRepo()->GetFontFromName(font));
            t_temp.setCharacterSize(character_size);
            t_temp.setString(prefull);

            if (t_temp.getGlobalBounds().width >= box_width)
            {
                full += prevtemp + '\n';
                i--;
            } else
            {
                full += prefull;
            }

            SPDLOG_TRACE("Added \"{}\" to the output", prevtemp + words[i]);

            i++;
            temp = "";
            wordcount = 0;
        }

        if (wordcount > 0)
            temp += " ";

        temp += words[i];
        wordcount++;

        sf::Text t_temp;
        t_temp.setFont(CoreManager::getInstance().getStrRepo()->GetFontFromName(font));
        t_temp.setCharacterSize(character_size);
        t_temp.setString(temp);

        //cout << "Testing string \"" << temp << "\", " << wordcount << " words, size: " << t_temp.getGlobalBounds().width << endl;

        if (t_temp.getGlobalBounds().width >= box_width)
        {
            if (wordcount > 1)
            {
                //cout << "String exceeded the max box width (" << box_width << " vs " << t_temp.getGlobalBounds().width << ")" << endl;
                full += prevtemp;
                full += '\n';

                //cout << "Added \"" << prevtemp << "\" to the output" << endl;

                i -= 1;
                temp = "";
                wordcount = 0;
            } else
            {
                std::string ltemp = "";

                ///if its just a long ass word
                ///need to optimize this because it doesnt work correctly for chinese
                for (unsigned long e = 0; e < temp.size(); e++)
                {
                    ltemp += temp[e];

                    sf::Text t_ltemp;
                    t_ltemp.setFont(CoreManager::getInstance().getStrRepo()->GetFontFromName(font));
                    t_ltemp.setCharacterSize(character_size);
                    t_ltemp.setString(ltemp);

                    if (t_ltemp.getGlobalBounds().width >= box_width - 30)
                    {
                        full += ltemp;
                        full += "-";
                        full += '\n';

                        ltemp = "";
                    }
                }

                ///adding remains
                temp = ltemp;
                wordcount = 1;
            }
        }
    }

    //cout << "End of word list, adding remains to the full string" << endl;
    full += temp;

    return full;
}

template<typename T>
std::string Func::to_str(const T& t)
{
    std::ostringstream ss;
    ss << t;
    return ss.str();
}

std::string Func::num_padding(int num, int padding) ///stolen from cplusplus.com
{
    std::ostringstream ss;
    ss << std::setw(padding) << std::setfill('0') << num;

    return ss.str();
}

std::string Func::getCurrentWorkingDir()
{
    std::filesystem::path cwd = std::filesystem::current_path();
    return cwd.string();
}

void Func::create_directory(const std::string& path) {
    std::filesystem::path dir(path);

    try {
        if (std::filesystem::create_directories(dir)) {
            SPDLOG_INFO("Directory created: {}", dir.string());
        } else {
            SPDLOG_WARN("Directory already exists: {}", dir.string());
        }
    } catch (const std::filesystem::filesystem_error& e) {
        SPDLOG_ERROR("Error creating directory", std::string(e.what()));
    }
}

std::string Func::getTempDirectory() {
#ifdef _WIN32
    // For Windows
    char tempPath[MAX_PATH];
    if (GetTempPathA(MAX_PATH, tempPath)) {
        return std::string(tempPath);
    }
#elif defined(__linux__) || defined(__APPLE__)
    // For Linux and macOS
    const char* tempPath = std::getenv("TMPDIR");
    if (tempPath) {
        return std::string(tempPath);
    }
    return "/tmp"; // Default for Linux
#endif
    return ""; // Fallback if no temp directory found
}

bool Func::replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void Func::smoothTransition(float& current, float& destination, float& delta)
{
    if(fabs(current - destination) > 0)
    {
        if(current > destination)
        {
            current -= fabs(current - destination) * delta;
        }
        else
        {
            current += fabs(current - destination) * delta;
        }
    }
}

// Function to convert hex string to sf::Color
sf::Color Func::hexToColor(const std::string& hex) {
    unsigned int r, g, b;

    // Remove the '#' if present
    std::string hexColor = hex[0] == '#' ? hex.substr(1) : hex;

    // Use stringstream to convert hex to integers
    std::stringstream ss;
    ss << std::hex << hexColor;

    // Extract the red, green, and blue components from the hex string
    ss >> r >> g >> b;

    // Return the corresponding SFML color (alpha is set to 255 by default)
    return sf::Color(static_cast<sf::Uint8>(r),
                     static_cast<sf::Uint8>(g),
                     static_cast<sf::Uint8>(b));
}