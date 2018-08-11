#include "Config.h"
#include "Func.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <string>

using namespace std;

Config::Config()
{
    ///check if config file already exists
    ifstream check("config.ini");
    bool exists = check.good();
    check.close();

    ///if config not exists
    if(!exists)
    {
        ofstream conf("config.ini");

        if(conf.is_open())
        {
            ///Safety warning
            conf << "# Take caution! Changing some of the settings below may cause your game to crash or work really unstable! Don't edit this file if you don't know what you're doing! #";
            conf << '\n';

            for(int i=0; i<configKeys.size(); i++)
            {
                ///save all keys and defaults
                conf << configKeys[i] << ":" << configDefaults[i];

                ///remember to newline
                if(i != configKeys.size()-1)
                conf << '\n';
            }
        }

        conf.close();
    }
}

void Config::LoadConfig()
{
    ifstream conf("config.ini");

    vector<string> keysCheckList = configKeys;
    vector<string> keysCheckDefaults = configDefaults;

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
                configMap[key[0]] = key[1];

                cout << "Loaded key '" << key[0] << "' with value '" << key[1] << "'" << endl;

                for(int i=0; i<keysCheckList.size(); i++)
                {
                    if(keysCheckList[i] == key[0])
                    {
                        ///Already exists in the config, remove from check
                        keysCheckList.erase(keysCheckList.begin()+i);
                        keysCheckDefaults.erase(keysCheckDefaults.begin()+i);
                        break;
                    }
                }
            }
        }
    }
    else
    {
        cout << "ERROR! Could not load config file." << endl;
    }

    conf.close();

    ofstream conf2("config.ini", ios::app);

    if(conf2.is_open())
    {
        for(int i=0; i<keysCheckList.size(); i++)
        {
            if(i == 0)
            conf2 << '\n';

            ///save all keys and defaults
            conf2 << keysCheckList[i] << ":" << keysCheckDefaults[i];
            configMap[keysCheckList[i]] = keysCheckDefaults[i];

            cout << "Adding missing config entry: " << keysCheckList[i] << " = " << keysCheckDefaults[i] << endl;

            ///remember to newline
            if(i != keysCheckList.size()-1)
            conf2 << '\n';
        }
    }

    conf2.close();
}

void Config::SaveConfig()
{

}

int Config::GetInt(std::string key)
{
    return atoi(configMap[key].c_str());
}

std::string Config::GetString(std::string key)
{
    return configMap[key];
}

