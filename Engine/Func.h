#ifndef FUNC_H
#define FUNC_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <optional>
#include <nlohmann/json.hpp>
#include <random>
#include <Mission/Units/Entity.h>

class Func
{
public:
    Func();
    static std::vector<std::string> Split(const std::string& s, char delim);
    static std::vector<std::string> Split(const std::string& s, wchar_t delim);
    static sf::String ConvertToUtf8String(const std::string& s);
    static std::string trim(const std::string& str, const std::string& whitespace);
    static std::string wrap_text(std::string input, int box_width, std::string font, int character_size);
    //static sf::String wrap_text(std::string input, int box_width, sf::Font font, int character_size);
    static void create_directory(const std::string& path);
    static std::string getTempDirectory();
    static bool replace(std::string& str, const std::string& from, const std::string& to);
    static void smoothTransition(float& current, float& destination, float delta);
    static std::string getCurrentWorkingDir();

    template<typename T>
    static std::string to_str(const T& t);
    static std::string num_padding(int num, int padding);
    static std::string fnum_padding(float num, int padding);
    static sf::Color hexToColor(const std::string& hex);
    static nlohmann::json parseLootArray(std::mt19937& gen, std::uniform_real_distribution<double>& roll, nlohmann::json loot);
    static void parseEntityLoot(std::mt19937& gen, std::uniform_real_distribution<double>& roll, nlohmann::json loot, std::vector<Entity::Loot>& to_drop);

    static unsigned int calculateImageChecksum(const sf::Image& image);
    static unsigned int calculateTotalChecksum(const std::vector<std::string>& filePaths, libzippp::ZipArchive& zf);
};

#endif // FUNC_H
