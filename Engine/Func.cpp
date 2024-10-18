#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "Func.h"
#include <codecvt>
#include <fstream>
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
    StringRepository* strRepo = CoreManager::getInstance().getStrRepo();

    //cout << "wrap_text(" << input << ", " << box_width << ")" << endl;
    input = strRepo->GetString(input);

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
            PText t_temp;
            t_temp.setFont(strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage()));
            t_temp.setCharacterSize(character_size);
            t_temp.setString(prefull);
            t_temp.draw(true);

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

        PText t_temp;
        t_temp.setFont(strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage()));
        t_temp.setCharacterSize(character_size);
        t_temp.setString(temp);
        t_temp.draw(true);

        //cout << "Testing string \"" << temp << "\", " << wordcount << " words, size: " << t_temp.getGlobalBounds().width << endl;

        if (t_temp.getLocalBounds().width >= box_width)
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

                    PText t_ltemp;
                    t_ltemp.setFont(strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage()));
                    t_ltemp.setCharacterSize(character_size);
                    t_ltemp.setString(ltemp);
                    t_ltemp.draw(true);

                    if (t_ltemp.getLocalBounds().width >= box_width - 30)
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

std::string Func::fnum_padding(float value, int precision) {
    std::ostringstream stream;

    // If precision is 0, cast the value to an int to remove decimal part.
    if (precision == 0) {
        stream << static_cast<int>(value);
    } else {
        // Set the fixed format and the desired precision.
        stream << std::fixed << std::setprecision(precision) << value;
    }

    return stream.str();
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

void Func::smoothTransition(float& current, float& destination, float delta)
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
    std::string hexColor = (hex[0] == '#') ? hex.substr(1) : hex;

    // Ensure the string is 6 characters long (RGB)
    if (hexColor.length() != 6)
        throw std::invalid_argument("Hex color must be 6 characters long");

    // Convert the red, green, and blue components
    r = std::stoul(hexColor.substr(0, 2), nullptr, 16); // Red
    g = std::stoul(hexColor.substr(2, 2), nullptr, 16); // Green
    b = std::stoul(hexColor.substr(4, 2), nullptr, 16); // Blue

    // Return the corresponding SFML color (alpha is set to 255 by default)
    return sf::Color(static_cast<sf::Uint8>(r),
                     static_cast<sf::Uint8>(g),
                     static_cast<sf::Uint8>(b));
}

nlohmann::json Func::parseLootArray(std::mt19937& gen, std::uniform_real_distribution<double>& roll, nlohmann::json loot)
{
    SPDLOG_DEBUG("Parse loot array");

    if (!loot.is_array())
    {
        return loot;
    }

    int total = 0;
    for (int i = 1; i < loot.size(); i++) // Chances -> Intervals for second loop
    {
        SPDLOG_DEBUG("Loot loop #1 (i={})", i);
        if (loot[i].is_array())
        {
            loot[i][0] += total;
            total = loot[i][0];
        } else if (loot[i].is_object() && loot[i].size() == 2)
        {
            int tmp = loot[i]["chance"];
            loot[i].erase("chance");
            total += tmp;
            loot[i]["chance"] = total;
        } else
        {
			SPDLOG_WARN("Undefined behavior detected while parsing loot: {} | (Element of array is neither an array nor an object)", loot);
        }
    }

    if (total < 100)
    {
		SPDLOG_WARN("Undefined behavior detected while parsing loot: {} | (Total chances in array less than 100)", loot);
    } else if (total > 100)
    {
		SPDLOG_WARN("Undefined behavior detected while parsing loot: {} | (Total chances in array more than 100)", loot);
    }

    SPDLOG_DEBUG("Roll");
    float n = roll(gen);
    for (int i = 1; i < loot.size(); i++) // Because of the first loop, values are neatly sorted
    {
        SPDLOG_DEBUG("Loot loop #2 (i={})", i);
        if (loot[i].is_array())
        {
            SPDLOG_DEBUG("Is array");
            if (n <= float(loot[i][0]) / 100)
            {
                return parseLootArray(gen, roll, loot[i]);
                break;
            }
        } else if (loot[i].is_object() && loot[i].size() == 2)
        {
            SPDLOG_DEBUG("Is object and loot == 2");
            if (n <= float(loot[i]["chance"]) / 100)
            {
                return loot[i];
                break;
            }
        } else
        {
			SPDLOG_WARN("Undefined behavior detected while parsing loot: {} | (Element of array is neither an array nor an object)", loot);
        }
    }
}


