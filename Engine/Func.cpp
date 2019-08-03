#include <vector>
#include <string>
#include <sstream>
#include <locale>
#include <codecvt>
#include "Func.h"

using namespace std;

std::vector<std::string> Func::Split(const std::string &s, char delim)
{
	std::vector<std::string> elems;

	std::stringstream ss(s);
	std::string item;
	while(std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}

	return elems;
}
std::vector<std::wstring> Func::Split(const std::wstring &s, wchar_t delim)
{
    std::vector<std::wstring> ResultVec;
	std::wstringstream sStream(s);
    std::wstring item;

    while (std::getline(sStream, item, delim)) {
        ResultVec.push_back(item);
    }
    return ResultVec;
}
template<typename T> std::string Func::to_str(const T& t)
{
	ostringstream ss;
	ss << t;
	return ss.str();
}
