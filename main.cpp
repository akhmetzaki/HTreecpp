#include <iostream>

#include "utils.h"
#include "hftree.h"
using namespace std;

int main()
{
    Utils ut;
    if (ut.loadDictionary("/Users/yerke/CLionProjects/pa4/paper.txt")) {
        ut.buildTree();
        ut.setEncodedTable();
        cout << "[start compressing file as encode_paper.bin ...]" << endl;
        ut.saveBinDictionary("/Users/yerke/CLionProjects/pa4/encode_paper.bin");
        cout << "[compressed file encode_paper.bin saved. ]" << endl << endl;

        cout << "[start decoding...]" << endl;
        ut.decode("/Users/yerke/CLionProjects/pa4/encode_paper.bin");
    }
    return 0;
}
