#ifndef FUNC_H
#define FUNC_H

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class Func
{
    public:
    Func();
    static std::vector<std::string> Split(const std::string &s, char delim);
    static std::vector<std::wstring> Split(const std::wstring &s, wchar_t delim);
    static sf::String ConvertToUtf8String(const std::wstring &s);
    static sf::String ConvertToUtf8String(const std::string &s);
    template<typename T> static std::string to_str(const T& t);
};

#endif // FUNC_H
