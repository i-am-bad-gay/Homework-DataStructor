#include <iostream>
#include <map>
#include <vector>
#include <list>   
using namespace std;

class Graph
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
    virtual void InsertEdge(int u,int v)=0;
    virtual void DeleteVertex(int v)=0;
    virtual void DeleteEdge(int u,int v)=0;
};

class AdjacencyMatrix: public Graph
{
private:
    vector<vector<int>> matrix;
    map<int, int> maap;
public:
    AdjacencyMatrix()
    {
        n=0;  
        e=0;
        matrix.assign(n, vector<int>(n, 0));

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

    void InsertVertex(int v)override         //插入vertex
    {
        if(maap.find(v) == maap.end())       //找不到V V不存在
        {
            for(int i=0;i<n;i++) matrix[i].push_back(0);
            maap[n]=v;
            n++;
            matrix.push_back(vector <int>(n,0));
        } 
    }

    virtual void InsertEdge(int u,int v)override  //插入edge
    {
        matrix[u][v]=1;
        e++;
    }

    virtual void DeleteVertex(int v)override
    {    
        int indx=maap[v];
        int de=Degree(v);
        
        matrix.erase(matrix.begin() + indx);  //刪除行
        for(int i=0;i<n-1;i++)matrix[i].erase(matrix[i].begin() + indx);  //刪除列
        
        //更新字典
        for (auto it = maap.begin(); it != maap.end(); ++it)
        {
            if (it->second > indx)
            {
                it->second -= 1; 
            }
        }
        
        n--;
        e-=de;
        maap.erase(v);
    }

    virtual void DeleteEdge(int u,int v)override
    {
        matrix[u][v]=0;
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

bool check(vector <node*>& a,int b)  //check vector is there have any repeat number
{
    for(int i=0;i<a.size();i++)
    {
        if(a[i].data==b)return true;
    }
    return false;
}

class AdjacencyList: public Graph
{
private:    
    vector <node*> liss;  //拿來裝nodes的
public:
    AdjacencyList()
    {
        n=0;e=0;
    }
    ~AdjacencyList(){}

    int Degree(int u)const override
    {
        if (u >= liss.size()) return 0;
        int count = 0;
        node* curr = liss[u];
        while (curr != nullptr) {
            count++;
            curr = curr->nex;
        }
        return count;
    }
    bool ExistsEdge(int u,int v)const override
    {
        if (u >= liss.size()) return false;
        node* curr = liss[u];
        while (curr != nullptr) {
            if (curr->data == v) return true;
            curr = curr->nex;
        }
        return false;
    }
    void InsertVertex(int v)override
    {
        if(check(liss,v)==0)
        {
            node* newNode=new node(v);
            liss.push_back(newNode);
        }
    }   
    void InsertEdge(int u,int v) override
    {
        //產生一個新的node
        node* newNode = new node(v);
        //找到u的尾端的ptr
       if (liss[u]->nex == nullptr) {
        // 如果還沒有鄰居，直接讓 liss[u] 指向這個新節點
        liss[u] = newNode;
        } 
        else {
        // 3. 如果已經有鄰居，就順著指標走到最末端
        node* temp = liss[u];
        while (temp->nex != nullptr) {
            temp = temp->nex;
        }
        // 4. 把新鄰居接在最後面
        temp->nex = newNode;
        }

        
    }
    void DeleteVertex(int v) override
    {
        if (u >= liss.size()) return;

        node* curr = liss[u];
        node* prev = nullptr;

        while (curr != nullptr) {
            if (curr->data == v) {
                if (prev == nullptr) {
                    // 要刪除的是頭節點
                    liss[u] = curr->nex;
                } else {
                    // 要刪除的是中間或尾巴
                    prev->nex = curr->nex;
                }
                delete curr;
                e--;
                return;
            }
            prev = curr;
            curr = curr->nex;
        }
    }
    void DeleteEdge(int u,int v)override
    {
        if (v >= liss.size()) return;

        // 1. 刪除從 v 出發的所有邊
        node* curr = liss[v];
        while (curr != nullptr) {
            node* temp = curr;
            curr = curr->nex;
            delete temp;
            e--;
        }
        liss[v] = nullptr;

        // 2. 掃描所有其他頂點，刪除所有指向 v 的邊
        for (int i = 0; i < liss.size(); ++i) {
            if (i == v) continue;
            DeleteEdge(i, v); // 呼叫剛才寫好的 DeleteEdge
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

class AdjacencyMultilist : public Graph {
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
    

}