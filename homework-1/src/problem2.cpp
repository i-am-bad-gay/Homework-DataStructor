#include <iostream>
using namespace std;

void PowerSet(string a,int ii,int end,string c)
{
    if(ii!=end)
    {
        c+=a[ii];
        cout<< "(" << c << ")";
        PowerSet(a,ii+1,end,c+",");
    }
    
}


int main()
{
    string a;
    char b[1001]={'\0'};
    getline(cin,a);
    for(int i=0;i<a.size();i+=2)b[i/2]=a[i];  //取出字母
    int n = 0;
    while (b[n] != '\0')n++; // 計算元素個數

    cout << "powerSet(S) = {( ),";
    string B(b);
    for(int i=0;i<n;i++)
    {
        PowerSet(B,i,n,"");
    }
    
   
    cout << " }";
}