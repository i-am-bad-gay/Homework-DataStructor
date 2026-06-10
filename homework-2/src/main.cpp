#include <iostream>
#include <map>
#include <vector>
#include <list> 
#include <queue>
#include <stack>
#include <algorithm>
#include <climits>

using namespace std;

#define BigNum INT_MAX

// ==========================================
// 1. 基底類別 Graph
// ==========================================
class Graph {
protected:
    int n; // 頂點數
    int e; // 邊數

public:
    Graph() : n(0), e(0) {}
    virtual ~Graph() {}
    bool IsEmpty() const { return n == 0; }
    int NumberOfVertices() const { return n; }
    int NumberOfEdges() const { return e; }

    virtual int Degree(int u) const = 0;
    virtual bool ExistsEdge(int u, int v) const = 0;
    virtual void InsertVertex(int v) = 0;   
    virtual void InsertEdge(int u, int v, int cost) = 0; // 統一改為 3 個參數
    virtual void DeleteVertex(int v) = 0;
    virtual void DeleteEdge(int u, int v) = 0;
};

// ==========================================
// 2. 相鄰矩陣 AdjacencyMatrix
// ==========================================
class AdjacencyMatrix : public Graph {
private:
    vector<vector<int>> matrix;
    map<int, int> maap; // 頂點 ID 到矩陣索引 (0, 1, 2...) 的映射

public:
    AdjacencyMatrix() {
        n = 0;  
        e = 0;
    }
    ~AdjacencyMatrix() override {}

    int Degree(int u) const override {
        auto it = maap.find(u);
        if (it == maap.end()) return 0;
        int idx = it->second;
        int count = 0; // 修正：初始化變數
        for (int i = 0; i < n; i++) {
            if (matrix[idx][i] != BigNum && matrix[idx][i] != 0) count++;
        }
        return count;
    }

    bool ExistsEdge(int u, int v) const override {
        auto it_u = maap.find(u);
        auto it_v = maap.find(v);
        if (it_u == maap.end() || it_v == maap.end()) return false;
        return matrix[it_u->second][it_v->second] != BigNum;
    }

    void InsertVertex(int v) override {
        if (maap.find(v) == maap.end()) {
            for (int i = 0; i < n; i++) {
                matrix[i].push_back(BigNum);
            }
            maap[v] = n; 
            n++;
            vector<int> newRow(n, BigNum);
            newRow[n - 1] = 0; 
            matrix.push_back(newRow);
        } 
    }

    void InsertEdge(int u, int v, int cost) override {   
        if (maap.find(u) == maap.end() || maap.find(v) == maap.end()) return;
        int idx_u = maap[u];
        int idx_v = maap[v];
        matrix[idx_u][idx_v] = cost;
        e++;
    }

    void DeleteVertex(int v) override {   
        if (maap.find(v) == maap.end()) return;
        int indx = maap[v];
        int edgesToDelete = 0;
        
        for (int j = 0; j < n; j++) {
            if (matrix[indx][j] != BigNum && matrix[indx][j] != 0) {
                edgesToDelete++;
            }
        }
        
        matrix.erase(matrix.begin() + indx);
        for (int i = 0; i < n - 1; i++) {
            matrix[i].erase(matrix[i].begin() + indx);
        }
        
        for (auto it = maap.begin(); it != maap.end(); ++it) {
            if (it->second > indx) it->second -= 1; 
        }
        
        n--;
        e -= edgesToDelete;
        maap.erase(v);
    }

    void DeleteEdge(int u, int v) override {
        auto it_u = maap.find(u);
        auto it_v = maap.find(v);
        if (it_u != maap.end() && it_v != maap.end()) {
            matrix[it_u->second][it_v->second] = BigNum;
            e--;
        }
    }
};

// ==========================================
// 3. 相鄰串列 AdjacencyList (內含五大基本操作)
// ==========================================
class node {
public:
    int data;
    node* nex; 
    node(int d) : data(d), nex(nullptr) {}
};

class AdjacencyList : public Graph {
private:
    vector<node*> liss; 

    // DFS 輔助函式
    void DFSUtil(int v, vector<bool>& visited) const {
        visited[v] = true;
        cout << v << " ";
        node* curr = liss[v];
        while (curr) {
            if (!visited[curr->data]) {
                DFSUtil(curr->data, visited);
            }
            curr = curr->nex;
        }
    }

    // Spanning Tree 輔助函式
    void SpanningTreeUtil(int v, vector<bool>& visited) const {
        visited[v] = true;
        node* curr = liss[v];
        while (curr) {
            if (!visited[curr->data]) {
                cout << "(" << v << ", " << curr->data << ") ";
                SpanningTreeUtil(curr->data, visited);
            }
            curr = curr->nex;
        }
    }

