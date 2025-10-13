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

    for (char c : input) {  
        if (c != ',') {      
            a += c;
        }
    }

    cout<<"powerSet={ ";
    powerSet(a,0,"");
    
    cout << " }";
}