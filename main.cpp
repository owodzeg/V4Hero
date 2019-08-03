#include <iostream>

#include "Engine/V4Core.h"

// Includes needed for _setmode()
#include <io.h>
#include <fcntl.h>
using namespace std;

int main()
{
    cout << "V4" << endl;
    //_setmode(_fileno(stdout), _O_U16TEXT);
    //wchar_t * unicode_text = L"aäbcdefghijklmnoöpqrsßtuüvwxyz";
    //wprintf(L"%s", unicode_text);

    V4Core V4;
    V4.Init();

    return 0;
}
