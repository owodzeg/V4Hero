#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "DebugOut.h"
#include "Config.h"
using namespace std;
DebugOut::DebugOut()
{

}
DebugOut::DebugOut(Config* currentConfig)
{
    config = currentConfig;
}
void DebugOut::DebugMessage(const std::string &s)
{
    if(config->GetInt("showDebugMsg")){
        cout<<"[DEBUG]: "<<s;
    }
}
void DebugOut::RhythmnDebugMessage(const std::string &s)
{
    if(config->GetInt("showRhythmDebugMsg")){
        cout<<"[RHYTHM]: "<<s;
    }

}
void DebugOut::ImportantDebugMessage(const std::string &s)
{
    if(config->GetInt("showImportantDebugMsg")){
        cout<<"[!! IMPORTANT !!]: "<<s;
    }

}