    // BCC Tarjan 演算法輔助函式
    void BCCUtil(int u, int& time, vector<int>& dfn, vector<int>& low, 
                 vector<int>& parent, stack<pair<int, int>>& st) const {
        dfn[u] = low[u] = ++time;
        int children = 0;
        node* curr = liss[u];

        while (curr) {
            int v = curr->data;
            if (dfn[v] == 0) {
                children++;
                parent[v] = u;
                st.push({u, v});

                BCCUtil(v, time, dfn, low, parent, st);
                low[u] = min(low[u], low[v]);

                if ((parent[u] == -1 && children > 1) || (parent[u] != -1 && low[v] >= dfn[u])) {
                    cout << "  BCC 邊集合: ";
                    while (st.top().first != u || st.top().second != v) {
                        cout << "(" << st.top().first << "," << st.top().second << ") ";
                        st.pop();
                    }
                    cout << "(" << st.top().first << "," << st.top().second << ")\n";
                    st.pop();
                }
            } 
            else if (v != parent[u] && dfn[v] < dfn[u]) {
                low[u] = min(low[u], dfn[v]);
                st.push({u, v});
            }
            curr = curr->nex;
        }
    }

public:
    AdjacencyList() { n = 0; e = 0; }
    
    ~AdjacencyList() override {
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
        if (v >= liss.size()) {
            liss.resize(v + 1, nullptr);
            n++;
        }
    }

    void InsertEdge(int u, int v, int cost = 1) override { // 修正：符合基底類別參數
        if (u >= liss.size() || v >= liss.size()) return;
        if (ExistsEdge(u, v)) return;

        node* newNode = new node(v);
        newNode->nex = liss[u];
        liss[u] = newNode;
        e++;
    }

    void DeleteEdge(int u, int v) override {
        if (u < 0 || u >= liss.size()) return;
        node* curr = liss[u];
        node* prev = nullptr;

        while (curr) {
            if (curr->data == v) {
                if (!prev) liss[u] = curr->nex;
                else prev->nex = curr->nex; 
                delete curr;
                e--;
                return;
            }
            prev = curr;
            curr = curr->nex;
        }
    }

    void DeleteVertex(int v) override {
        if (v < 0 || v >= liss.size()) return;
        node* curr = liss[v];
        while (curr) {
            node* temp = curr;
            curr = curr->nex;
            delete temp;
            e--;
        }
        liss[v] = nullptr;

        for (int i = 0; i < liss.size(); i++) {
            if (i == v) continue;
            DeleteEdge(i, v); 
        }
        n--;
    }

    // --- 擴充功能實作 ---
    
    // 1. Depth First Search (DFS)
    void DFS(int startVertex) const {
        if (startVertex < 0 || startVertex >= liss.size()) return;
        vector<bool> visited(liss.size(), false);
        cout << "DFS 走訪順序: ";
        DFSUtil(startVertex, visited);
        cout << endl;
    }

    // 2. Breadth First Search (BFS)
    void BFS(int startVertex) const {
        if (startVertex < 0 || startVertex >= liss.size()) return;
        vector<bool> visited(liss.size(), false);
        queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        cout << "BFS 走訪順序: ";
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            cout << v << " ";

            node* curr = liss[v];
            while (curr) {
                if (!visited[curr->data]) {
                    visited[curr->data] = true;
                    q.push(curr->data);
                }
                curr = curr->nex;
            }
        }
        cout << endl;
    }

    // 3. Connected Components
    void ConnectedComponents() const {
        vector<bool> visited(liss.size(), false);
        int count = 0;
        cout << "--- 連通元件列表 ---\n";
        for (int i = 0; i < liss.size(); i++) {
            if (!visited[i] && (liss[i] != nullptr || Degree(i) > 0)) {
                cout << "元件 " << ++count << ": ";
                DFSUtil(i, visited);
                cout << endl;
            }
        }
    }

    // 4. Spanning Trees
    void SpanningTree(int startVertex) const {
        if (startVertex < 0 || startVertex >= liss.size()) return;
        vector<bool> visited(liss.size(), false);
        cout << "DFS 生成樹邊集合: ";
        SpanningTreeUtil(startVertex, visited);
        cout << endl;
    }

    // 5. Biconnected Components (BCC)
    void BiconnectedComponents() const {
        int V = liss.size();
        vector<int> dfn(V, 0), low(V, 0), parent(V, -1);
        stack<pair<int, int>> st;
        int time = 0;

        cout << "--- 雙連通元件 (BCC) 列表 ---\n";
        for (int i = 0; i < V; i++) {
            if (dfn[i] == 0 && (liss[i] != nullptr || Degree(i) > 0)) {
                BCCUtil(i, time, dfn, low, parent, st);
                
                if (!st.empty()) {
                    cout << "  BCC 邊集合: ";
                    while (!st.empty()) {
                        cout << "(" << st.top().first << "," << st.top().second << ") ";
                        st.pop();
                    }
                    cout << endl;
                }
            }
        }
    }
};

