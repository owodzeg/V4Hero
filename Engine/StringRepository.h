#ifndef STRINGREPOSITORY_H
#define STRINGREPOSITORY_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include "DebugOut.h"
class StringRepository
{
    public:
    std::vector<std::wstring> stringKeys;
    std::map<std::wstring,std::wstring> stringMap;
    int configDebugID=0;
    StringRepository();
    void LoadLanguageFiles(int langNum);
    void LoadLanguageFile(std::wifstream* conf);
    ///I wanted to overload these two in a single function, but somehow it just doesnt want to work...
    ///Will change it into one someday
    std::wstring GetUnicodeString(std::wstring key);
    std::string GetString(std::wstring key);
};

#endif // STRINGREPOSITORY_H
