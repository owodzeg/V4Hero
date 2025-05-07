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
    static std::unordered_map<std::string, unsigned int> checksums;
    static std::mutex func_mutex;
    static PataText tmp_ptext;

    static std::vector<std::string> Split(const std::string& s, char delim);
    static sf::String ConvertToUtf8String(const std::string& s);
    static sf::String GetStrFromKey(const std::string& key);
    static std::string trim(const std::string& str, const std::string& whitespace);
    static float measure_text_width(PataText& measurer, const std::string& text, const std::string& font, float character_size);
    static std::string wrap_text(const std::string& input, int box_width, const std::string& font, float character_size);
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
    
    template<typename T = int>
    static T rand();

    template<typename T>
    static T rand_range(T min, T max);

    static std::mt19937& global_rng();
};

template<typename T>
T Func::rand()
{
    if constexpr (std::is_integral_v<T>)
    {
        std::uniform_int_distribution<T> dist(0, std::numeric_limits<T>::max());
        return dist(global_rng());
    } else if constexpr (std::is_floating_point_v<T>)
    {
        std::uniform_real_distribution<T> dist(0, 1);
        return dist(global_rng());
    }
}

template<typename T>
T Func::rand_range(T min, T max)
{
    if constexpr (std::is_integral_v<T>)
    {
        std::uniform_int_distribution<T> dist(min, max);
        return dist(global_rng());
    } else if constexpr (std::is_floating_point_v<T>)
    {
        std::uniform_real_distribution<T> dist(min, max);
        return dist(global_rng());
    }
}

#endif // FUNC_H
