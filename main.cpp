#include <iostream>
#include <string>
#include <windows.h>

#include "Engine/V4Core.h"

// Includes needed for _setmode()
#include <io.h>
#include <fcntl.h>
using namespace std;
string ExePath() {
    char buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    string::size_type pos = string( buffer ).find_last_of( "\\/" );
    return string( buffer ).substr( 0, pos);
}
int main(int argc, char *argv[])
{
    std::cout << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;

    cout << "V4" << endl;
    cout << "my directory is " << ExePath() << "\n";
    //_setmode(_fileno(stdout), _O_U16TEXT);
    //wchar_t * unicode_text = L"aäbcdefghijklmnoöpqrsßtuüvwxyz";
    //wprintf(L"%s", unicode_text);

    ///Fix for buggy CreateProcess not copying the working directory properly
    string wd = argv[0];
    wd = wd.substr(0,wd.find_last_of("\\"));
    SetCurrentDirectoryA(wd.c_str());

    V4Core V4;
    V4.init();

    return 0;
}
