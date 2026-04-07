#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <ctime>

using namespace std;

// 定義節點
struct Node {
    int key;
    Node *left, *right;
    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

// 插入函式 (簡單遞迴)
Node* insert(Node* node, int k) {
    if (!node) return new Node(k);
    if (k < node->key) node->left = insert(node->left, k);
    else if (k > node->key) node->right = insert(node->right, k);
    return node;
}

// 計算高度
int getHeight(Node* node) {
    if (!node) return 0;
    return 1 + max(getHeight(node->left), getHeight(node->right));
}

// (b) 刪除函式：時間複雜度 O(h)，平均 O(log n)
Node* deleteNode(Node* root, int k) {
    if (!root) return nullptr;
    if (k < root->key) root->left = deleteNode(root->left, k);
    else if (k > root->key) root->right = deleteNode(root->right, k);
    else {
        // 情況 1 & 2: 只有一個小孩或沒小孩
        if (!root->left) { Node* t = root->right; delete root; return t; }
        if (!root->right) { Node* t = root->left; delete root; return t; }
        
        // 情況 3: 兩個小孩，找右子樹最小當替補
        Node* temp = root->right;
        while (temp->left) temp = temp->left;
        root->key = temp->key;
        root->right = deleteNode(root->right, temp->key);
    }
    return root;
}

int main() {
    srand(time(NULL)); // 初始化隨機數
    int n_list[] = {100, 500, 1000, 2000, 3000, 5000, 10000};

    cout << "n\tH\tlog2n\tH/log2n" << endl;
    cout << "------------------------------------" << endl;

    for (int n : n_list) {
        Node* root = nullptr;
        // 隨機插入 n 個數字
        for (int i = 0; i < n; i++) {
            root = insert(root, rand() % 1000000); 
        }

        int h = getHeight(root);
        double log2n = log2(n);
        
        cout << n << "\t" << h << "\t" << (int)log2n << "\t" << h / log2n << endl;
        
        // 這裡可以視需求清空記憶體，功課簡單寫的話這樣跑完就結束了
    }

    return 0;
}