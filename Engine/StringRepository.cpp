#include "StringRepository.h"
#include "Func.h"
#include "DebugOut.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <string>

using namespace std;

StringRepository::StringRepository()
{
    configDebugID=0;
    ///check if config file already exists
    ifstream check("config.ini");
    bool exists = check.good();
    check.close();


}

void StringRepository::LoadLanguageFiles()
{
    /// at some point we need to make this able to read an
    /// arbritrary language file selected from config.ini.
    ///
    /// I have not done this yet though.

    ifstream conf("resources/lang/str_ENG.txt");

    vector<string> keysCheckList = stringKeys;

    if(conf.good())
    {
        string line;
        while(getline(conf, line))
        {
            ///ignore comments
            if(line.find("//") == std::string::npos)
            {
                ///Split the Key and Value
                vector<string> key = Func::Split(line,'|');
                stringMap[key[0]] = key[1];
                //cout << "Loaded string repo entry '" << key[0] << "' with value '" << key[1] << "'" << endl;

                for(int i=0; i<keysCheckList.size(); i++)
                {
                    if(keysCheckList[i] == key[0])
                    {
                        ///Already exists in the lang file, remove from check
                        keysCheckList.erase(keysCheckList.begin()+i);
                        break;
                    }
                }
            }
        }
    }
    else
    {
        cout << "ERROR! Could not load lang file." << endl;
    }

    conf.close();


}


std::string StringRepository::GetString(std::string key)
{
    return stringMap[key];
}

