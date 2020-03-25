#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "hftree.h"
using namespace std;

class Utils
{
public:
    Utils();
    ~Utils();

    // load text dictionary file and form up the frequency table
    bool loadDictionary(const string& text_dic);

    // build huffman tree
    void buildTree();

    // set encoded table
    void setEncodedTable();

    // save bin_code into a binary file
    void saveBinDictionary(const string& bin_file);

    // decode a binary file to a plaintext file
    void decode(const string& bin_file);

private:
    // huffman tree
    HuffmanTree tree;

    // table that stores frequency of each character
    map<char, int> frequency_table;

    // result encoded table from huffman tree
    map<char, string> encoded_table;

    // content of whole file, read from disk file
    string content;
};

#endif // UTILS_H
