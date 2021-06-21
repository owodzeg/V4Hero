#include "Binary.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std;

///Read entire binary file to std::string
std::string Binary::get_file(const std::string& path)
{
    cout << "[Binary] get_file(" << path << ")" << endl;

    //cout << "Get the file located at " << path << endl;

    std::ifstream file(path, std::ios::binary);

    if (!file)
    {
        cout << "Error when loading file" << endl;
    }

    auto sz = std::filesystem::file_size(path);

    //cout << "Filesystem: size of file is " << sz << " bytes" << endl;

    std::string content;
    content.resize(sz);
    file.read(content.data(), sz);

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
    return uint16_t(uint32_t(source[offset]) + uint32_t(source[offset + 1] << 8));
}

///Read four bytes into unsigned int from file on given offset
uint32_t Binary::get_uint32(const std::vector<unsigned char>& source, int offset)
{
    return (uint32_t(source[offset]) + uint32_t(source[offset + 1] << 8) + uint32_t(source[offset + 2] << 16) + uint32_t(source[offset + 3] << 24));
}

///Read four bytes into unsigned int from file on given offset in reverse endianness
uint32_t Binary::get_uint32_r(const std::vector<unsigned char>& source, int offset)
{
    return (uint32_t(source[offset] << 24) + uint32_t(source[offset + 1] << 16) + uint32_t(source[offset + 2] << 8) + uint32_t(source[offset + 3]));
}

///Read a null-terminated ANSI string from file on given offset
std::string Binary::get_string(const std::vector<unsigned char>& source, int offset)
{
    std::string temp;
    int str = 0;

    while (get_uint8(source, offset + str) != 0x0)
    {
        temp += get_uint8(source, offset + str);
        str++;
    }

    return temp;
}

///Converts a non-null-terminated ANSI string to std::string
std::string Binary::to_string(const std::vector<unsigned char>& source)
{
    std::string temp;

    for (unsigned int i = 0; i < source.size(); i++)
    {
        temp += get_uint8(source, i);
    }

    return temp;
}

///Read a block of given size from file on given offset
std::vector<unsigned char> Binary::get_block(const std::vector<unsigned char>& source, int offset, int block_size)
{
    vector<unsigned char> a(source.begin() + offset, source.begin() + offset + block_size);
    return a;
}
