# 41343139

# 作業一 problem1

## 解題說明
使用 Min-Heap (最小堆積) 來實作優先權佇列 (Priority Queue)。

### 解題策略

Push:將新元素置於陣列末端，透過 Sift-up (向上過濾) 與父節點比較並交換，確保父節點永遠小於子節點。

POP:
1.將根節點（最小值）取出。
2.將陣列最後一個元素移至根部。
3.執行 Sift-down (向下過濾)：與左右小孩中較小者比較並交換，直到滿足堆積特性或到達葉節點。

## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
#include <algorithm>

using namespace std;

// 抽象父類
template<class T>
class MinPQ {
public:
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T& x) = 0;
    virtual void Pop() = 0;
};

template<class T>
class MinHeap : public MinPQ<T> {
private:
    T *heap;
    int capacity;
    int heapsize;

    // 動態擴展空間
    void resize() {
        int newCapacity = capacity * 2;
        T* newHeap = new T[newCapacity + 1];
        for (int i = 1; i <= heapsize; i++) {
            newHeap[i] = heap[i];
        }
        delete[] heap;
        heap = newHeap;
        capacity = newCapacity;
    }

public:
    MinHeap(int cap = 10) : capacity(cap), heapsize(0) {
        heap = new T[capacity + 1];
    }

    ~MinHeap() { delete[] heap; }

    bool IsEmpty() const override {
        return heapsize == 0;
    }

    const T& Top() const override {
        if (IsEmpty()) throw "Heap is empty";
        return heap[1];
    }

    void Push(const T& x) override {
        if (heapsize == capacity) {
            resize();
        }

        // Sift-up 向上調整
        int i = ++heapsize;
        while (i > 1 && x < heap[i / 2]) {
            heap[i] = heap[i / 2];
            i /= 2;
        }
        heap[i] = x;
    }

    void Pop() override {
        if (IsEmpty()) return;

        // 拿最後一個元素補到根部
        T lastElement = heap[heapsize--];

        // Sift-down 向下調整
        int i = 1;      // 從根開始
        int child = 2;  // 左小孩位置
        while (child <= heapsize) {
            // 找出較小的小孩 (如果有右小孩且右小孩更小)
            if (child < heapsize && heap[child + 1] < heap[child]) {
                child++;
            }
            
            // 如果最後一個元素比小孩都小，就定位了
            if (lastElement <= heap[child]) break;

            heap[i] = heap[child];
            i = child;
            child *= 2;
        }
        if (heapsize >= 0) heap[i] = lastElement;
    }
};

int main() {
    
        MinHeap<int> pq;

        cout << "輸入:" << endl;
        int a;
        while (cin >> a) {
            pq.Push(a);
            cout << "存入 " << a << ", 目前最小值 (Top): " << pq.Top() << endl;
        }

        cout << "\n依序取出元素 (從小到大):" << endl;
        while (!pq.IsEmpty()) {
            cout << "取出: " << pq.Top() << endl;
            pq.Pop();
        }

    
    return 0;
}

```

## 遞迴效能分析

Push時間複雜度:$O(\log n)$
Pop時間複雜度:$O(\log n)$



## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | 8 5 7 6 2 4 3 1 9       | 1 2 3 4 5 6 7 8 9        | 1 2 3 4 5 6 7 8 9   |


### 編譯與執行指令

```shell
PS D:\C++\data_structor\Homework-DataStructor>  & 'c:\Users\huang\.vscode\extensions\ms-vscode.cpptools-1.31.3-win32-x64\debugAdapters\bin\WindowsDebugLauncher.exe' '--stdin=Microsoft-MIEngine-In-y1vcxylj.5zy' '--stdout=Microsoft-MIEngine-Out-ihpqckuh.ptt' '--stderr=Microsoft-MIEngine-Error-kygl20xh.z4d' '--pid=Microsoft-MIEngine-Pid-mxmsqz1p.bh5' '--dbgExe=D:\mingw64\bin\gdb.exe' '--interpreter=mi'
輸入:
8 5 7 6 2 4 3 1 9 
存入 8, 目前最小值 (Top): 8
存入 5, 目前最小值 (Top): 5
存入 7, 目前最小值 (Top): 5
存入 6, 目前最小值 (Top): 5
存入 2, 目前最小值 (Top): 2
存入 4, 目前最小值 (Top): 2
存入 3, 目前最小值 (Top): 2
存入 1, 目前最小值 (Top): 1
存入 9, 目前最小值 (Top): 1
^Z  

