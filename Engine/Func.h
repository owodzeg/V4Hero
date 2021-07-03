#ifndef FUNC_H
#define FUNC_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Func
{
public:
    Func();
    static std::vector<std::string> Split(const std::string& s, char delim);
    static std::vector<std::string> Split(const std::string& s, wchar_t delim);
    static sf::String ConvertToUtf8String(const std::string& s);
    static std::string trim(const std::string& str, const std::string& whitespace);
    static std::string wrap_text(std::string input, int box_width, sf::Font& font, int character_size);
    //static sf::String wrap_text(std::string input, int box_width, sf::Font font, int character_size);

    template<typename T>
    static std::string to_str(const T& t);
    static std::string num_padding(int num, int padding);
};

#endif // FUNC_H
