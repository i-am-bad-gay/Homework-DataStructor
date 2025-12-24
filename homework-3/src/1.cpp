#include <iostream>
using namespace std;

/*************************************************
 * 題目說明：
 * 1. 實作 ChainNode (單向鏈結節點)
 * 2. 實作 Chain (單向鏈結串列)
 * 3. 實作 ChainIterator，讓 Chain 可以像 STL 一樣用 for 走訪
 * 4. 使用「循環鏈結串列」(Circular List) 實作 Polynomial
 *************************************************/

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

/*************************************************
 * main：測試程式
 *************************************************/
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
