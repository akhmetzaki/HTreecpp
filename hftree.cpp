#include "hftree.h"



HuffmanTree::HuffmanTree() : hfTree(NULL) {}

char min(HuffmanTreeNode* root) {

    if (root->leftChild == nullptr)
        return root->key;

    char minLeft = min(root->leftChild);
    char minRight = min(root->rightChild);
    return minLeft < minRight ? minLeft : minRight;


}

struct minPriority {
    bool operator()(HuffmanTreeNode* first, HuffmanTreeNode* second) const {

        if (first->weight != second->weight)
            return first->weight > second->weight;


        return min(first) > min(second);
    }

};
void HuffmanTree::loadMap(map<char, int> frequency_table)
{
    
    priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, minPriority> myQ;
    map<char, int>::const_iterator it = frequency_table.begin();
    for (; it != frequency_table.end(); ++it) {
        HuffmanTreeNode* newNode = new HuffmanTreeNode;
        newNode->key = it->first;
        newNode->weight = it->second;
        myQ.push(newNode);
    }

    while (myQ.size() > 1) {

        HuffmanTreeNode* left = myQ.top();
        myQ.pop();

        HuffmanTreeNode* right = myQ.top();
        myQ.pop();

        HuffmanTreeNode* newNode = new HuffmanTreeNode;
        mergeTree(left, right, newNode);
        myQ.push(newNode);
    }
    hfTree = myQ.top();
    myQ.pop();
}

void HuffmanTree::mergeTree(HuffmanTreeNode *bt1, HuffmanTreeNode *bt2,
                       HuffmanTreeNode *pt)
{

    pt->leftChild = bt1;
    pt->rightChild = bt2;
    pt->key = '\0';
    pt->weight = bt1->weight + bt2->weight;
}


void HuffmanTree::encode(map<char, string>& encoded_table)
{
    
    queue<HuffmanTreeNode*> Q;
    Q.push(hfTree);

    while (!Q.empty()) {
        HuffmanTreeNode* temp = Q.front();
        if (temp->key != '\0') {
            encoded_table[temp->key] = temp->huffman_code;
        }
        Q.pop();

        if (temp->leftChild != nullptr) {
            Q.push(temp->leftChild);
            temp->leftChild->huffman_code = temp->huffman_code + "0";

            Q.push(temp->rightChild);
            temp->rightChild->huffman_code = temp->huffman_code + "1";
        }
    }

}

string HuffmanTree::decode(const string& bin_str)
{
    // decode a binary string to plaintext
    string plaintext = "";

    HuffmanTreeNode* temp = hfTree;

    for (int i = 0; i < bin_str.length(); ++i) {
        if (bin_str[i] == '1') {
            if (temp->rightChild != nullptr) {
                temp = temp->rightChild;
            }
            else {
                plaintext += temp->key;
                --i;
                temp = hfTree;
            }
        }

        else if (bin_str[i] == '0') {
            if (temp->leftChild != nullptr) {
                temp = temp->leftChild;
            }
            else {
                plaintext += temp->key;
                --i;
                temp = hfTree;
            }
        }
    }
    plaintext += temp->key;
    return plaintext;

}

void HuffmanTree::release()
{
    cout << "[start releasing the huffman tree...]" << endl;
    if (hfTree == NULL)
        return;

    // store the nodes in vector path with post-order first
    vector<HuffmanTreeNode*> path;

    // insert your code here ...
    stack<HuffmanTreeNode*> nodes;
    HuffmanTreeNode* temp = hfTree;
    do {
        while (temp != nullptr) {

            if (temp->rightChild != nullptr)
                nodes.push(temp->rightChild);
            nodes.push(temp);

            temp = temp->leftChild;
        }

        temp = nodes.top();
        nodes.pop();

        if (!nodes.empty()) {
            if (temp->rightChild != nullptr && nodes.top() == temp->rightChild) {
                nodes.pop();
                nodes.push(temp);
                temp = temp->rightChild;
            } else {
                path.push_back(temp);
                temp = nullptr;
            }
        }
        else {
            path.push_back(temp);
            temp = nullptr;
        }

    } while (!nodes.empty());

    for (int i = 0; i < path.size(); i++) {
        if (i != path.size() - 1)
            cout << dec << path[i]->weight << " ";
        else
            cout << dec << path[i]->weight << endl;

        // delete the node
        delete path[i];
    }
    cout << "[successfully released.]" << endl;
}

