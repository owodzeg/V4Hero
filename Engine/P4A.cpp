#include "P4A.h"

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
    filenames.push_back(filename.substr(filename.find_last_of("\\")+1));

    ///Get the size of a file
    filesizes.push_back(GetFileSize(filename));
}

void P4A::CreateDictionary()
{
    ///Calculate the full size of dictionary
    for(int i=0; i<filenames.size(); i++)
    {
        dictionary_size += 9 + filenames[i].size();
    }

    ///Offset where files start
    file_offset = dictionary_size;

    for(int i=0; i<filenames.size(); i++)
    {
        ///Get file name, name's length and convert it to 1 byte signed (what for???)
        string file = filenames[i];
        int filename_length = file.size();
        int8_t length_8bit = filename_length;

        ///Save the 8-bit length
        output_dictionary.push_back(length_8bit);

        ///Save the file's name
        for(int a=0; a<file.size(); a++)
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
    output_header.push_back(char(uint32_t(dictionary_size-8)));
    output_header.push_back(char(uint32_t(dictionary_size-8) >> 8));
    output_header.push_back(char(uint32_t(dictionary_size-8) >> 16));
    output_header.push_back(char(uint32_t(dictionary_size-8) >> 24));
}

void P4A::SaveToFile(string filename)
{
    ///Clear the dictionary in case the handle was used before
    output_dictionary.clear();

    ///Rebuild dictionary
    CreateDictionary();

    ofstream file(filename,std::ios_base::binary);

    while(file.is_open())
    {
        ///Write header
        for(int i=0; i<output_header.size(); i++)
        {
            file.put(output_header[i]);
        }

        ///Write dictionary
        for(int i=0; i<output_dictionary.size(); i++)
        {
            file.put(output_dictionary[i]);
        }

        ///Write files
        for(int i=0; i<filenames.size(); i++)
        {
            ifstream get_file(filenames[i], std::ios_base::binary);

            char ch;

            while(get_file.get(ch))
            file.put(ch);

            get_file.close();
        }

        file.close();
    }
}

void P4A::ReadDictionary(std::string filename)
{
    p4a_filename = filename;

    ifstream file(filename, std::ifstream::binary);

    char header[3];
    file.read(header,3);

    cout << header[0] << header[1] << header[2] << endl;
    if(header[0] == 'P' and header[1] == '4' and header[2] == 'A')
    {
        cout << "Correct archive format!" << endl;
        ///reading

        ///8 bits = 1 byte, 0-255 (0-FF)
        ///signed 8 bit integer: -127 - 128
        ///unsigned 8 bit integer: 0 - 255
        /// file version - 01
        /// unsigned - 1
        /// signed - -126

        ///16 bits = 2 bytes, 0 - 65535

        uint8_t file_version;

        file.seekg(3);
        file.read(reinterpret_cast<char*>(&file_version), 1);

        if(file_version == 1)
        {
            cout << "Detected Archive version 1" << endl;

            uint32_t file_dictionary_size;

            file.seekg(4);
            file.read(reinterpret_cast<char*>(&file_dictionary_size), 4);

            cout << "Dictionary size is: " << file_dictionary_size << endl;

            int file_data_pointer = file_dictionary_size + 8;

            p4a_offset = 8;
            vector<string> file_names;
            vector<int> file_offsets;
            vector<int> file_sizes;

            while(p4a_offset < file_data_pointer)
            {
                uint8_t filename_length;

                file.seekg(p4a_offset);
                file.read(reinterpret_cast<char*>(&filename_length), 1);
                p4a_offset += 1;

                file.seekg(p4a_offset);
                char file_name[filename_length];
                file.read(file_name,filename_length);

                string str_filename(file_name);
                str_filename = str_filename.substr(0,filename_length);

                cout << "Filename: " << str_filename << endl;
                in_fnames.push_back(str_filename);

                p4a_offset += filename_length;

                uint32_t file_offset;

                file.seekg(p4a_offset);
                file.read(reinterpret_cast<char*>(&file_offset), 4);
                cout << "File offset: " << file_offset << endl;
                in_foffsets.push_back(file_offset);

                p4a_offset += 4;

                uint32_t file_size;

                file.seekg(p4a_offset);
                file.read(reinterpret_cast<char*>(&file_size), 4);
                cout << "File size: " << file_size << endl;
                in_fsizes.push_back(file_size);

                p4a_offset += 4;
            }
        }
        else
        {
            cout << "Incorrect file version!" << endl;
        }
    }
    else
    {
        cout << "Incorrect file type" << endl;
        ///exit
    }

    file.close();
}

std::string P4A::ReadToMemory(std::string name)
{
    cout << "Reading " << name << " from memory" << endl;

    for(int i=0; i<in_fnames.size(); i++)
    {
        if(in_fnames[i] == name)
        {
            cout << "File found, open buffer" << endl;

            ifstream p4(p4a_filename, ios::binary);
            cout << "Buffer size: " << in_fsizes[i] << " bytes" << endl;
            string buffer;
            buffer.resize(in_fsizes[i]);
            cout << "Allocated char, size: " << buffer.size() << endl;

            p4.seekg(in_foffsets[i]);
            p4.read(&buffer[0],in_fsizes[i]);
            cout << "Buffer filled" << endl;

            p4.close();
            cout << "Reading successful" << endl;
            return string(buffer);
        }
    }

    cout << "Can't find file to read from" << endl;
    return "";
}

void P4A::Extract(std::string name)
{
    for(int i=0; i<in_fnames.size(); i++)
    {
        if(in_fnames[i] == name)
        {
            ifstream p4(p4a_filename, ios::binary);
            char buffer[in_fsizes[i]];
            p4.seekg(in_foffsets[i]);
            p4.read(buffer,in_fsizes[i]);
            p4.close();


            ofstream p4o(in_fnames[i], ios::binary | ios::trunc);
            p4o.write(buffer,in_fsizes[i]);
            p4o.close();
        }
    }
}
