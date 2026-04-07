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



template<class T>
class MinHeap :public MinPQ{
private:
	T *heap;
	int capacity;
	int heapsize;

public:
	MinHeap(int cap=10):capacity(cap),heapsize(0)
	{
		heap=new T[capacity+1];
	}

	~MinHeap(){delete []heap;}

	bool IsEmpty() const override
	{
		return heapsize==0;
	}

	T top()
	{
		
	}

};