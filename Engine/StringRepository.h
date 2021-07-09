#ifndef STRINGREPOSITORY_H
#define STRINGREPOSITORY_H
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
class StringRepository
{
public:
    std::vector<int> langIDs;
    std::vector<std::string> langNames;
    std::vector<std::string> langFonts;
    int configDebugID = 0;
    StringRepository();
    void LoadLanguageFiles(int langNum);
    std::string GetString(std::string key);
private:
    std::map<std::string, std::string> stringMap;
    std::vector<std::string> langFiles;

    void LoadLanguageFile(std::ifstream* conf);
};

#endif // STRINGREPOSITORY_H
