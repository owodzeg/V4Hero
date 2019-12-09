#include <vector>
#include <string>
#include <sstream>
#include <locale>
#include <codecvt>
#include "Func.h"
#include <iostream>


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
sf::String Func::ConvertToUtf8String(const std::wstring &s)
{
    std::basic_string<sf::Uint32> tmp;
    sf::Utf8::toUtf32(s.begin(),s.end(),std::back_inserter(tmp));
    return sf::String(tmp);
}
sf::String Func::ConvertToUtf8String(const std::string &s)
{
    std::basic_string<sf::Uint32> tmp;
    sf::Utf8::toUtf32(s.begin(),s.end(),std::back_inserter(tmp));
    return sf::String(tmp);
}
std::string Func::trim(const std::string& str, const std::string& whitespace = " \t")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}
std::wstring ConvertToWString(const std::string &s){
    std::wstring resws;
    resws.assign(s.begin(), s.end());
    return resws;
}

std::string ConvertToString(const std::wstring &ws){
    std::string s( ws.begin(), ws.end() );
    return s;
}

template<typename T> std::string Func::to_str(const T& t)
{
	ostringstream ss;
	ss << t;
	return ss.str();
}
