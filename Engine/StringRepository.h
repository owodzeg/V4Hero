#ifndef STRINGREPOSITORY_H
#define STRINGREPOSITORY_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>
#include "DebugOut.h"
class StringRepository
{
    public:
    std::vector<std::string> stringKeys;
    std::map<std::string,std::string> stringMap;
    int configDebugID=0;
    StringRepository();
    void LoadLanguageFiles();
    ///I wanted to overload these two in a single function, but somehow it just doesnt want to work...
    ///Will change it into one someday
    std::string GetString(std::string key);
};

#endif // STRINGREPOSITORY_H
