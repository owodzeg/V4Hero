#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "Config.h"
#include "Func.h"
#include "V4Core.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <spdlog/spdlog.h>
#include "CoreManager.h"

using namespace std;

Config::Config()
{
    SPDLOG_INFO("Config initialization");

    configDebugID = 0;
    ///check if config file already exists
    ifstream check("config.ini");
    bool exists = check.good();
    check.close();

    ///if config not exists
    if (!exists)
    {
        ofstream conf("config.ini");

        if (conf.is_open())
        {
            ///Safety warning
            conf << "# Take caution! Changing some of the settings below may cause your game to crash or work really unstable! Don't edit this file if you don't know what you're doing! #";
            conf << '\n';

            for (unsigned int i = 0; i < configKeys.size(); i++)
            {
                ///save all keys and defaults
                conf << configKeys[i] << ":" << configDefaults[i];

                ///remember to newline
                if (i != configKeys.size() - 1)
                    conf << '\n';
            }
        }

        conf.close();
    }

    SPDLOG_INFO("Config initialization finished");
}

void Config::LoadConfig()
{
    ifstream conf("config.ini");

    vector<string> keysCheckList = configKeys;
    vector<string> keysCheckDefaults = configDefaults;

    if (conf.good())
    {
        string line;
        while (getline(conf, line))
        {
            ///ignore comments
            if (line.find("#") == std::string::npos)
            {
                if (line.back() == '\r')
                {
                    line.pop_back();
                }
                ///Split the Key and Value
                vector<string> key = Func::Split(line, ':');

                if (key.size() > 1)
                {
                    configMap[key[0]] = key[1];
                    SPDLOG_DEBUG("Loaded key '{}' with value '{}'", key[0], key[1]);

                    for (unsigned int i = 0; i < keysCheckList.size(); i++)
                    {
                        if (keysCheckList[i] == key[0])
                        {
                            ///Already exists in the config, remove from check
                            keysCheckList.erase(keysCheckList.begin() + i);
                            keysCheckDefaults.erase(keysCheckDefaults.begin() + i);
                            break;
                        }
                    }
                } else
                {
                    SPDLOG_WARN("Ignoring key '{}'. Reason: invalid value or corrupted config", key[0]);
                }
            }
        }
    } else
    {
        SPDLOG_ERROR("Could not load config file.");
    }

    conf.close();

    ofstream conf2("config.ini", ios::app);

    if (conf2.is_open())
    {
        for (unsigned int i = 0; i < keysCheckList.size(); i++)
        {
            if (i == 0)
                conf2 << '\n';

            ///save all keys and defaults
            conf2 << keysCheckList[i] << ":" << keysCheckDefaults[i];
            configMap[keysCheckList[i]] = keysCheckDefaults[i];

            SPDLOG_DEBUG("Adding missing config entry: '{}' = '{}'", keysCheckList[i], keysCheckDefaults[i]);

            ///remember to newline
            if (i != keysCheckList.size() - 1)
                conf2 << '\n';
        }
    }

    conf2.close();
}

void Config::SaveConfig()
{
    ofstream conf2("config.ini", ios::trunc);
    SPDLOG_DEBUG("Config Size: {}, Config Keys: {}", configMap.size(), configKeys.size());

    if (conf2.is_open())
    {
        for (unsigned int i = 0; i < configMap.size(); i++)
        {
            if (i == 0)
            {
                conf2 << "# Take caution! Changing some of the settings below may cause your game to crash or become unstable! Don't edit this file unless you know what you're doing! #" << '\n';
            }

            ///save all keys and defaults
            conf2 << configKeys[i] << ":" << configMap[configKeys[i]];

            SPDLOG_DEBUG("Saving config entry '{}' = '{}'", configKeys[i], configMap[configKeys[i]]);

            ///remember to newline
            if (i != configMap.size() - 1)
                conf2 << '\n';
        }
    }

    conf2.close();
}
void Config::ReloadLanguages()
{
    /** Load lang from resources/lang/str_ENG.cfg **/
    if (changedLang)
    {
        strRepo.LoadLanguageFiles(GetInt("lang"));
        SPDLOG_DEBUG("Language changed to {}", strRepo.GetString("language_file_loaded"));
        changedLang = false;
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
void Config::SetString(std::string key, std::string val)
{
    configMap[key] = val;
}
std::string Config::GetLanguageName()
{
    std::string s = strRepo.langNames[atoi(configMap["lang"].c_str()) - 1];
    std::string resws;
    resws.assign(s.begin(), s.end());
    return resws;
}

bool Config::keyExists(std::string key)
{
    if (configMap.count(key) == 0)
        return false;
    else
        return true;
}
