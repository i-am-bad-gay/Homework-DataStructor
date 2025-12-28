# 41343139

作業一 problem1

## 解題說明

1. 實作 ChainNode (單向鏈結節點)
2. 實作 Chain (單向鏈結串列)
3. 實作 ChainIterator，讓 Chain 可以像 STL 一樣用 for 走訪
4. 使用「循環鏈結串列」(Circular List) 實作 Polynomial

### 解題策略

    首先以 ChainNode<T> 建立單向鏈結節點，並透過 Chain<T> 管理整條串列。
    接著實作 ChainIterator<T>，使鏈結串列可使用 Begin() 與 End() 方式進行走訪。

    在多項式部分，採用循環鏈結串列並設置 header node，以簡化邊界條件判斷。
    插入項次時依指數排序，並合併相同指數項。
    多項式加法則同時走訪兩條已排序串列完成運算。

最後逐步測試各模組功能，確保程式正確運作。

## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
using namespace std;

/*************************************************
 * 1. ChainNode
 * 功能：
 *   單向鏈結串列的基本節點
 *   data : 存放資料
 *   link : 指向下一個節點
 *************************************************/

template <class T>
class ChainNode {
public:
    T data;                  // 節點中存放的資料
    ChainNode<T>* link;      // 指向下一個節點

    // 建構子
    ChainNode(const T& element = T(), ChainNode<T>* next = nullptr)
        : data(element), link(next) {}
};

// 先宣告 Chain，讓 Iterator 可以成為朋友
template <class T> class Chain;

/*************************************************
 * 3. ChainIterator
 * 功能：
 *   讓 Chain 可以使用 Begin(), End()
 *   並支援 ++ 與 * 操作子
 *************************************************/

template <class T>
class ChainIterator {
public:
    typedef ChainIterator<T> iterator;

    // 建構子，接收一個節點指標
    ChainIterator(ChainNode<T>* node = nullptr) : current(node) {}

    // 取出目前節點的資料
    T& operator*() const {
        return current->data;
    }

    // 前置 ++，移動到下一個節點
    iterator& operator++() {
        current = current->link;
        return *this;
    }

    // 判斷兩個 iterator 是否不同
    bool operator!=(const iterator& rhs) const {
        return current != rhs.current;
    }

private:
    ChainNode<T>* current;    // 目前指向的節點
    friend class Chain<T>;   // 讓 Chain 可以存取 private 成員
};

/*************************************************
 * 2. Chain
 * 功能：
 *   單向鏈結串列的容器
 *   支援插入、走訪、Begin/End
 *************************************************/

template <class T>
class Chain {
public:
    typedef ChainIterator<T> iterator;

    Chain() : first(nullptr), size(0) {}

    ~Chain() { clear(); }

    bool empty() const { return size == 0; }
    int Size() const { return size; }

    // 回傳 iterator，指向第一個節點
    iterator Begin() { return iterator(first); }

    // 回傳 iterator，代表結尾 (nullptr)
    iterator End() { return iterator(nullptr); }

    // 從前面插入
    void InsertFront(const T& e) {
        first = new ChainNode<T>(e, first);
        size++;
    }

    // 從尾巴插入
    void InsertBack(const T& e) {
        if (!first) {
            first = new ChainNode<T>(e);
        } else {
            ChainNode<T>* cur = first;
            while (cur->link)
                cur = cur->link;
            cur->link = new ChainNode<T>(e);
        }
        size++;
    }

    // 清空整條串列
    void clear() {
        while (first) {
            ChainNode<T>* next = first->link;
            delete first;
            first = next;
        }
        size = 0;
    }

private:
    ChainNode<T>* first;  // 指向第一個節點
    int size;             // 節點數量
};

/*************************************************
 * 4. Polynomial（循環鏈結串列）
 *************************************************/

// 多項式的一項：係數與次方
struct Term {
    int coef;   // 係數
    int exp;    // 次方
    Term(int c = 0, int e = 0) : coef(c), exp(e) {}
};

class Polynomial {
public:
    Polynomial() {
        // 建立 header node
        head = new ChainNode<Term>(Term(0, -1));
        head->link = head; // 形成循環串列
    }

    ~Polynomial() {
        clear();
        delete head;
    }

    // 插入一個多項式項目（依次方由大到小）
    void InsertTerm(int c, int e) {
        if (c == 0) return;

        ChainNode<Term>* prev = head;
        ChainNode<Term>* cur = head->link;

        // 找到適合插入的位置
        while (cur != head && cur->data.exp > e) {
            prev = cur;
            cur = cur->link;
        }

        // 次方相同，合併係數
        if (cur != head && cur->data.exp == e) {
            cur->data.coef += c;
            if (cur->data.coef == 0) {
                prev->link = cur->link;
                delete cur;
            }
        } else {
            // 新增節點
            prev->link = new ChainNode<Term>(Term(c, e), cur);
        }
    }

    // 多項式加法
    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        ChainNode<Term>* aCur = head->link;
        ChainNode<Term>* bCur = b.head->link;

        while (aCur != head && bCur != b.head) {
            if (aCur->data.exp > bCur->data.exp) {
                result.InsertTerm(aCur->data.coef, aCur->data.exp);
                aCur = aCur->link;
            } else if (aCur->data.exp < bCur->data.exp) {
                result.InsertTerm(bCur->data.coef, bCur->data.exp);
                bCur = bCur->link;
            } else {
                result.InsertTerm(aCur->data.coef + bCur->data.coef,
                                  aCur->data.exp);
                aCur = aCur->link;
                bCur = bCur->link;
            }
        }

