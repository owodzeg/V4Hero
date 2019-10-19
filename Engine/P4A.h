#ifndef P4A_H
#define P4A_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>

class P4A
{
    public:
    ///For output
    std::vector<std::string> original_path;
    std::vector<std::string> filenames;
    std::vector<int> filesizes;

    std::vector<char> output_header;
    std::vector<char> output_dictionary;

    int dictionary_size = 8;
    long file_offset = 0;
    long file_size = 0;
    int p4a_offset = 0;

    ///For input
    std::string p4a_filename;

    std::vector<std::string> in_fnames;
    std::vector<int> in_fsizes;
    std::vector<int> in_foffsets;

    P4A();

    ///Packing functions
    int GetFileSize(std::string filename);
    void LoadFile(std::string filename);
    void CreateDictionary();
    void SaveToFile(std::string filename);
    void DataToImage();
    void EncryptImage();

    ///Unpacking functions
    void ReadDictionary(std::string filename);
    std::string ReadToMemory(std::string name);
    void Extract(std::string name);
    void ImageToData();
    void DecryptImage();
};

#endif // P4A_H
