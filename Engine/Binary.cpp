#include "Binary.hpp"
#include <fstream>
#include <iostream>

using namespace std;

///Read entire binary file to std::string
std::string Binary::get_file(const std::string& path)
{
    string npath = "";

    for(unsigned int i=0; i<path.size(); i++)
    {
        if(path[i] == '/')
        {
            npath += '\\';
        }
        else
        {
            npath += path[i];
        }
    }

    //cout << "Get the file located at " << npath << endl;

    std::ifstream file(npath, std::ios::binary);

    if(!file)
    {
        //cout << "Error when loading file" << endl;
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();

    return content;
}

std::vector<unsigned char> Binary::file_to_uchar(std::string const& str)
{
    return std::vector<unsigned char>(str.data(), str.data() + str.length());
}

std::string Binary::uchar_to_file(std::vector<unsigned char>& data)
{
    std::string str(data.begin(), data.end());
    return str;
}

///Read a single byte into unsigned int from file on given offset
uint8_t Binary::get_uint8(const std::vector<unsigned char>& source, int offset)
{
    return uint8_t(source[offset]);
}

///Read two bytes into unsigned int from file on given offset
uint16_t Binary::get_uint16(const std::vector<unsigned char>& source, int offset)
{
    return uint16_t(uint32_t(source[offset]) + uint32_t(source[offset+1] << 8));
}

///Read four bytes into unsigned int from file on given offset
uint32_t Binary::get_uint32(const std::vector<unsigned char>& source, int offset)
{
    return (uint32_t(source[offset]) + uint32_t(source[offset+1] << 8) + uint32_t(source[offset+2] << 16) + uint32_t(source[offset+3] << 24));
}

///Read a null-terminated ANSI string from file on given offset
std::string Binary::get_string(const std::vector<unsigned char>& source, int offset)
{
    std::string temp;
    int str = 0;

    while(get_uint8(source,offset+str) != 0x0)
    {
        temp += get_uint8(source,offset+str);
        str++;
    }

    return temp;
}

///Read a block of given size from file on given offset
std::vector<unsigned char> Binary::get_block(const std::vector<unsigned char>& source, int offset, int block_size)
{
    vector<unsigned char> a(source.begin() + offset, source.begin() + offset + block_size);
    return a;
}
