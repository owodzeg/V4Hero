#include "SaveReader.h"
#include "Func.h"
#include "DebugOut.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <string>
#include "config.h"
using namespace std;

SaveReader::SaveReader()
{
    ifstream check("resources/data/sv1.p4sv");
    bool exists = check.good();
    check.close();

    ///if config not exists
    if(!exists)
    {
        ofstream conf("resources/data/sv1.p4sv");

        if(conf.is_open())
        {
            ///Safety warning
            conf << "# Take caution! The data below represents your save data! Don't edit it unless you know what you're doing, and if you must, PLEASE back it up somewhere else first <3 #";
            conf << '\n';
        }

        conf.close();
    }
    isNewSave = !exists;
}

void SaveReader::LoadSave(Config& tconfig)
{
    config = &tconfig;
    debugOut = config->debugOut;

    ifstream conf("resources/data/sv1.p4sv");
    if(conf.good())
    {
        string line;
        while(getline(conf, line))
        {
            ///ignore comments
            if(line.find("#") == std::string::npos)
            {
                ///Split the Key and Value
                vector<string> key = Func::Split(line,':');
                //configMap[key[0]] = key[1];
                //cout << "Loaded key '" << key[0] << "' with value '" << key[1] << "'" << endl;

            }
        }
    }
    else
    {
        cout << "ERROR! Could not load save file!" << endl;
    }
    conf.close();

}

void SaveReader::Save()
{
    ofstream conf2("resources/data/sv1.p4sv", ios::ate);
    conf2.seekp(0);
    if(conf2.is_open())
    {

            /*for(int i=0; i<configMap.size(); i++)
        {
            if(i == 0){
                conf2 << "# Take caution! Changing some of the settings below may cause your game to crash or become unstable! Don't edit this file unless you know what you're doing! #" <<'\n';
            }
            /*
            ///save all keys and defaults
            conf2 << configKeys[i] << ":" << configMap[configKeys[i]];

            cout << "Saving config entry: " << configKeys[i] << " = " << configMap[configKeys[i]] << endl;

            ///remember to newline
            if(i != configMap.size()-1)
            conf2 << '\n';*/
        //}
    }

    conf2.close();
}

