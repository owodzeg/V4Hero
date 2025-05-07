#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "P4A.h"
#include "Binary.hpp"
#include <algorithm>
#include <spdlog/spdlog.h>



P4A::P4A()
{
    ///P4A header
    output_header.push_back(0x50);
    output_header.push_back(0x34);
    output_header.push_back(0x41);

    ///P4A version
    output_header.push_back(0x01);
}

int P4A::GetFileSize(std::string filename)
{
    ///Reads the size of a file
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return int(in.tellg());
}

void P4A::LoadFile(std::string filename)
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
        dictionary_size += 9 + static_cast<int>(filenames[i].size());
    }

    ///Offset where files start
    file_offset = dictionary_size;

    for (int i = 0; i < filenames.size(); i++)
    {
        ///Get file name, name's length and convert it to 1 byte signed (what for???)
        std::string file = filenames[i];
        int filename_length = static_cast<int>(file.size());
        int8_t length_8bit = static_cast<int8_t>(filename_length);

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

void P4A::SaveToFile(std::string filename)
{
    ///Clear the dictionary in case the handle was used before
    output_dictionary.clear();

    ///Rebuild dictionary
    CreateDictionary();

    std::ofstream file(filename, std::ios_base::binary);

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
            std::ifstream get_file(filenames[i], std::ios_base::binary);

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

    SPDLOG_INFO("Reading file: {}", p4a_filename);

    std::ifstream file(filename, std::ifstream::binary);

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
            SPDLOG_DEBUG("Detected Archive version 1");

            uint32_t file_dictionary_size = Binary::get_uint32(bin_data, 4);

            SPDLOG_DEBUG("Dictionary size is: {}", file_dictionary_size);

            int file_data_pointer = file_dictionary_size + 8;

            p4a_offset = 8;

            while (p4a_offset < file_data_pointer)
            {
                uint8_t filename_length = Binary::get_uint8(bin_data, p4a_offset);
                p4a_offset += 1;

                std::string str_filename = Binary::to_string(Binary::get_block(bin_data, p4a_offset, filename_length));
                p4a_offset += filename_length;

                uint32_t l_file_offset = Binary::get_uint32(bin_data, p4a_offset);
                p4a_offset += 4;

                uint32_t l_file_size = Binary::get_uint32(bin_data, p4a_offset);
                p4a_offset += 4;

                files[str_filename] = Binary::get_block(bin_data, l_file_offset, l_file_size);
            }
        } else
        {
            SPDLOG_ERROR("Incorrect file version!");
        }
    } else
    {
        SPDLOG_ERROR("Incorrect file type");
        ///exit
    }

    file.close();
}

std::string P4A::ReadToMemory(std::string name)
{
    SPDLOG_INFO("Reading file from memory: {}", name);

    return std::string(files[name].begin(), files[name].end());
}

std::vector<unsigned char> P4A::ReadToMemoryChar(std::string name)
{
    return files[name];
}