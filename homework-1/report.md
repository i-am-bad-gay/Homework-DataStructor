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



### 解題策略

1. 把每一個子集合都當成岔路，先走到路的底端之後再到岔路口，再從岔路口出發



## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
using namespace std;

void powerSet(string s, int i, string c)
{
    
    if (i == s.size())  //走到底就執行 沒到底就繼續往下繼續走
    {
        cout << "(";
        for (int i = 0; i < c.size(); i++)
        {
            cout << c[i];
            if (i != c.size() - 1)
                cout << ",";
        }
        cout << "),";
        return;
    }

    powerSet(s, i + 1, c);  //一直往下走 順便讓 i 記錄往下走了幾次 

    c+= s[i];  //走到底之後 回來 選擇一個字元
    powerSet(s, i+1, c); //帶著字元走到底
}


int main()
{
    string input;
    getline(cin, input); 
    string a;

    for (char c : input)  //分割逗號
    {  
        if (c != ',')  
        {      
            a += c;
        }
    }

    cout<<"powerSet={ ";
    powerSet(a,0,"");
    
    cout << " }";
}
```

## 效能分析
時間複雜度:O(2^n)
空間複雜度:O(n)

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | $a,b,c  $    | (),(c),(b),(b,c),(a),(a,c),(a,b),(a,b,c)       | (),(c),(b),(b,c),(a),(a,c),(a,b),(a,b,c)        |
| 測試二   | $e,g,1  $     | (),(1),(g),(g,1),(e),(e,1),(e,g),(e,g,1)        | (),(1),(g),(g,1),(e),(e,1),(e,g),(e,g,1)        |
| 測試三   | $aaa,bb,cc  $        | (),(aaa),(bb),(cc),(aaa,bb),(aaa,cc),(bb,cc),(aaa,bb,cc)       | (),(c),(c),(c,c),(b),(b,c),(b,c),(b,c,c),(b),(b,c),(b,c),(b,c,c),(b,b),(b,b,c),(b,b,c),(b,b,c,c),(a),(a,c),(a,c),(a,c,c),(a,b),(a,b,c),(a,b,c),(a,b,c,c),(a,b),(a,b,c),(a,b,c),(a,b,c,c),(a,b,b),(a,b,b,c),(a,b,b,c),(a,b,b,c,c),(a),(a,c),(a,c),(a,c,c),(a,b),(a,b,c),(a,b,c),(a,b,c,c),(a,b),(a,b,c),(a,b,c),(a,b,c,c),(a,b,b),(a,b,b,c),(a,b,b,c),(a,b,b,c,c),(a,a),(a,a,c),(a,a,c),(a,a,c,c),(a,a,b),(a,a,b,c),(a,a,b,c),(a,a,b,c,c),(a,a,b),(a,a,b,c),(a,a,b,c),(a,a,b,c,c),(a,a,b,b),(a,a,b,b,c),(a,a,b,b,c),(a,a,b,b,c,c),(a),(a,c),(a,c).......... }       |



### 編譯與執行指令

```shell
$PS D:\Homework-DataStructor> ^C
$PS D:\Homework-DataStructor>
$PS D:\Homework-DataStructor>  & 'c:\Users\huang\.vscode\extensions\ms-vscode.$'--stdin=Microsoft-MIEngine-In-ogjpaxud.kws' $'--stdout=Microsoft-MIEngine-Out-1v2f4epm.xdu' $'--stderr=Microsoft-MIEngine-Error-wkzhdsg4.35t' $'--pid=Microsoft-MIEngine-Pid-dtcabuh4.rbk' '--dbgExe=D:\mingw64\bin\gdb.$exe' '--interpreter=mi'
$a,b,c
powerSet={ (),(c),(b),(b,c),(a),(a,c),(a,b),(a,b,c), }
```

### 結論

1. 這樣寫程式能正確的計算只有單個字元的子集合 。  
2. 但在element字元有>1的情況下，這個方法就不適用了。  

## 申論及開發報告

### 選擇遞迴的優點和缺點

在本程式中，使用遞迴的優點如下：

1. **程式邏輯簡單化**  
   相比於其他解法，遞迴更能簡單明瞭的寫出來，只寫出想選或不選的決策樹就好。 

1. **易於理解**
    比起迴圈或其他寫法，程式比較短，也蠻好理解的，容易看懂。 


在本程式中，使用遞迴的缺點如下：.

1. **遞迴的限制**  
<<<<<<< HEAD
   當element大於一定的數量的時候，就沒辦法用遞迴了。
=======
   當element大於一定的數量的時候，就沒辦法用遞迴了。
>>>>>>> 94be57b1b801c9e5bac7f12322ac6cff3b8f030d
