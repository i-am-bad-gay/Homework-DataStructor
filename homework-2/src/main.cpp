#include <iostream>
#include <map>
#include <vector>
#include <list>   
using namespace std;

#define BigNum INTMAX_MAX

class Graph   //基本的模板
{
protected:
    int n;                                          //頂點數
    int e;                                          //邊數

public:
    virtual ~Graph(){};
    bool IsEmpty()const{return n==0;}

    int NumberOfVertices()const{return n;}
    int NumberOfEdges()const{return e;}

    virtual int Degree(int u)const=0;
    virtual bool ExistsEdge(int u,int v)const=0;
    virtual void InsertVertex(int v)=0;   
    virtual void InsertEdge(int u,int v,int cost)=0;
    virtual void DeleteVertex(int v)=0;
    virtual void DeleteEdge(int u,int v)=0;
};

class AdjacencyMatrix: public Graph   //相鄰陣列
{
private:
    vector<vector<int>> matrix;
    map<int, int> maap;         //對應頂點 ID 到矩陣索引 (0, 1, 2...)
public:
    AdjacencyMatrix()
    {
        n=0;  
        e=0;
        matrix.assign(n, vector<int>(n, BigNum));

    }

    ~AdjacencyMatrix(){}

    int Degree(int u)const override
    {
        int count;
        for(int i=0;i<n;i++)
        {
            if(matrix[u][i]==1)count++;
        }
        return count;
    }

    bool ExistsEdge(int u,int v)const override
    {
        return matrix[u][v]==1;
    }

   void InsertVertex(int v) override         // 插入 vertex
    {
        if (maap.find(v) == maap.end())       // 找不到v 代表v不存在 要新增
        {
            // 1. 原本的每一列（舊頂點），都在尾端補上一個 BigNum，代表它們跟新點不連通
            for (int i = 0; i < n; i++) {
                matrix[i].push_back(BigNum);
            }
            
            // 2. 建立映射：把頂點編號 v 對應到目前的矩陣索引 n
            maap[v] = n; 
            
            // 頂點數量加 1
            n++;
            
            // 3. 新增全新的一列（代表新點到所有人的距離）
            // 預設全部填 BigNum
            vector<int> newRow(n, BigNum);
            // 自己到自己的距離要設為 0
            newRow[n - 1] = 0; 
            
            matrix.push_back(newRow);
        } 
    }

    virtual void InsertEdge(int u, int v, int cost) override  // 插入 edge
    {   
    // 1. 先檢查這兩個頂點在 map 裡存不存在
    if (maap.find(u) == maap.end() || maap.find(v) == maap.end()) {
        return; // 如果有點不存在，就直接結束不處理
    }

    // 2. 透過 map 轉換：把頂點編號轉成矩陣的真正的 index
    int idx_u = maap[u];
    int idx_v = maap[v];

    // 3. 填入權重
    matrix[idx_u][idx_v] = cost;
    
    // 如果是「無向圖」，反向也要填喔！
    // matrix[idx_v][idx_u] = cost; 

    e++; // 邊的數量加 1
    }

    virtual void DeleteVertex(int v) override
    {   
    // 1. 安全檢查：如果點根本不存在，直接結束
    if (maap.find(v) == maap.end()) return;

    int indx = maap[v];
    int edgesToDelete = 0;
    
    // 2. 算一下這個頂點到底連了幾條邊（看它要刪除的那一列）
    // 只要不是無限大(BigNum)，且不是自己到自己(0)，就代表有一條邊
    for (int j = 0; j < n; j++) {
        if (matrix[indx][j] != BigNum && matrix[indx][j] != 0) {
            edgesToDelete++;
        }
    }
    
    // 3. 開始刪除記憶體結構
    matrix.erase(matrix.begin() + indx);  // 刪除第 indx 列 (Row)
    
    // 注意：因為上一行已經把 matrix 的長度減 1 了，所以現在的矩陣列數是 n-1
    for (int i = 0; i < n - 1; i++) {
        matrix[i].erase(matrix[i].begin() + indx);  // 刪除每一列的第 indx 個元素 (Column)
    }
    
    // 4. 更新字典（後面的 index 要全部往前一格
    for (auto it = maap.begin(); it != maap.end(); ++it)
    {
        if (it->second > indx)
        {
            it->second -= 1; 
        }
    }
    
    // 5. 更新圖的統計數據
    n--;            // 頂點數 - 1
    e -= edgesToDelete; // 扣掉實際刪除的邊數
    maap.erase(v);  // 把這個點從 map 裡徹底移除
}

    virtual void DeleteEdge(int u,int v)override
    {
        matrix[u][v]=BigNum;
        e--;
    }
};

class node 
{
public:
    int data;
    node* nex; 

    node(int d):data(d),nex(nullptr){}
    
};


bool check(vector<node*>& a, int b) {
    for (int i = 0; i < a.size(); i++) {
        
        if (a[i] != nullptr && a[i]->data == b) return true;
    }
    return false;
}

class AdjacencyList : public Graph {   //相鄰串列
private:
    vector<node*> liss; 

public:
    AdjacencyList() { n = 0; e = 0; }
    
    // 解構子：釋放所有 new 出來的 node，避免記憶體洩漏
    ~AdjacencyList() {
        for (node* head : liss) {
            while (head) {
                node* temp = head;
                head = head->nex;
                delete temp;
            }
        }
    }

    int Degree(int u) const override {
        if (u < 0 || u >= liss.size()) return 0;
        int count = 0;
        node* curr = liss[u];
        while (curr) {
            count++;
            curr = curr->nex;
        }
        return count;
    }

    bool ExistsEdge(int u, int v) const override {
        if (u < 0 || u >= liss.size()) return false;
        node* curr = liss[u];
        while (curr) {
            if (curr->data == v) return true;
            curr = curr->nex;
        }
        return false;
    }

