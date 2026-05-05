#include <iostream>
#include <map>
#include <vector>
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

class AdjacencyMatrixGraph: public Graph
{
private:
    vector<vector<int>> matrix;
    map<int, int> maap;
public:
    AdjacencyMatrixGraph()
    {
        n=0;  //vertex & edge不能為空　
        e=0;
        matrix.assign(n, vector<int>(n, 0));


    }

    ~AdjacencyMatrixGraph(){}

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


int main()
{   
    int a,b;
    /*
    假設輸入是
    1 2 3 4 //頂點
    1 2 2 3 3 4 4 1  //邊
    */
    
    AdjacencyMatrixGraph Adj();
    while(cin>>a); Adj.InsertVertex(a);}
    while(cin>>a>>b)e.push_back(a);
    
}