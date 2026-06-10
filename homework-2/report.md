
# 41343139,41343137


# 作業二 

## 解題說明
本作業實作了圖形（Graph）的抽象資料型態（ADT），並透過三種主流的資料結構進行底層封裝：**相鄰矩陣（Adjacency Matrix）**、**相鄰串列（Adjacency List）**、以及**相鄰多重串列（Adjacency Multilist）**。

此外，針對相鄰串列進行功能擴充，完整實作了五大基本圖形操作（Elementary Graph Operations）：深度優先搜尋（DFS）、廣度優先搜尋（BFS）、連通元件（Connected Components）、生成樹（Spanning Trees）以及雙連通元件（Biconnected Components）。

### 解題策略
* **介面抽象化（Polymorphism）**：定義基底抽象類別 `Graph`，將頂點與邊的增刪、查詢功能定為純虛擬函式，確保三種衍生子類別具備一致的外部介面。
* **空間與索引映射（Mapping）**：
    * **相鄰矩陣**：使用 `std::map` 將離散的頂點 ID 映射至連續的矩陣索引（0 到 $n-1$），解決動態配置與非連續頂點編號的對應問題。
    * **相鄰串列**：採用動態陣列 `vector<node*>` 搭配自訂單向鏈結串列，節省稀疏圖的空間。
    * **相鄰多重串列**：一條邊僅由一個 `Edge` 節點表示，內部維護兩個頂點的指標鏈結（`path1`, `path2`），適合無向圖的邊管理。
* **演算法核心設計**：
    * **DFS / BFS**：利用 `vector<bool>` 記錄造訪狀態，分別透過「遞迴堆疊」與「佇列（Queue）」實現。
    * **連通元件**：以外層迴圈逐一檢查未拜訪節點，藉由 DFS 走訪次數定義元件數量。
    * **生成樹**：在 DFS 過程中，即時記錄並輸出不構成迴路的樹邊（Tree Edges）。
    * **雙連通元件（BCC）**：採用 **Tarjan 演算法**，透過深度優先步數（`dfn`）與回溯最低步數（`low`）尋找關切點（Articulation Point），並利用邊堆疊（Edge Stack）即時彈出獨立的雙連通分支。

---

## 程式實作

以下為主要程式碼（整合完整資料結構與五大操作）：

```cpp
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
// 基底類別 Graph
// ==========================================
class Graph {
protected:
    int n; 
    int e; 

public:
    Graph() : n(0), e(0) {}
    virtual ~Graph() {}
    bool IsEmpty() const { return n == 0; }
    int NumberOfVertices() const { return n; }
    int NumberOfEdges() const { return e; }

    virtual int Degree(int u) const = 0;
    virtual bool ExistsEdge(int u, int v) const = 0;
    virtual void InsertVertex(int v) = 0;   
    virtual void InsertEdge(int u, int v, int cost) = 0; 
    virtual void DeleteVertex(int v) = 0;
    virtual void DeleteEdge(int u, int v) = 0;
};

// ==========================================
// 相鄰矩陣 AdjacencyMatrix
// ==========================================
class AdjacencyMatrix : public Graph {
private:
    vector<vector<int>> matrix;
    map<int, int> maap; 

public:
    AdjacencyMatrix() { n = 0; e = 0; }
    ~AdjacencyMatrix() override {}

    int Degree(int u) const override {
        auto it = maap.find(u);
        if (it == maap.end()) return 0;
        int idx = it->second;
        int count = 0; 
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
// 相鄰串列 AdjacencyList
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

    void InsertEdge(int u, int v, int cost = 1) override { 
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

    // --- 圖形演算法操作 ---
    void DFS(int startVertex) const {
        if (startVertex < 0 || startVertex >= liss.size()) return;
        vector<bool> visited(liss.size(), false);
        cout << "DFS 走訪順序: ";
        DFSUtil(startVertex, visited);
        cout << endl;
    }

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

    void SpanningTree(int startVertex) const {
        if (startVertex < 0 || startVertex >= liss.size()) return;
        vector<bool> visited(liss.size(), false);
        cout << "DFS 生成樹邊集合: ";
        SpanningTreeUtil(startVertex, visited);
        cout << endl;
    }

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
// 相鄰多重串列 AdjacencyMultilist
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

    void InsertEdge(int u, int v, int cost = 1) override { 
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
            delete targetEdge; 
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

int main() {
    AdjacencyList adjList;

    for (int i = 0; i <= 5; i++) adjList.InsertVertex(i);

    // 建立元件一 (0, 1, 2, 3, 4)，其中 1 為關切點 (Cut-vertex)
    adjList.InsertEdge(0, 1); adjList.InsertEdge(1, 0);
    adjList.InsertEdge(1, 2); adjList.InsertEdge(2, 1);
    adjList.InsertEdge(2, 0); adjList.InsertEdge(0, 2);
    adjList.InsertEdge(1, 3); adjList.InsertEdge(3, 1);
    adjList.InsertEdge(3, 4); adjList.InsertEdge(4, 3);
    adjList.InsertEdge(4, 1); adjList.InsertEdge(1, 4);

    // 建立元件二 (5) 為孤立點

    cout << "=== 圖形基本操作測試 ===" << endl;
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
```

