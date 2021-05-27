#include <vector>
#include <string>
#include <sstream>
#include <locale>
#include <codecvt>
#include "Func.h"
#include <iostream>
#include <iomanip>

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

std::string Func::wrap_text(std::string input, int box_width, sf::Font& font, int character_size)
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
                ///need to optimize this because it doesnt work correctly for chinese
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

/**std::wstring Func::wrap_text(std::wstring input, int box_width, sf::Font& font, int character_size)
{
    wcout << "wrap_text(" << input << ", " << box_width << ")" << endl;

    if(input.size() <= 0)
    return L"";

    wstring temp = L"";
    vector<wstring> words;

    for(int i=0; i<input.size(); i++)
    {
        if((input[i] == L' ') || (input[i] == L'\n') || (i == input.size()-1))
        {
            if(i == input.size()-1)
            temp += input[i];

            if(input[i] == L'\n')
            temp += input[i];

            wcout << "Registered word: " << temp << endl;
            words.push_back(temp);
            temp = L"";
        }
        else
        {
            temp += input[i];
        }
    }

    wstring full = L"";

    wstring prevtemp = L"";
    int wordcount = 0;
    temp = L"";

    for(int i=0; i<words.size(); i++)
    {
        prevtemp = temp;

        if(words[i].find(L"\n") != std::wstring::npos)
        {
            cout << "wstring found to contain a new line character" << endl;
            wstring prefull = prevtemp + L" " + words[i];

            sf::Text t_temp;
            t_temp.setFont(font);
            t_temp.setCharacterSize(character_size);
            t_temp.setString(prefull);

            if(t_temp.getGlobalBounds().width >= box_width)
            {
                full += prevtemp + L'\n';
                i--;
            }
            else
            {
                full += prefull;
            }

            //wcout << "Added \"" << prevtemp << words[i] << "\" to the output" << endl;

            i++;
            temp = L"";
            wordcount = 0;
        }

        if(wordcount>0)
        temp += L" ";

        temp += words[i];
        wordcount++;

        sf::Text t_temp;
        t_temp.setFont(font);
        t_temp.setCharacterSize(character_size);
        t_temp.setString(temp);

        //cout << "Testing wstring \"" << temp << "\", " << wordcount << " words, size: " << t_temp.getGlobalBounds().width << endl;

        if(t_temp.getGlobalBounds().width >= box_width)
        {
            if(wordcount > 1)
            {
                cout << "wstring exceeded the max box width (" << box_width << " vs " << t_temp.getGlobalBounds().width << ")" << endl;
                full += prevtemp;
                full += '\n';

                //cout << "Added \"" << prevtemp << "\" to the output" << endl;

                i -= 1;
                temp = L"";
                wordcount = 0;
            }
            else
            {
                wstring ltemp = L"";

                ///if its just a long ass word
                for(int e=0; e<temp.size(); e++)
                {
                    ltemp += temp[e];

                    sf::Text t_ltemp;
                    t_ltemp.setFont(font);
                    t_ltemp.setCharacterSize(character_size);
                    t_ltemp.setString(ltemp);

                    cout << "Testing word " << i << " letter " << e+1 << "/" << temp.size() << " : " << t_ltemp.getGlobalBounds().width << "/" << box_width-30 << endl;

                    if(t_ltemp.getGlobalBounds().width >= box_width-30)
                    {
                        full += ltemp;
                        full += L"-";
                        full += '\n';

                        ltemp = L"";
                    }
                }

                ///adding remains
                temp = ltemp;
                wordcount = 1;
            }
        }
    }

    //cout << "End of word list, adding remains to the full wstring" << endl;
    full += temp;

    return full;
}*/

