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