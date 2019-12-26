#include <iostream>
#include <windows.h>

#include "Engine/V4Core.h"

// Includes needed for _setmode()
#include <io.h>
#include <fcntl.h>
using namespace std;

int main(int argc, char *argv[])
{
    cout << "V4" << endl;
    //_setmode(_fileno(stdout), _O_U16TEXT);
    //wchar_t * unicode_text = L"aäbcdefghijklmnoöpqrsßtuüvwxyz";
    //wprintf(L"%s", unicode_text);

    ///Fix for buggy CreateProcess not copying the working directory properly
    string wd = argv[0];
    wd = wd.substr(0,wd.find_last_of("\\"));
    SetCurrentDirectoryA(wd.c_str());

    V4Core V4;
    V4.Init();

    return 0;
}
