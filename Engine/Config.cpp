#include "Config.h"
#include "Func.h"
#include "DebugOut.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <string>

using namespace std;

Config::Config()
{
    configDebugID=0;
    debugOut = new DebugOut(this);
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

    /** Load lang from resources/lang/str_ENG.cfg **/
    strRepo.LoadLanguageFiles(GetInt("lang"));
    cout<<strRepo.GetString(L"language_file_loaded")<<endl;
}

void Config::SaveConfig()
{
    ofstream conf2("config.ini", ios::ate);
    cout<<"Config Size: "<<configMap.size()<<"Config Keys: "<<configKeys.size()<<endl;
    conf2.seekp(0);
    if(conf2.is_open())
    {
        for(int i=0; i<configMap.size(); i++)
        {
            if(i == 0){
                conf2 << "# Take caution! Changing some of the settings below may cause your game to crash or become unstable! Don't edit this file unless you know what you're doing! #" <<'\n';
            }
            ///save all keys and defaults
            conf2 << configKeys[i] << ":" << configMap[configKeys[i]];

            cout << "Saving config entry: " << configKeys[i] << " = " << configMap[configKeys[i]] << endl;

            ///remember to newline
            if(i != configMap.size()-1)
            conf2 << '\n';
        }
    }

    conf2.close();

}
void Config::ReloadLanguages(){
     /** Load lang from resources/lang/str_ENG.cfg **/
     if (changedLang){
        strRepo.LoadLanguageFiles(GetInt("lang"));
        cout<<strRepo.GetString(L"language_file_loaded")<<endl;
        changedLang=false;
     }
}
int Config::GetInt(std::string key)
{
    int num = atoi(configMap[key].c_str());
    return num;
}

std::string Config::GetString(std::string key)
{
    return configMap[key];
}
void Config::SetString(std::string key,std::string val)
{
    configMap[key] = val;
}
std::wstring Config::GetLanguageName(){
    switch (atoi(configMap["lang"].c_str())){
        case 1:
            return strRepo.GetUnicodeString(L"language_english");
            break;
        case 2:
            return strRepo.GetUnicodeString(L"language_french");
            break;
        case 3:
            return strRepo.GetUnicodeString(L"language_spanish");
            break;
        case 4:
            return strRepo.GetUnicodeString(L"language_danish");
            break;
        case 5:
            return strRepo.GetUnicodeString(L"language_polish");
            break;
        case 6:
        default:
            return strRepo.GetUnicodeString(L"language_english");
            break;
    }
}