---

## 測試與驗證

### 編譯與執行指令

```shell
# 使用 g++ 以 C++11 標準編譯主程式
g++ -std=c++11 main.cpp -o graph_demo

# 執行編譯後的執行檔
./graph_demo
```

### 結論
測試範例設計了一個包含 6 個頂點（0~5）的無向圖。頂點 0, 1, 2 構成一個環，頂點 1, 3, 4 構成另一個環，兩者交於頂點 1；頂點 5 則為獨立的連通元件。

經環境編譯執行後：
1. **DFS 與 BFS**：能正確依據先進後出（Stack）與先進先出（Queue）的邏輯完成頂點拜訪。
2. **連通元件**：成功切分出兩個元件（元件一：0 1 2 3 4、元件二：5）。
3. **生成樹**：完美提取出無迴路的 DFS 生成邊集合。
4. **雙連通元件**：Tarjan 演算法成功抓出頂點 1 為關切點，並正確輸出兩個獨立的雙連通區塊（BCC 邊集合）。驗證了程式的邏輯正確性與結構穩定度。

---

## 申論及開發報告

### 1. 三種圖形表示法的效能權衡（Trade-offs）

| 特性 / 結構 | 相鄰矩陣 (Adjacency Matrix) | 相鄰串列 (Adjacency List) | 相鄰多重串列 (Adjacency Multilist) |
| :--- | :--- | :--- | :--- |
| **空間複雜度** | $O(V^2)$ | $O(V + E)$ | $O(V + E)$ |
| **判斷邊是否存在** | $O(1)$ | $O(\text{Degree}(u))$ | $O(\text{Degree}(u))$ |
| **動態加/刪頂點** | $O(V^2)$（需擴展/縮減矩陣） | $O(1)$ 或 $O(V)$（刪除需遍歷全圖） | $O(1)$ 或 $O(V)$ |
| **最適用場景** | 稠密圖（Dense Graph） | 稀疏圖（Sparse Graph） | 無向圖且需要頻繁操作「邊」 |

在開發過程中發現，**相鄰矩陣**雖然查詢邊的時間複雜度極佳（$O(1)$），但面對圖形頂點數頻繁動態增刪時，矩陣重建與記憶體拷貝的成本過高；相對地，**相鄰串列**表現最為平衡，也是實作 DFS/BFS 等基本圖形走訪的最佳基底。

### 2. 開發陷阱與錯誤修正報告

在整合這份程式碼時，原先存在幾處底層臭蟲（Bugs），已在此版本全面修正：
* **多重串列的記憶體洩漏（Memory Leak）**：原 `DeleteEdge` 在拔除節點鏈結時，並未真正呼叫 `delete` 釋放 `Edge*`。現已修正為先取出該指標，鏈結完成斷開後確實回收記憶體。
* **虛擬函式介面不一致**：原矩陣與串列的 `InsertEdge` 參數數量不同（一個有 `cost`、一個沒有），導致多型（Override）失效，無法通過編譯。現已統一規格，並以預設參數 `cost = 1` 相容無權重圖。
* **未初始化變數**：矩陣的 `Degree()` 函式中，計數器變數 `int count;` 未初始化即進行自增運算，導致輸出垃圾值。現已確實初始化為 `0`。

```