// ==========================================
// 4. 相鄰多重串列 AdjacencyMultilist
// ==========================================
struct Edge {
    bool mark;          
    int v1, v2;         
    Edge *path1, *path2; 
    Edge(int u, int v) : mark(false), v1(u), v2(v), path1(nullptr), path2(nullptr) {}
};

class AdjacencyMultilist : public Graph {       
private:
    vector<Edge*> head; 

    Edge* helperDelete(int target, int peer, Edge* currentHead) {
        Edge* curr = currentHead;
        Edge* prev = nullptr;

        while (curr != nullptr) {
            if ((curr->v1 == target && curr->v2 == peer) || (curr->v1 == peer && curr->v2 == target)) {
                Edge* nextInChain = (curr->v1 == target) ? curr->path1 : curr->path2;
                if (prev == nullptr) return nextInChain; 
                
                if (prev->v1 == target) prev->path1 = nextInChain;
                else prev->path2 = nextInChain;
                
                return currentHead;
            }
            prev = curr;
            curr = (curr->v1 == target) ? curr->path1 : curr->path2;
        }
        return currentHead;
    }

public:
    AdjacencyMultilist(int vertices = 0) {
        n = vertices; e = 0;
        head.assign(n, nullptr);
    }

    void InsertVertex(int v) override {
        if (v >= head.size()) {
            head.resize(v + 1, nullptr);
            n++;
        }
    }

    void InsertEdge(int u, int v, int cost = 1) override { // 修正：符合基底類別參數
        if (u >= head.size() || v >= head.size()) return;
        
        Edge* newEdge = new Edge(u, v);
        newEdge->path1 = head[u];
        head[u] = newEdge;
        newEdge->path2 = head[v];
        head[v] = newEdge;
        e++;
    }

    bool ExistsEdge(int u, int v) const override {
        if (u >= head.size()) return false;
        Edge* curr = head[u];
        while (curr != nullptr) {
            if ((curr->v1 == u && curr->v2 == v) || (curr->v1 == v && curr->v2 == u)) {
                return true;
            }
            curr = (curr->v1 == u) ? curr->path1 : curr->path2;
        }
        return false;
    }

    int Degree(int u) const override {
        if (u >= head.size()) return 0;
        int count = 0;
        Edge* curr = head[u];
        while (curr != nullptr) {
            count++;
            curr = (curr->v1 == u) ? curr->path1 : curr->path2;
        }
        return count;
    }

    void DeleteEdge(int u, int v) override {
        if (u >= head.size() || v >= head.size()) return;
        
        // 先找出該 Edge 節點指標，最後釋放記憶體
        Edge* targetEdge = nullptr;
        Edge* curr = head[u];
        while (curr != nullptr) {
            if ((curr->v1 == u && curr->v2 == v) || (curr->v1 == v && curr->v2 == u)) {
                targetEdge = curr;
                break;
            }
            curr = (curr->v1 == u) ? curr->path1 : curr->path2;
        }

        head[u] = helperDelete(u, v, head[u]);
        head[v] = helperDelete(v, u, head[v]);
        
        if (targetEdge) {
            delete targetEdge; // 修正：避免記憶體洩漏
            e--;
        }
    }

    void DeleteVertex(int v) override {
        if (v >= head.size()) return;
        Edge* curr = head[v];
        while (curr != nullptr) {
            int peer = (curr->v1 == v) ? curr->v2 : curr->v1;
            Edge* next = (curr->v1 == v) ? curr->path1 : curr->path2;
            DeleteEdge(v, peer); 
            curr = next;
        }
        n--;
    }
};

// ==========================================
// 5. 測試主程式 main()
// ==========================================
int main() {
    AdjacencyList adjList;

    // 建立 0 到 5 號頂點
    for (int i = 0; i <= 5; i++) {
        adjList.InsertVertex(i);
    }

    // 建立無向圖邊結構 (雙向插入)
    // 元件 1: 包含 0, 1, 2, 3, 4 (其中 1 為關切點)
    adjList.InsertEdge(0, 1); adjList.InsertEdge(1, 0);
    adjList.InsertEdge(1, 2); adjList.InsertEdge(2, 1);
    adjList.InsertEdge(2, 0); adjList.InsertEdge(0, 2);
    adjList.InsertEdge(1, 3); adjList.InsertEdge(3, 1);
    adjList.InsertEdge(3, 4); adjList.InsertEdge(4, 3);
    adjList.InsertEdge(4, 1); adjList.InsertEdge(1, 4);

    // 元件 2: 孤立點 5
    // 頂點 5 沒有連向任何人

    // --- 測試五大操作 ---
    cout << "=== 圖形基本操作測試 (以頂點 0 開始) ===" << endl;
    adjList.DFS(0);
    adjList.BFS(0);
    
    cout << endl;
    adjList.ConnectedComponents();
    
    cout << endl;
    adjList.SpanningTree(0);
    
    cout << endl;
    adjList.BiconnectedComponents();

    return 0;
}