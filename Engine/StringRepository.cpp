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
    vector<wstring> keysCheckList = stringKeys;
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
                if (key.size()==2){
                    stringMap[key[0]] = key[1];
                    wcout << "Loaded string repo entry '" << key[0] << "' with value '" << key[1] << "'" << endl;

                    /// below code shows raw unicode bytes
                    /*
                    const size_t length = key[1].length();
                    for (size_t i = 0; i < length; ++i)
                    {
                    unsigned int value = key[1][i];
                    std::cout << std::setw(2) << std::setfill('0') << std::hex << value << ' ' ;
                    }
                    cout<< "\n";
                    */

                    for(int i=0; i<keysCheckList.size(); i++)
                    {
                        if(keysCheckList[i] == key[0])
                        {
                            ///Already exists in the strRepo file, remove from check
                            keysCheckList.erase(keysCheckList.begin()+i);
                            break;
                        }
                    }
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
    /// at some point we need to make this able to read an
    /// arbritrary language file selected from config.ini.
    ///
    /// I have not done this yet though.
    switch (langNum){
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

    }
}


std::wstring StringRepository::GetUnicodeString(std::wstring key)
{
    return stringMap[key];
}
std::string StringRepository::GetString(std::wstring key)
{
    std::wstring ws = stringMap[key];
                        std::string s( ws.begin(), ws.end() );
    return s;
}

