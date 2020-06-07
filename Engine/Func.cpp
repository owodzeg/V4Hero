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

std::string Func::wrap_text(std::string input, int box_width, sf::Font font, int character_size)
{
    //cout << "wrap_text(" << input << ", " << box_width << ")" << endl;

    if(input.size() <= 0)
    return "";

    string temp = "";
    vector<string> words;

    for(int i=0; i<input.size(); i++)
    {
        if((input[i] == ' ') || (input[i] == '\n') || (i == input.size()-1))
        {
            if(i == input.size()-1)
            temp += input[i];

            if(input[i] == '\n')
            temp += input[i];

            //cout << "Registered word: " << temp << endl;
            words.push_back(temp);
            temp = "";
        }
        else
        {
            temp += input[i];
        }
    }

    string full = "";

    string prevtemp = "";
    int wordcount = 0;
    temp = "";

    for(int i=0; i<words.size(); i++)
    {
        prevtemp = temp;

        if(words[i].find("\n") != std::string::npos)
        {
            //cout << "String found to contain a new line character" << endl;
            string prefull = prevtemp + " " + words[i];

            sf::Text t_temp;
            t_temp.setFont(font);
            t_temp.setCharacterSize(character_size);
            t_temp.setString(prefull);

            if(t_temp.getGlobalBounds().width >= box_width)
            {
                full += prevtemp + '\n';
                i--;
            }
            else
            {
                full += prefull;
            }

            cout << "Added \"" << prevtemp + words[i] << "\" to the output" << endl;

            i++;
            temp = "";
            wordcount = 0;
        }

        if(wordcount>0)
        temp += " ";

        temp += words[i];
        wordcount++;

        sf::Text t_temp;
        t_temp.setFont(font);
        t_temp.setCharacterSize(character_size);
        t_temp.setString(temp);

        //cout << "Testing string \"" << temp << "\", " << wordcount << " words, size: " << t_temp.getGlobalBounds().width << endl;

        if(t_temp.getGlobalBounds().width >= box_width)
        {
            if(wordcount > 1)
            {
                //cout << "String exceeded the max box width (" << box_width << " vs " << t_temp.getGlobalBounds().width << ")" << endl;
                full += prevtemp;
                full += '\n';

                //cout << "Added \"" << prevtemp << "\" to the output" << endl;

                i -= 1;
                temp = "";
                wordcount = 0;
            }
            else
            {
                string ltemp = "";

                ///if its just a long ass word
                for(int e=0; e<temp.size(); e++)
                {
                    ltemp += temp[e];

                    sf::Text t_ltemp;
                    t_ltemp.setFont(font);
                    t_ltemp.setCharacterSize(character_size);
                    t_ltemp.setString(ltemp);

                    if(t_ltemp.getGlobalBounds().width >= box_width-30)
                    {
                        full += ltemp;
                        full += "-";
                        full += '\n';

                        ltemp = "";
                    }
                }

                ///adding remains
                temp = ltemp;
                wordcount = 1;
            }
        }
    }

    //cout << "End of word list, adding remains to the full string" << endl;
    full += temp;

    return full;
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
