#include <iostream>

#include "utils.h"
#include "hftree.h"
using namespace std;

int main()
{
    Utils ut;
    if (ut.loadDictionary("/*Your directory here*/paper.txt")) {
        ut.buildTree();
        ut.setEncodedTable();
        cout << "[start compressing file as encode_paper.bin ...]" << endl;
        ut.saveBinDictionary("/*Your directory here*/encode_paper.bin");
        cout << "[compressed file encode_paper.bin saved. ]" << endl << endl;

        cout << "[start decoding...]" << endl;
        ut.decode("/*Your directory here*/encode_paper.bin");
    }
    return 0;
}
