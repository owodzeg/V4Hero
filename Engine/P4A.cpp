#include "P4A.h"
#include "Binary.hpp"
#include <algorithm>

using namespace std;

P4A::P4A()
{
    ///P4A header
    output_header.push_back(0x50);
    output_header.push_back(0x34);
    output_header.push_back(0x41);

    ///P4A version
    output_header.push_back(0x01);
}

int P4A::GetFileSize(string filename)
{
    ///Reads the size of a file
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return int(in.tellg());
}

void P4A::LoadFile(string filename)
{
    ///Save the original path of the file
    original_path.push_back(filename);

    ///Strip the file name only
    filenames.push_back(filename.substr(filename.find_last_of("\\") + 1));

    ///Get the size of a file
    filesizes.push_back(GetFileSize(filename));
}

void P4A::CreateDictionary()
{
    ///Calculate the full size of dictionary
    for (int i = 0; i < filenames.size(); i++)
    {
        dictionary_size += 9 + filenames[i].size();
    }

    ///Offset where files start
    file_offset = dictionary_size;

    for (int i = 0; i < filenames.size(); i++)
    {
        ///Get file name, name's length and convert it to 1 byte signed (what for???)
        string file = filenames[i];
        int filename_length = file.size();
        int8_t length_8bit = filename_length;

        ///Save the 8-bit length
        output_dictionary.push_back(length_8bit);

        ///Save the file's name
        for (int a = 0; a < file.size(); a++)
        {
            output_dictionary.push_back(file[a]);
        }

        ///Save the file's offset
        output_dictionary.push_back(char(uint32_t(file_offset)));
        output_dictionary.push_back(char(uint32_t(file_offset) >> 8));
        output_dictionary.push_back(char(uint32_t(file_offset) >> 16));
        output_dictionary.push_back(char(uint32_t(file_offset) >> 24));

        ///Save the file's size
        file_size = filesizes[i];

        output_dictionary.push_back(char(uint32_t(file_size)));
        output_dictionary.push_back(char(uint32_t(file_size) >> 8));
        output_dictionary.push_back(char(uint32_t(file_size) >> 16));
        output_dictionary.push_back(char(uint32_t(file_size) >> 24));

        ///Increment offset by file's size
        file_offset += file_size;
    }

    ///Save the dictionary size to header
    output_header.push_back(char(uint32_t(dictionary_size - 8)));
    output_header.push_back(char(uint32_t(dictionary_size - 8) >> 8));
    output_header.push_back(char(uint32_t(dictionary_size - 8) >> 16));
    output_header.push_back(char(uint32_t(dictionary_size - 8) >> 24));
}

void P4A::SaveToFile(string filename)
{
    ///Clear the dictionary in case the handle was used before
    output_dictionary.clear();

    ///Rebuild dictionary
    CreateDictionary();

    ofstream file(filename, std::ios_base::binary);

    while (file.is_open())
    {
        ///Write header
        for (int i = 0; i < output_header.size(); i++)
        {
            file.put(output_header[i]);
        }

        ///Write dictionary
        for (int i = 0; i < output_dictionary.size(); i++)
        {
            file.put(output_dictionary[i]);
        }

        ///Write files
        for (int i = 0; i < filenames.size(); i++)
        {
            ifstream get_file(filenames[i], std::ios_base::binary);

            char ch;

            while (get_file.get(ch))
                file.put(ch);

            get_file.close();
        }

        file.close();
    }
}

void P4A::ReadDictionary(std::string filename)
{
    p4a_filename = filename;

    cout << "[P4A] Reading " << p4a_filename << endl;

    ifstream file(filename, std::ifstream::binary);

    char header[3];
    file.read(header, 3);

    std::vector<unsigned char> bin_data = Binary::file_to_uchar(Binary::get_file(filename));

    //cout << header[0] << header[1] << header[2] << endl;
    if (header[0] == 'P' && header[1] == '4' && header[2] == 'A')
    {
        //cout << "[P4A] Correct archive format!" << endl;

        uint8_t file_version = Binary::get_uint8(bin_data, 3);
        ;

        if (file_version == 1)
        {
            cout << "[P4A] Detected Archive version 1" << endl;

            uint32_t file_dictionary_size = Binary::get_uint32(bin_data, 4);

            cout << "[P4A] Dictionary size is: " << file_dictionary_size << endl;

            int file_data_pointer = file_dictionary_size + 8;

            p4a_offset = 8;

            while (p4a_offset < file_data_pointer)
            {
                uint8_t filename_length = Binary::get_uint8(bin_data, p4a_offset);
                p4a_offset += 1;

                string str_filename = Binary::to_string(Binary::get_block(bin_data, p4a_offset, filename_length));
                p4a_offset += filename_length;

                uint32_t file_offset = Binary::get_uint32(bin_data, p4a_offset);
                p4a_offset += 4;

                uint32_t file_size = Binary::get_uint32(bin_data, p4a_offset);
                p4a_offset += 4;

                files[str_filename] = Binary::get_block(bin_data, file_offset, file_size);
            }
        } else
        {
            cout << "[P4A] Incorrect file version!" << endl;
        }
    } else
    {
        cout << "[P4A] Incorrect file type" << endl;
        ///exit
    }

    file.close();
}

std::string P4A::ReadToMemory(std::string name)
{
    cout << "[P4A] Reading " << name << " from memory" << endl;

    return string(files[name].begin(), files[name].end());
}

vector<unsigned char> P4A::ReadToMemoryChar(std::string name)
{
    return files[name];
}