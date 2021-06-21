#ifndef DEBUGOUT_H
#define DEBUGOUT_H

#include <string>
#include <vector>
class Config;
class DebugOut
{
public:
    Config* config;
    DebugOut();
    DebugOut(Config* currentConfig);
    void DebugMessage(const std::string& s);
    void RhythmnDebugMessage(const std::string& s);
    void ImportantDebugMessage(const std::string& s);
};

#endif // FUNC_H