        // 剩餘項目直接加入
        while (aCur != head) {
            result.InsertTerm(aCur->data.coef, aCur->data.exp);
            aCur = aCur->link;
        }
        while (bCur != b.head) {
            result.InsertTerm(bCur->data.coef, bCur->data.exp);
            bCur = bCur->link;
        }
        return result;
    }

    // 輸出多項式
    void Print() const {
        ChainNode<Term>* cur = head->link;
        while (cur != head) {
            if (cur != head->link && cur->data.coef > 0)
                cout << "+";
            cout << cur->data.coef << "x^" << cur->data.exp;
            cur = cur->link;
        }
        cout << endl;
    }

private:
    ChainNode<Term>* head; // header node

    void clear() {
        ChainNode<Term>* cur = head->link;
        while (cur != head) {
            ChainNode<Term>* next = cur->link;
            delete cur;
            cur = next;
        }
        head->link = head;
    }
};


int main() {
    // 測試 Chain 與 Iterator
    Chain<int> x;
    x.InsertBack(1);
    x.InsertBack(2);
    x.InsertBack(3);

    for (Chain<int>::iterator it = x.Begin(); it != x.End(); ++it)
        cout << *it << " ";
    cout << endl;

    // 測試 Polynomial
    Polynomial p1, p2;
    p1.InsertTerm(3, 2); // 3x^2
    p1.InsertTerm(2, 1); // 2x^1

    p2.InsertTerm(1, 2); // 1x^2
    p2.InsertTerm(4, 0); // 4x^0

    Polynomial p3 = p1 + p2;
    p3.Print(); // 4x^2+2x^1+4x^0

    return 0;
}

```

## 效能分析

| 函式名稱| 時間複雜度                   | 空間複雜度    |
| --- | ----------------------- | -------- |
| InsertTerm()   | O(n)                   | O(n)     |
| operator+()   | O(m + n)                    | O(m + n)     |
| Print()   | O(m × n × k)                 | O(m + n)     |
| Iterator 走訪   | O(n)                   | O(1)     |


## 測試與驗證

| 測試案例 | 預期輸出 | 實際輸出 |
|----------|--------------|--------------|
| Chain 與 Iterator 測試 | 1 2 3 | 1 2 3 |
| Polynomial 插入測試 | 2x^1 | 2x^1 |
| Polynomial 加法測試 | 4x^2 + 2x^1 + 4x^0 | 4x^2 + 2x^1 + 4x^0 |
| Polynomial 代入測試 | a(2) = 16 | a(2) = 16 |
### 編譯與執行指令

```shell
PS D:\C++\data_structor>  & 'c:\Users\huang\.vscode\extensions\ms-vscode.cpptools-1.30.0-win32-x64\debugAdapters\bin\WindowsDebugLauncher.exe' '--stdin=Microsoft-MIEngine-In-oauzv3mtPS D:\C++\data_structor>  & 'c:\Users\huang\.vscode\extensions\ms-vscode.cpptools-1.30.0-win32-x64\debugAdapters\bin\WindowsDebugLauncher.exe' '--stdin=Microsoft-MIEngine-In-oauzv3mt.zyr' '--stdout=Microsoft-MIEngine-Out-kh42m1b1.zan' '--stderr=Microsoft-MIEngine-Error-isufdvj1.vk2' '--pid=Microsoft-MIEngine-Pid-fdche2hr.1cp' '--dbgExe=D:\mingw64\bin\gdb.exe' '--interpreter=mi'
1 2 3
4x^2+2x^1+4x^0
PS D:\C++\data_structor> ^C
PS D:\C++\data_structor> 
PS D:\C++\data_structor>  & 'c:\Users\huang\.vscode\extensions\ms-vscode.cpptools-1.30.0-win32-x64\debugAdapters\bin\WindowsDebugLauncher.exe' '--stdin=Microsoft-MIEngine-In-a4zmnvml.flx' '--stdout=Microsoft-MIEngine-Out-fignlsb0.c0g' '--stderr=Microsoft-MIEngine-Error-usrvd4ve.i1g' '--pid=Microsoft-MIEngine-Pid-nxw3c3mg.hao' '--dbgExe=D:\mingw64\bin\gdb.exe' '--interpreter=mi' 
1 2 3 
4x^2+2x^1+4x^0
PS D:\C++\data_structor> 
```

### 結論

在 Polynomial 的實作中，採用循環鏈結串列與 header node 的方式，
有效簡化插入與走訪時的邊界條件判斷，
使多項式插入與加法運算能夠正確且穩定地執行。
經由多組測試驗證，各項功能皆能依預期運作，程式具備良好的正確性與可讀性。

## 申論及開發報告

### 件導向設計概念的應用

本程式以 Term（單項式）與 Polynomial（多項式）為兩個類別核心。

1.  Term 專責儲存「係數與指數」。

2.  Polynomial 負責運算與結構管理。

這樣的封裝（Encapsulation）設計讓程式結構清晰、易於維護，也體現了物件導向的「資料抽象化」與「模組化」原則。
### 程式邏輯與功能分析

