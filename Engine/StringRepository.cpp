#include "StringRepository.h"
#include "Func.h"
#include "DebugOut.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <locale>
#include <codecvt>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;

StringRepository::StringRepository()
{
    configDebugID=0;
    ///check if config file already exists
    ifstream check("config.ini");
    bool exists = check.good();
    check.close();


}
void StringRepository::LoadLanguageFile(wifstream* conf)
{
    vector<wstring> keysCheckList;
    //conf->imbue(std::locale(conf->getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>));


    if(conf->good())
    {
        std::wstring line;
        while(getline(*conf, line))
        {
            ///ignore comments
            if(line.find(L"//") == std::wstring::npos)
            {
                ///Split the Key and Value
                ///wcout<<line<<endl;
                vector<wstring> key = Func::Split(line,L'|');
                if(key.size() == 2)
                {
                    std::string s(key[0].begin(), key[0].end());

                    for(int k=0; k<key[1].size(); k++)
                    {
                        if(key[1][k] == L'\\')
                        key[1][k] = L'\n';
                    }

                    stringMap[s] = key[1];
                }
            }
        }
    }
    else
    {
        cout << "ERROR! Could not load lang file." << endl;
    }

    conf->close();
}
void StringRepository::LoadLanguageFiles(int langNum)
{
    ifstream conf("resources/lang/lang.txt");
    if(conf.good())
    {
        std::string line;
        while(getline(conf, line))
        {
            ///ignore comments
            if(line.find("//") == std::string::npos)
            {
                vector<string> key = Func::Split(line,L'|');

                if(key.size() == 2)
                {
                    cout << "Loaded language file'" << key[0] << "' with value '" << key[1] << "'" << endl;
                    langFiles.push_back(""+key[1]);
                    langNames.push_back(key[0]);
                }
            }
        }
        wifstream conf2("resources/lang/"+langFiles[langNum-1]+".txt");
        cout<<"#### Loading language file: "<< langNames[langNum-1] <<endl;
        LoadLanguageFile(&conf2);
    } else {
        wifstream conf2("resources/lang/str_ENG.txt");
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


std::wstring StringRepository::GetUnicodeString(std::wstring key)
{
    std::string s(key.begin(), key.end());
    return stringMap[s];
}

std::wstring StringRepository::GetUnicodeString(std::string key)
{
    return stringMap[key];
}

std::string StringRepository::GetString(std::wstring key)
{
    std::string s(key.begin(), key.end());
    std::wstring ws = stringMap[s];
    std::string o(ws.begin(), ws.end());
    return o;
}

std::string StringRepository::GetString(std::string key)
{
    std::wstring ws = stringMap[key];
    std::string s(ws.begin(), ws.end());
    return s;
}