void Func::parseEntityLoot(std::mt19937& gen, std::uniform_real_distribution<double>& roll, json loot, vector<Entity::Loot>& to_drop)
{
    SPDLOG_DEBUG("Start parsing entity loot: {}", loot.dump());

    if (loot.is_array())
    {
        SPDLOG_DEBUG("Is array");
        if (roll(gen) <= float(loot[0]) / 100)
        {
            Entity::Loot tmp;
            json parsedArray = parseLootArray(gen, roll, loot);
            tmp.order_id = CoreManager::getInstance().getSaveReader()->itemReg.getItemByName(parsedArray["item"])->order_id;
            to_drop.push_back(tmp);
        }
    } else if (loot.is_object() && loot.size() > 1)
    {
        SPDLOG_DEBUG("Is object and loot > 1");
        try
        {
            for (auto it = loot.begin(); it != loot.end(); ++it) // Assume it's an object containing two items to be parsed
            {
                parseEntityLoot(gen, roll, it.value(), to_drop);
            }
        } catch (const exception& e)
        {
            SPDLOG_DEBUG("Exception");
            if (roll(gen) <= float(loot["chance"]) / 100) // Assume it's the below else if
            {
                Entity::Loot tmp;
                tmp.order_id = CoreManager::getInstance().getSaveReader()->itemReg.getItemByName(loot["item"])->order_id;
                to_drop.push_back(tmp);
            }
        }
    } else if (loot.is_object() && loot.size() == 2)
    {
        SPDLOG_DEBUG("Is object with loot == 2");
        if (roll(gen) <= float(loot["chance"]) / 100)
        {
            Entity::Loot tmp;
            tmp.order_id = CoreManager::getInstance().getSaveReader()->itemReg.getItemByName(loot["item"])->order_id;
            to_drop.push_back(tmp);
        }
    }
}

unsigned int Func::calculateImageChecksum(const sf::Image& image) {
    unsigned int checksum = 0;

    sf::Vector2u imageSize = image.getSize();
    unsigned int width = imageSize.x;
    unsigned int height = imageSize.y;

    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            sf::Color pixelColor = image.getPixel(x, y);
            unsigned int pixelValue = (pixelColor.r << 24) | (pixelColor.g << 16) | (pixelColor.b << 8) | pixelColor.a;
            checksum ^= pixelValue;
        }
    }

    return checksum;
}

unsigned int Func::calculateTotalChecksum(const std::vector<std::string>& filePaths, libzippp::ZipArchive& zf) {
    unsigned int totalChecksum = 0x12345678;

    if(zf.isOpen())
    {
        for(auto filePath : filePaths)
        {
            char* data = (char*) zf.readEntry(filePath, true);
            unsigned int s = zf.getEntry(filePath).getSize();

            // XOR checksum for this file
            unsigned int fileChecksum = 0;
            size_t i = 0;

            // Process the data in 4-byte chunks (32-bit)
            for (; i + 4 <= s; i += 4) {
                // Treat each 4 bytes as a single 32-bit integer, and XOR it with the checksum
                unsigned int chunk =
                    (static_cast<unsigned char>(data[i]) << 24) |  // byte 1 (most significant byte)
                    (static_cast<unsigned char>(data[i + 1]) << 16) |  // byte 2
                    (static_cast<unsigned char>(data[i + 2]) << 8) |   // byte 3
                    static_cast<unsigned char>(data[i + 3]);           // byte 4 (least significant byte)
                fileChecksum ^= chunk;
            }

            // Handle any remaining bytes (if the file size isn't a multiple of 4)
            unsigned int lastBytes = 0;
            for (size_t j = 0; i < s; ++i, ++j) {
                lastBytes |= (static_cast<unsigned char>(data[i]) << (8 * (3 - j)));  // shift remaining bytes
            }
            fileChecksum ^= lastBytes;

            // Combine with the total checksum
            totalChecksum ^= fileChecksum;
        }
    }
    else
    {
        for (auto sFilePath : filePaths) {
            std::filesystem::path filePath = std::filesystem::path(sFilePath);
            if (std::filesystem::exists(filePath) && std::filesystem::is_regular_file(filePath)) {
                std::ifstream file(filePath, std::ios::binary);
                if (!file.is_open()) {
                    std::cerr << "Failed to open file: " << filePath << std::endl;
                    continue;  // Skip to next file if we can't open this one
                }

                unsigned int fileChecksum = 0x77777777;
                char byte;
                size_t byteCount = 0;  // To track how many bytes have been processed
                while (file.get(byte)) {
                    fileChecksum ^= (static_cast<unsigned int>(static_cast<unsigned char>(byte)) << ((byteCount % 4) * 8));
                    ++byteCount;
                }

                totalChecksum ^= fileChecksum;  // Combine each file's checksum
            } else {
                std::cerr << "Skipping invalid or non-regular file: " << filePath << std::endl;
            }
        }
    }

    return totalChecksum;
}