std::wstring Func::wrap_text(std::wstring input, int box_width, sf::Font& font, int character_size)
{
    //cout << "Prepare output string" << endl;
    wstring output = L"";

    //cout << "Split text into lines" << endl;
    vector<wstring> lines = Split(input, L'\n');

    //cout << "Go over lines and squish them if necessary" << endl;
    for(int i=0; i<lines.size(); i++)
    {
        //cout << "Split lines into words" << endl;
        vector<wstring> words = Split(lines[i], L' ');

        //cout << "Vector for sub lines" << endl;
        vector<wstring> small_lines;

        //cout << "Test if words doesn't need to be wrapped" << endl;
        wstring length_test = L"";
        for(int w=0; w<words.size(); w++)
        {
            //cout << "Testing word " << w << " size: " << words[w].size() << endl;

            //cout << "Increase the sub line" << endl;
            //cout << "Preserve the original line, if tested one is too big" << endl;
            wstring pre_length_test = length_test;
            length_test += words[w]+L" ";

            //cout << "Test the sub line" << endl;
            sf::Text t_tmp;
            t_tmp.setFont(font);
            t_tmp.setCharacterSize(character_size);
            t_tmp.setString(ConvertToUtf8String(length_test));

            if(t_tmp.getLocalBounds().width > box_width)
            {
                //cout << "Line is too big." << endl;
                if(pre_length_test.size() > 0)
                {
                    //cout << "Ok, some words were already parsed, submit the smaller line" << endl;
                    //cout << "Submit the original line without tested word" << endl;
                    small_lines.push_back(pre_length_test);

                    //cout << "Reset length test" << endl;
                    length_test = L"";

                    //cout << "Move the loop back one step, so the word that exceeded size won't be skipped" << endl;
                    w--;
                }
                else
                {
                    //cout << "No words were submitted, that means the word is so huge, it needs to be split into smaller pieces." << endl;
                    //cout << "We need to go through every character, testing the sample if it fits the box." << endl;
                    wstring word_test = L"";

                    ///Check string letter by letter
                    for(int e=0; e<length_test.size(); e++)
                    {
                        //cout << "Preserve original word without testing letter" << endl;
                        wstring pre_word_test = word_test;
                        int bpw = 1; ///byte per word

                        ///Check if letter is from different alphabet
                        if((length_test[e] >= 0xE0) && (length_test[e] <= 0xEF))
                        {
                            ///3-byte letter detected, treat differently
                            word_test += length_test[e];
                            word_test += length_test[e+1];
                            word_test += length_test[e+2];

                            bpw = 3;
                            e += 2;

                        }
                        else ///otherwise treat like regular text, there should be no problems with that
                        {
                            //cout << "Add one letter (" << e+1 << "/" << length_test.size() << endl;
                            word_test += length_test[e];
                        }

                        //cout << "Test the word" << endl;
                        sf::Text t_tmp;
                        t_tmp.setFont(font);
                        t_tmp.setCharacterSize(character_size);
                        t_tmp.setString(ConvertToUtf8String(word_test));

                        if(t_tmp.getLocalBounds().width > box_width-30)
                        {
                            //cout << "Tested word is too big, submit the letters" << endl;
                            if(bpw != 3)
                            pre_word_test += L"-";

                            small_lines.push_back(pre_word_test);

                            word_test = L"";

                            //cout << "Move the loop back one step, so the letter that exceeded size won't be skipped" << endl;
                            e-=bpw;
                        }
                        else
                        {
                            //cout << "Tested word is appropriate! Continue, unless it has finished going over words." << endl;
                            if(e == length_test.size()-1)
                            {
                                small_lines.push_back(word_test);
                            }
                        }
                    }
                }
            }
            else
            {
                //cout << "Line is appropriate! Continue incrementing." << endl;
                //cout << "UNLESS! There are no words left" << endl;
                if(w == words.size()-1)
                {
                    small_lines.push_back(length_test);
                }
            }
        }

        for(int s=0; s<small_lines.size(); s++)
        {
            output += small_lines[s]+L'\n';
        }
    }

    return output;
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

std::string Func::num_padding(int num, int padding) ///stolen from cplusplus.com
{
    std::ostringstream ss;
    ss << std::setw(padding) << std::setfill('0') << num;

    return ss.str();
}
