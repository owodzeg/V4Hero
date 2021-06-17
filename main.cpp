#include <iostream>
#include <filesystem>

#include "Engine/V4Core.h"

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    std::cout << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;

    cout << "V4" << endl;
    auto executable_path = weakly_canonical(fs::path(argv[0])).parent_path();
    cout << "my directory is " << executable_path << "\n";
    //_setmode(_fileno(stdout), _O_U16TEXT);
    //wchar_t * unicode_text = L"aäbcdefghijklmnoöpqrsßtuüvwxyz";
    //wprintf(L"%s", unicode_text);

    fs::current_path(executable_path);

    V4Core V4;
    V4.init();

    return 0;
}