    void InsertVertex(int v) override {
        // v作為索引，如果超出範圍就 resize
        if (v >= liss.size()) {
            liss.resize(v + 1, nullptr);
            n++;
        }
    }

    void InsertEdge(int u, int v) override {
        if (u >= liss.size() || v >= liss.size()) return;
        if (ExistsEdge(u, v)) return; // 避免重複插入相同的邊

        node* newNode = new node(v);
        
        newNode->nex = liss[u];
        liss[u] = newNode;
        e++;
    }

    // 刪除邊 (u, v)
    void DeleteEdge(int u, int v) override {
        if (u < 0 || u >= liss.size()) return;

        node* curr = liss[u];
        node* prev = nullptr;

        while (curr) {
            if (curr->data == v) {
                if (!prev) liss[u] = curr->nex; // 刪除的是頭
                else prev->nex = curr->nex;      // 刪除的是中間或尾巴
                
                delete curr;
                e--;
                return;
            }
            prev = curr;
            curr = curr->nex;
        }
    }

    // 刪除頂點 v
    void DeleteVertex(int v) override {
        if (v < 0 || v >= liss.size()) return;

        // 1. 釋放頂點 v 指向的所有鄰居節點
        node* curr = liss[v];
        while (curr) {
            node* temp = curr;
            curr = curr->nex;
            delete temp;
            e--;
        }
        liss[v] = nullptr;

        // 2. 遍歷其他所有頂點，刪除指向 v 的邊
        for (int i = 0; i < liss.size(); i++) {
            if (i == v) continue;
            DeleteEdge(i, v); 
        }
        n--;
    }
};

struct Edge {
    bool mark;          // 標記是否被走訪過
    int v1, v2;         // 這條邊連接的兩個頂點
    Edge *path1, *path2; // path1 是 v1 的下一條邊，path2 是 v2 的下一條邊

    Edge(int u, int v) : mark(false), v1(u), v2(v), path1(nullptr), path2(nullptr) {}
};

class AdjacencyMultilist : public Graph {       //相鄰多重串列
private:
    vector<Edge*> head; // 每個頂點的第一條邊

public:
    AdjacencyMultilist(int vertices = 0) {
        n = vertices; e = 0;
        head.assign(n, nullptr);
    }

    // 插入頂點
    void InsertVertex(int v) override {
        if (v >= head.size()) {
            head.resize(v + 1, nullptr);
            n++;
        }
    }

    // 插入邊 (u, v)
    void InsertEdge(int u, int v) override {
        if (u >= head.size() || v >= head.size()) return;
        
        // 1. 產生一個唯一的邊節點
        Edge* newEdge = new Edge(u, v);

        // 2. 處理 u 的鏈結：插在 u 的串列最前面
        newEdge->path1 = head[u];
        head[u] = newEdge;

        // 3. 處理 v 的鏈結：插在 v 的串列最前面
        newEdge->path2 = head[v];
        head[v] = newEdge;

        e++;
    }

    // 檢查邊是否存在
    bool ExistsEdge(int u, int v) const override {
        Edge* curr = head[u];
        while (curr != nullptr) {
            if ((curr->v1 == u && curr->v2 == v) || (curr->v1 == v && curr->v2 == u)) {
                return true;
            }
            // 判斷下一條邊要跟著 path1 還是 path2 走
            curr = (curr->v1 == u) ? curr->path1 : curr->path2;
        }
        return false;
    }

    // 計算頂點的度數
    int Degree(int u) const override {
        int count = 0;
        Edge* curr = head[u];
        while (curr != nullptr) {
            count++;
            curr = (curr->v1 == u) ? curr->path1 : curr->path2;
        }
        return count;
    }

    // 刪除邊 (邏輯較複雜，需考慮 prev 指標)
    void DeleteEdge(int u, int v) override {
        head[u] = helperDelete(u, v, head[u]);
        head[v] = helperDelete(v, u, head[v]);
        e--; // 注意：因為 helperDelete 會被叫兩次，e-- 只能在這裡執行一次
    }

    // 輔助刪除邏輯
    Edge* helperDelete(int target, int peer, Edge* currentHead) {
        Edge* curr = currentHead;
        Edge* prev = nullptr;

        while (curr != nullptr) {
            if ((curr->v1 == target && curr->v2 == peer) || (curr->v1 == peer && curr->v2 == target)) {
                Edge* nextInChain = (curr->v1 == target) ? curr->path1 : curr->path2;
                if (prev == nullptr) return nextInChain; // 刪除的是頭
                
                // 將前一個節點的正確 path 指向下一位
                if (prev->v1 == target) prev->path1 = nextInChain;
                else prev->path2 = nextInChain;
                
                return currentHead;
            }
            prev = curr;
            curr = (curr->v1 == target) ? curr->path1 : curr->path2;
        }
        return currentHead;
    }

    void DeleteVertex(int v) override {
        // 先刪除所有與 v 相關的邊
        Edge* curr = head[v];
        while (curr != nullptr) {
            int peer = (curr->v1 == v) ? curr->v2 : curr->v1;
            Edge* next = (curr->v1 == v) ? curr->path1 : curr->path2;
            DeleteEdge(v, peer); // 這裡會處理 delete 記憶體
            curr = next;
        }
        n--;
    }
};



int main()
{   
    int a,b;
    /*
    假設輸入是
    1 2 3 4 //頂點
    1 2 2 3 3 4 4 1  //邊
    */
    
    AdjacencyMatrix adj;
    while(cin>>a){ adj.InsertVertex(a);}
    while(cin>>a>>b)adj.InsertEdge(a,b);
    cout<<BigNum;

}