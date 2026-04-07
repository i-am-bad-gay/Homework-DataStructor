#include <iostream>
using namespace std;

template<class T>
class MinPQ{
    public:
        virtual ~MinPQ(){}
        virtual bool IsEmpty() const = 0; 
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
};

template <class T>
class MinHeap : public MinPQ<T> {
private:
    T* heap;
    int capacity;
    int heapSize;

public:
    // 建構子
    MinHeap(int cap = 10) : capacity(cap), heapSize(0) {
        heap = new T[capacity + 1];
    }
    //解構子
    ~MinHeap(){delete[] heap;}

    // 開始實作繼承下來的虛擬函式
    bool IsEmpty() const override {
        return heapSize == 0;
    }

    const T& Top() const override {
        // 這裡寫回傳根節點的邏輯
        return heap[1];
    }

    void Push(const T& x) override {
         
        if(heapSize == capacity){       // 1. 檢查 heapSize 是否滿了
            int newCapacity = capacity * 2;
            T* newHeap = new T[newCapacity + 1];
            for (int i = 1; i <= heapSize; i++) {
                newHeap[i] = heap[i];
            }
            delete[] heap;
            heap = newHeap;
            capacity = newCapacity;
            capacity=capacity*2-1;

        }else{                          // 3. 實作 Sift-up (向上與父節點比較並交換)
            int i = ++heapSize;
            heap[++heapSize]=x;         //  把 x 放進 heap 
;     
            while(i>0 && x<heap[i/2]){          
                heap[i]=heap[i/2];
                i=i/2;
            } 
        }
       
        
    }

    void Pop() override {
        if (IsEmpty()) return;

        // 拿最後一個元素補到根部
        T lastElement = heap[heapSize--];

        // Sift-down 邏輯
        int i = 1;      // 從根開始
        int child = 2;  // 左小孩
        while (child <= heapSize) {
            // 找出較小的小孩
            if (child < heapSize && heap[child + 1] < heap[child]) {
                child++;
            }
            // 如果最後一個元素比小孩都小，就定位了
            if (lastElement <= heap[child]) break;

            heap[i] = heap[child];
            i = child;
            child *= 2;
        }
        heap[i] = lastElement;
    }


};

#include <iostream>
#include <vector>

int main() {
    try {
        MinHeap<int> pq(5); // 初始容量設小一點，測試自動擴展

        // 1. 測試 Push (亂序輸入)
        cout << "--- 正在存入元素: 50, 10, 80, 20, 5, 30 ---" << endl;
        vector<int> data = {50, 10, 80, 20, 5, 30};
        for (int x : data) {
            pq.Push(x);
            cout << "存入 " << x << ", 目前最小值 (Top): " << pq.Top() << endl;
        }

        cout << "\n--- 開始取出元素 (Pop) ---" << endl;
        
        // 2. 測試 Pop (應該會從小到大輸出)
        while (!pq.IsEmpty()) {
            cout << "取出最小值: " << pq.Top() << endl;
            pq.Pop();
        }

    } catch (const char* msg) {
        cerr << "錯誤: " << msg << endl;
    }

    return 0;
}