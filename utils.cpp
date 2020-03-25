#include "utils.h"

Utils::Utils()  {}

Utils::~Utils()
{
    tree.release();
}

bool Utils::loadDictionary(const string& text_dic)
{
    // load the file with name text_dic, and save all characters in this->content
    ifstream load_file(text_dic);
    if (!load_file.is_open()) {
        cout << "Failed to load the article. Exit." << endl;
        return false;
    }

    ostringstream buf;
    char ch;
    while (buf && load_file.get(ch))
        buf.put(ch);
    this->content = buf.str();
    // move out the end useless '\r' of a file
    this->content.pop_back();

    load_file.close();

    // scan the string this->content, calculate the frequency table
    for (int i = 0; i < this->content.length(); ++i) {
            frequency_table[this->content[i]]++;
    }

    return true;
}

void Utils::buildTree()
{
    tree.loadMap(frequency_table);
}

void Utils::setEncodedTable()
{
    tree.encode(encoded_table);
}

void Utils::saveBinDictionary(const string& bin_file)
{
    // load key file
    ifstream ifile("/Users/yerke/CLionProjects/pa4/xor_key.bin", ios::binary);
    if (!ifile.is_open()) {
        cout << "cannot load key file, exit." << endl;
        return;
    }

    ofstream ofile(bin_file, ios::binary);

    string bin_code = "";
    string raw = "";
    for (int i = 0; i < content.length(); ++i)
        raw += encoded_table[content[i]];

    string begin = "";
    int cutoff = raw.length() % 8;

    begin = "00000";
    int div = 4;
    int temp = cutoff;
    while (div > 0) {
        if (temp/div)
            begin += '1';
        else
            begin += '0';
        temp %= div;
        div /= 2;
    }


    bin_code = begin + raw;
    if (cutoff != 0) {
        for (int i = 0; i < 8 - cutoff; ++i)
            bin_code += '0';
    }



    int ch_code = 0;
    char ch_key;
    for (int i = 0; i < bin_code.length(); ++i) {
        if (bin_code[i] == '1') {
            int temp = 1;
            int count = 7 - i%8;
            while(count > 0) {
                temp *= 2;
                --count;
            }
            ch_code += temp;
        }
        if ((i + 1)%8 == 0) {
            ifile.get(ch_key);
            unsigned char encoded = ch_code ^ ch_key;
            cout << hex << (int)encoded;
            ofile << encoded;
            ch_code = 0;

        }
    }

    cout << endl << endl;

    ifile.close();
    ofile.close();

}


void swap(char& c1, char& c2) {
    char temp = c1;
    c1 = c2;
    c2 = temp;
}

void reverse(string& toReverse) {
    int n = toReverse.length();

    for (int i = 0; i < n/2; ++i)
        swap(toReverse[i], toReverse[n - i - 1]);
}

void Utils::decode(const string& bin_file)
{
    ifstream ifile(bin_file, ios::binary);
    if (!ifile.is_open()) {
        cout << "cannot open .bin file, stop decoding." << endl;
        return;
    }


    ifstream key_file("/Users/yerke/CLionProjects/pa4/xor_key.bin", ios::binary);
    if (!key_file.is_open()) {
        cout << "cannot load key file, exit." << endl;
        return;
    }


    // the string used to search on huffman tree to decode as plaintext
    string bit_str = "";

    // bin_file: stores a binary huffman code with possible extra bits at the end
    // key_file: decryption XOR key
    char ch_code;
    char ch_key;
    unsigned char decoded;
    bool flag = true;
    int cutoff = 0;

    string raw = "";
    while (ifile.get(ch_code)) {
        key_file.get(ch_key);
        decoded = (ch_code ^ ch_key);
        int temp = (int)decoded;
        if (flag) {
            cutoff = temp;
            flag = false;
        }
        else {
            string inverse = "";
            while (temp > 0) {
                if (temp % 2)
                    inverse += "1";
                else
                    inverse += "0";

                temp /= 2;
            }
            reverse(inverse);
            if (inverse.length() != 8) {
                string zeroes = "";
                for (int i = 0; i < 8 - inverse.length(); ++i)
                    zeroes += '0';

                inverse = zeroes + inverse;

            }
            raw += inverse;
        }
    }

    ifile.close();
    key_file.close();
    if (cutoff != 0) {
        for (int i = 0; i < raw.length() - 8 + cutoff; ++i)
            bit_str += raw[i];
    }
    else
        bit_str = raw;
    // using huffman tree you've built before to decode the bit string
    string plaintext = tree.decode(bit_str);
    cout << plaintext << endl << endl;
}