依序取出元素 (從小到大):
取出: 1
取出: 2
取出: 3
取出: 4
取出: 5
取出: 6
取出: 7
取出: 8
取出: 9
```

### 結論
1. 透過 8 5 7 6 2 4 3 1 9 的輸入測試，程式能即時透過 Top() 回傳當前最小值，且最後輸出有達到min-heap的要求。

2. 本實作採用陣列（Array-based）存儲二元樹，相較於鏈結串列，具有更好的 記憶體連續性（Locality of Reference）。Push 與 Pop 的時間複雜度均穩定在 $O(\log n)$，適合處理大量動態資料

## 申論及開發報告

### min heap的優點
1. 對於需要頻繁獲取最小值的應用（如：實作優先權佇列），效率極高。
2. 不論是新增元素還是移除最小值，重新調整堆積結構的時間複雜度僅為對數等級。
3. Min-Heap 通常使用完全二元樹的結構，這意味著我們可以用連續的陣列（Array）來儲存，不需要像鏈結串列那樣額外儲存指標。
4. 父子節點的關係可以用簡單的索引公式計算（父節點為 $i/2$，左小孩 $2i$，右小孩 $2i+1$），程式邏輯清晰，易於維護

### min heap的缺點
1. 堆積只保證父小於子，並不保證左小於右。如果你想在 Min-Heap 中找一個特定的值，必須要掃描整個陣列，效率遠低於二元搜尋樹。
2. 堆積並不是一個完全排序的結構。雖然它能保證根部是最小，但同一層的兄弟節點之間沒有特定順序。如果你需要資料隨時保持「由小到大」的完整排列，Min-Heap 無法直接辦到，必須不斷執行 Pop 操作才能得到排序結果。





   




# 作業一 problem2

## 解題說明
驗證隨機產生的樹高是否符合理論上的 $O(\log n)$ 成長


### 解題策略

1. 結構設計：使用 class Node 封裝資料與指標，並建立 class BST 類別管理整棵樹。
2.  左小右大規則：插入新節點時，從根節點開始比較。比目前節點小往左走，比目前節點大往右走。
3. 高度測量：使用遞迴法從底層向上回傳高度（$1 + \max(\text{左高}, \text{右高})$）。
4. 刪除邏輯：

    無小孩/單小孩：直接將子節點（或 nullptr）接上。
    雙小孩：尋找「右子樹的最小值」替換目前節點的值，再刪除該最小值節點。


## 程式實作

以下為主要程式碼：

```cpp
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
```

## 效能分析
平均情況 (隨機插入)：$O(\log n)$。如實驗數據所示，樹的高度趨於平衡。
空間複雜度：$O(n)$。每個資料點對應一個 Node 物件空間。
## 測試與驗證

### 測試案例

| 測試案例 | | |  |  |
|----------|----------|--------------|----------|----------|
|  |n      | H      | log2n  | H/log2n|
|  |100   |  12    |  6     |  1.80618|
|  |500   |  19   |   8   |    2.11917|
|  |1000  |  21   |   9   |    2.10721|
|  |2000   |   23    |  10  |    2.09743|
|  |3000   | 28   |   11   |   2.42409|
|  |5000   | 24   |   12   |   1.95317|
|  |10000  | 30   |   13   |   2.25772|



### 編譯與執行指令

```shell
PS D:\C++\data_structor\Homework-DataStructor>  & 'c:\Users\huang\.vscode\extensions\ms-vscode.cpptools-1.31.4-win32-x64\debugAdapters\bin\WindowsDebugLauncher.exe' '--stdin=Microsoft-MIEngine-In-v1ju15qi.f51' '--stdout=Microsoft-MIEngine-Out-cwske12i.vs3' '--stderr=Microsoft-MIEngine-Error-2fiqxuxi.ucr' '--pid=Microsoft-MIEngine-Pid-yndlwx1k.nl4' '--dbgExe=D:\mingw64\bin\gdb.exe' '--interpreter=mi' 
n       H       log2n   H/log2n
------------------------------------
100     12      6       1.80618
500     19      8       2.11917
1000    21      9       2.10721
2000    23      10      2.09743
3000    28      11      2.42409
5000    24      12      1.95317
10000   30      13      2.25772
```

### 結論



## 申論及開發報告

### 使用 BST 的優點

1. 動態維護：相較於排序陣列（Sorted Array），BST 在執行插入與刪除時不需要移動大量的記憶體元素，僅需調整指標，靈活性極高。
2. 中序走訪優勢：BST 具有天然的排序特性。若執行中序走訪（In-order Traversal），可以在 $O(n)$ 時間內獲得由小到大的排序結果。
3. 搜尋效率高：在平均情況下，搜尋效率與二分搜尋法相當

 
### 使用 BST 的缺點
1.每個節點需要儲存兩個額外的指標空間，相較於 Heap（使用陣列實作），記憶體佔用較多。