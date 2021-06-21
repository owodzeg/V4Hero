#ifndef STRINGREPOSITORY_H
#define STRINGREPOSITORY_H
#include "DebugOut.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
class StringRepository
{
public:
    std::vector<std::string> langFiles;
    std::vector<std::string> langNames;
    std::vector<std::string> langFonts;
    std::map<std::string, std::wstring> stringMap;
    int configDebugID = 0;
    StringRepository();
    void LoadLanguageFiles(int langNum);
    void LoadLanguageFile(std::wifstream* conf);
    ///I wanted to overload these two in a single function, but somehow it just doesnt want to work...
    ///Will change it into one someday
    std::wstring GetUnicodeString(std::wstring key);
    std::wstring GetUnicodeString(std::string key);
    std::string GetString(std::wstring key);
    std::string GetString(std::string key);
};

#endif // STRINGREPOSITORY_H
