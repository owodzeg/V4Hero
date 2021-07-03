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
    std::map<std::string, std::string> stringMap;
    int configDebugID = 0;
    StringRepository();
    void LoadLanguageFiles(int langNum);
    std::string GetString(std::string key);
private:
    void LoadLanguageFile(std::ifstream* conf);
};

#endif // STRINGREPOSITORY_H
