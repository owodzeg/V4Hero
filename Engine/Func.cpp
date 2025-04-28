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

#ifdef _WIN32
#include <windows.h>
#endif

#include <spdlog/spdlog.h>

#include "CoreManager.h"

namespace fs = std::filesystem;

std::unordered_map<std::string, unsigned int> Func::checksums;
std::mutex Func::func_mutex;

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

sf::String Func::GetStrFromKey(const std::string& key)
{
    auto strRepo = CoreManager::getInstance().getStrRepo();
    return Func::ConvertToUtf8String(strRepo->GetString(key));
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

// Helper function to measure the width of a string using PataText
double Func::measure_text_width(PataText& measurer, const std::string& text, const std::string& font, int character_size)
{
    if (text.empty())
    {
        return 0.0f;
    }
    measurer.reset(); // Clear previous state
    measurer.defaultStyleSetFont(font);
    measurer.defaultStyleSetCharSize(character_size);
    measurer.append(text);
    return measurer.getGlobalBounds().size.x;
}

std::string Func::wrap_text(const std::string& input, int box_width, const std::string& font, int character_size)
{
    if (input.empty() || box_width <= 0)
    {
        return ""; // Nothing to wrap or no space
    }

    std::vector<std::string> words = Split(input, ' ');

    std::vector<std::string> output_lines;
    std::string current_line;

    PataText measurer;
    float space_width = measure_text_width(measurer, " ", font, character_size);
    float hyphen_width = measure_text_width(measurer, "-", font, character_size);
    float effective_box_width_for_hyphen = std::max(1.0f, box_width - hyphen_width);

    for (const auto& original_word : words)
    {
        std::string remaining_word = original_word;

        while (measure_text_width(measurer, remaining_word, font, character_size) > box_width)
        {
            if (!current_line.empty())
            {
                output_lines.push_back(current_line);
                current_line.clear();
            }

            int break_pos = 0;
            for (int j = 1; j <= remaining_word.length(); ++j)
            {
                std::string sub = remaining_word.substr(0, j);
                float sub_width = measure_text_width(measurer, sub, font, character_size);

                if (sub_width <= effective_box_width_for_hyphen)
                {
                    break_pos = j;
                } else
                {
                    break;
                }
            }

            if (break_pos == 0 && !remaining_word.empty())
            {
                break_pos = 1;
            }

            if (break_pos == 0)
            {
                break;
            }

            output_lines.push_back(remaining_word.substr(0, break_pos) + "-");
            remaining_word = remaining_word.substr(break_pos);

            if (remaining_word.empty())
            {
                break;
            }
        }

        if (!remaining_word.empty())
        {
            float remaining_word_width = measure_text_width(measurer, remaining_word, font, character_size);
            float current_line_width = measure_text_width(measurer, current_line, font, character_size);

            float width_with_space = current_line.empty() ? 0.0f : space_width;
            float potential_width = current_line_width + width_with_space + remaining_word_width;

            if (current_line.empty())
            {
                current_line = remaining_word;
            } else if (potential_width <= box_width)
            {
                current_line += " " + remaining_word;
            } else
            {
                output_lines.push_back(current_line);
                current_line = remaining_word;
            }
        }
    }

    if (!current_line.empty())
    {
        output_lines.push_back(current_line);
    }

    std::string final_output;
    if (!output_lines.empty())
    {
        final_output = output_lines[0];
        for (size_t i = 1; i < output_lines.size(); ++i)
        {
            final_output += "{n}" + output_lines[i];
        }
    }

    return final_output;
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
    {
        return std::string("");
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
    return sf::Color(static_cast<uint8_t>(r),
                     static_cast<uint8_t>(g),
                     static_cast<uint8_t>(b));
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
			SPDLOG_WARN("Undefined behavior detected while parsing loot: {} | (Element of array is neither an array nor an object)", loot.dump());
        }
    }

    if (total < 100)
    {
		SPDLOG_WARN("Undefined behavior detected while parsing loot: {} | (Total chances in array less than 100)", loot.dump());
    } else if (total > 100)
    {
		SPDLOG_WARN("Undefined behavior detected while parsing loot: {} | (Total chances in array more than 100)", loot.dump());
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
			SPDLOG_WARN("Undefined behavior detected while parsing loot: {} | (Element of array is neither an array nor an object)", loot.dump());
        }
    }
}


void Func::parseEntityLoot(std::mt19937& gen, std::uniform_real_distribution<double>& roll, json loot, std::vector<Entity::Loot>& to_drop)
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
        } catch (const std::exception& e)
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
            sf::Color pixelColor = image.getPixel({x, y});
            unsigned int pixelValue = (pixelColor.r << 24) | (pixelColor.g << 16) | (pixelColor.b << 8) | pixelColor.a;
            checksum ^= pixelValue;
        }
    }

    return checksum;
}

unsigned int Func::calculateTotalChecksum(const std::vector<std::string>& filePaths, libzippp::ZipArchive& zf) {
    std::lock_guard<std::mutex> guard(func_mutex);
    unsigned int totalChecksum = 0x12345678;

    if(zf.isOpen())
    {
        for(auto filePath : filePaths)
        {
            if (checksums.count(filePath) > 0)
            {
                totalChecksum ^= checksums[filePath];
                continue;
            }

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
            checksums[filePath] = fileChecksum;

            delete[] data;
        }
    }
    else
    {
        for (auto sFilePath : filePaths) {

            if (checksums.count(sFilePath) > 0)
            {
                totalChecksum ^= checksums[sFilePath];
                continue;
            }

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
                checksums[sFilePath] = fileChecksum;
            } else {
                std::cerr << "Skipping invalid or non-regular file: " << filePath << std::endl;
            }
        }
    }

    return totalChecksum;
}