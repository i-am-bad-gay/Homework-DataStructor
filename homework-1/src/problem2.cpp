#include <iostream>
#include <algorithm>
#include <cmath>
#include <ctime>

using namespace std;

// 定義節點
class Node {
public:
    int key;
    Node *left, *right;

    // 建構子
    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

class BST {
private:
    Node* root;

    
    Node* insert(Node* node, int k) {
        if (!node) return new Node(k);
        if (k < node->key) node->left = insert(node->left, k);
        else if (k > node->key) node->right = insert(node->right, k);
        return node;
    }

    int getHeight(Node* node) {
        if (!node) return 0;
        return 1 + max(getHeight(node->left), getHeight(node->right));
    }

    Node* deleteNode(Node* node, int k) {
        if (!node) return nullptr;
        if (k < node->key) node->left = deleteNode(node->left, k);
        else if (k > node->key) node->right = deleteNode(node->right, k);
        else {
            if (!node->left) { Node* t = node->right; delete node; return t; }
            if (!node->right) { Node* t = node->left; delete node; return t; }
            Node* temp = node->right;
            while (temp->left) temp = temp->left;
            node->key = temp->key;
            node->right = deleteNode(node->right, temp->key);
        }
        return node;
    }

public:
    BST() : root(nullptr) {}

    // 外部呼叫的介面 (Public)
    void insert(int k) { root = insert(root, k); }
    int getHeight() { return getHeight(root); }
    void remove(int k) { root = deleteNode(root, k); }
};

int main() {
    srand(time(NULL));
    int n_list[] = {100, 500, 1000, 2000, 3000, 5000, 10000};

    cout << "n\tH\tlog2n\tH/log2n" << endl;
    cout << "------------------------------------" << endl;

    for (int n : n_list) {
        BST tree; // 每次循環建立一棵新樹
        for (int i = 0; i < n; i++) {
            tree.insert(rand() % 1000000);
        }

        int h = tree.getHeight();
        double log2n = log2(n);
        
        cout << n << "\t" << h << "\t" << (int)log2n << "\t" << h / log2n << endl;
    }

    return 0;
}