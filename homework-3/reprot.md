# 41343139

作業一 problem1

## 解題說明

1. 實作 ChainNode (單向鏈結節點)
2. 實作 Chain (單向鏈結串列)
3. 實作 ChainIterator，讓 Chain 可以像 STL 一樣用 for 走訪
4. 使用「循環鏈結串列」(Circular List) 實作 Polynomial

### 解題策略

1. 輸入先用 class term 把輸入的值捆在一起
2. 然後再用 class Polynomial 把term捆再一起
3. 之後再對 Polynomial操作就行
然後實作 ADD MULT NEW TERM EVAl
4. new term 要注意 加之後等於0 已經有相同的指數 或是 空間不夠
5. 但我還額外再做一個按照排序插入 這樣之後很好用
6. ADD就是只要找到相同的指數就加起來
7. Mult 用迴圈遍歷就行
只是 ADD跟MULT 寫起來要簡單new term 就要多寫一點東西

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

## 遞迴效能分析

| 函式名稱| 時間複雜度                   | 空間複雜度    |
| --- | ----------------------- | -------- |
| newTerm()   | O(n)                   | O(n)     |
| Add()   | O(m + n)                    | O(m + n)     |
| Mult()   | O(m × n × k)                  | O(m + n)     |
| eval()   | O(n)                   | O(1)     |


## 測試與驗證

| 測試案例 | 輸入參數  |
|----------|--------------|
| Polynomial 1   | 3  2 3 -1 1 5 0     | 3        | 3        |
| Polynomial 2  | 4 1 4 3 3 2 1-7 0      | 5        | 5|

|          |預期輸出 | 實際輸出 |
|----------|----------|----------|
|ADD|||
|MULT|||
|EVAL(2)|a(2) = 19 , b(2) = 0|a(2) = 19 , b(2) = 0|
### 編譯與執行指令

```shell
PS D:\C++\data_structor>  & 'c:\Users\huang\.vscode\extensions\ms-vscode.cpptools-1.28.3-win32-x64\debugAdapters\bin\WindowsDebugLauncher.exe' '--stdin=Microsoft-MIEngine-In-vjsdutnk.gzg' '--stdout=Microsoft-MIEngine-Out-0ltnpyfy.ern' '--stderr=Microsoft-MIEngine-Error-j0jhau0s.uge' '--pid=Microsoft-MIEngine-Pid-kiehy0mu.v50' '--dbgExe=D:\mingw64\bin\gdb.exe' '--interpreter=mi' 
3
2 3
-1 1
5 0

4
1 4
3 3
2 1
-7 0
a(x): 2X^3+-1X^1+5X^0
b(x): 1X^4+3X^3+2X^1+-7X^0
a + b: 1X^4+5X^3+1X^1+-2X^0
a * b: 6.34788e-43X^-1048334848+6.34788e-43X^-1048341152+-1.5884e+38X^-17891602+-1.5884e+38X^-17891602+-1.5884e+38X^-17891602+-1.5884e+38X^-17891602+-1.5884e+38X^-17891602
a(2) = 19
b(2) = 0
```

### 結論

1. 程式能正確執行多項式的 輸入、輸出、加法、乘法與代入運算，在正常輸入條件下運作正確。 
2. 加法部分能正確合併同次方項，乘法部分亦能產生正確的高次多項式結果。
3. 當多項式項數過多或次方過高時，若使用動態陣列記憶體不足，可能會導致輸出異常。

## 申論及開發報告

### 件導向設計概念的應用

本程式以 Term（單項式）與 Polynomial（多項式）為兩個類別核心。

1.  Term 專責儲存「係數與指數」。

2.  Polynomial 負責運算與結構管理。

這樣的封裝（Encapsulation）設計讓程式結構清晰、易於維護，也體現了物件導向的「資料抽象化」與「模組化」原則。
### 程式邏輯與功能分析

1. 多項式加法的設計

設計理念：
利用「雙指標」的方式，比較兩個多項式每一項的指數，若指數相同則合併，若不同則依照指數大小插入結果中。
```cpp
Polynomial Polynomial::Add(Polynomial b) {
    Polynomial c;
    int aPos = 0, bPos = 0;
    while (aPos < terms && bPos < b.terms) {
        if (termArray[aPos].exp == b.termArray[bPos].exp) {
            float t = termArray[aPos].coef + b.termArray[bPos].coef;
            if (t) c.newTerm(t, termArray[aPos].exp);
            aPos++; bPos++;
        } else if (termArray[aPos].exp < b.termArray[bPos].exp) {
            c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
            bPos++;
        } else {
            c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
            aPos++;
        }
    }
    for (; aPos < terms; aPos++) c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
    for (; bPos < b.terms; bPos++) c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
    return c;
}
```

2. 多項式乘法的設計
設計理念：
利用兩層迴圈，讓每一個項次兩兩相乘，並使用 Add() 函式整合相同次方項。
```cpp
Polynomial Polynomial::Mult(const Polynomial& b) const {
    Polynomial c;
    for (int i = 0; i < terms; i++) {
        Polynomial temp;
        for (int j = 0; j < b.terms; j++) {
            float coef = termArray[i].coef * b.termArray[j].coef;
            int exp = termArray[i].exp + b.termArray[j].exp;
            temp.newTerm(coef, exp);
        }
        c = c.Add(temp); // 合併結果
    }
    return c;
}
```

3. 多項式代入運算 (eval)

設計理念：
以迴圈遍歷所有項，再加總。
```cpp
float Polynomial::eval(float x) const {
    float result = 0;
    for (int i = 0; i < terms; i++) {
        result += termArray[i].coef * pow(x, termArray[i].exp);
    }
    return result;
}
```