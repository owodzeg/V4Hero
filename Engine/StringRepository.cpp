#include "StringRepository.h"
#include "DebugOut.h"
#include "Func.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <codecvt>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <string>
#include <vector>
using namespace std;

StringRepository::StringRepository()
{
    configDebugID = 0;
    ///check if config file already exists
    ifstream check("config.ini");
    bool exists = check.good();
    check.close();
}
void StringRepository::LoadLanguageFile(ifstream* conf)
{
    vector<string> keysCheckList;
    std::locale old_locale;
    std::locale utf8_locale(old_locale, new std::codecvt_utf8<wchar_t>);
    conf->imbue(utf8_locale);


    if (conf->good())
    {
        std::string line;
        while (getline(*conf, line))
        {
            ///ignore comments
            if (line.find("//") == std::string::npos)
            {
                ///Split the Key and Value
                ///wcout<<line<<endl;

                if (line.back() == '\r')
                {
                    line.pop_back();
                }
                vector<string> key = Func::Split(line, '|');

                if (key.size() == 2)
                {
                    std::string s(key[0].begin(), key[0].end());

                    for (int k = 0; k < key[1].size(); k++)
                    {
                        if (key[1][k] == '\\')
                            key[1][k] = '\n';
                    }

                    stringMap[s] = key[1];
                }
            }
        }
    } else
    {
        cout << "ERROR! Could not load lang file." << endl;
    }

    conf->close();
}

void StringRepository::LoadLanguageFiles(int langNum)
{
    ifstream conf("resources/lang/lang.txt");

    if (conf.good())
    {
        std::string line;
        while (getline(conf, line))
        {
            ///ignore comments
            if (line.find("//") == std::string::npos)
            {
                if (line.back() == '\r')
                {
                    line.pop_back();
                }
                vector<string> key = Func::Split(line, '|');

                if (key.size() == 3)
                {
                    cout << "Loaded language file'" << key[0] << "' with value '" << key[1] << "'" << endl;
                    langFiles.push_back("" + key[1]);
                    langNames.push_back(key[0]);
                    langFonts.push_back(key[2]);
                }
            }
        }

        ifstream conf2("resources/lang/" + langFiles[langNum - 1] + ".txt");
        cout << "#### Loading language file: " << langNames[langNum - 1] << endl;
        LoadLanguageFile(&conf2);
    } else
    {
        ifstream conf2("resources/lang/str_ENG.txt");
        LoadLanguageFile(&conf2);
    }
    conf.close();

    /*switch (langNum){
        case 1:
            {
                wifstream conf("resources/lang/str_ENG.txt");
                LoadLanguageFile(&conf);
                break;
            }

        case 2:{
            wifstream conf("resources/lang/str_FRA.txt");
            LoadLanguageFile(&conf);
            break;}
        case 3:{
            wifstream conf("resources/lang/str_SPA.txt");
                LoadLanguageFile(&conf);
            break;}
        case 4:{
            wifstream conf("resources/lang/str_DAN.txt");
            LoadLanguageFile(&conf);
            break;}
        case 5:{
            wifstream conf("resources/lang/str_POL.txt");
            LoadLanguageFile(&conf);
            break;}
        case 6:{
            wifstream conf("resources/lang/str_EMJ.txt");
            LoadLanguageFile(&conf);
            break;}
        default:{
            wifstream conf("resources/lang/str_ENG.txt");
            LoadLanguageFile(&conf);
            break;}

    }*/
}


std::string StringRepository::GetString(std::string key)
{
    return stringMap[key];
}

