# 41343139

作業一 problem1

## 解題說明

實作多項式，使用Class來實現多種功能

### 解題策略

1. 
2. 
3. 主程式呼叫計算的函式，並輸出計算結果。

## 程式實作

以下為主要程式碼：

```cpp

```

## 遞迴效能分析

| a 值 | 時間複雜度                   | 空間複雜度    |
| --- | ----------------------- | -------- |
| 0   | O(1)                    | O(1)     |
| 1   | O(b)                    | O(b)     |
| 2   | O(b²)                   | O(b)     |
| 3   | O(2^b)                  | O(b)     |
| 4   |  |  |


## 遞迴效能分析

| a 值 | 時間複雜度    | 空間複雜度 |
| --- | -------- | ----- |
| 0   | O(1)     | O(1)  |
| 1   | O(1)     | O(1)  |
| 2   | O(1)     | O(1)  |
| 3   | O(log b) | O(1)  |
| 4   | O(b)     | O(1)  |

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | $n = (1,1)$      | 3        | 3        |
| 測試二   | $n = (2,1)$      | 5        | 5        |
| 測試三   | $n = (4,0)$      | 13       | 13       |
| 測試四   | $n = (3,5)$      | 253      | 253       |
| 測試五   | $n = (4,1)$      | 異常 | 異常 |

### 編譯與執行指令

```shell
$PS D:\Homework-DataStructor> ^C
$PS D:\Homework-DataStructor>
$PS D:\Homework-DataStructor>  & 'c:\Users\huang\.$vscode\extensions\ms-vscode.cpptools-1.28.$1-win32-x64\debugAdapters\bin\WindowsDebugLauncher.exe' $'--stdin=Microsoft-MIEngine-In-rculw2xu.pll' $'--stdout=Microsoft-MIEngine-Out-xwlwadxx.ftk' $'--stderr=Microsoft-MIEngine-Error-erf4cd4e.0d1' $'--pid=Microsoft-MIEngine-Pid-qxcrkkin.iyo' '--dbgExe=D:\mingw64\bin\gdb.$exe' '--interpreter=mi'
$ 1 1
Recursive: 3
Non-Recursive: 3
$ 2 1
Recursive: 5
Non-Recursive: 5
$ 4 0
Recursive: 13
Non-Recursive: 13
$ 3 5
Recursive: 253
Non-Recursive: 253
```

### 結論

1. 程式能正確計算 $(m,n) n<4$ 的數字。  
2. 但在 $n>4$ 的情況下，除了 $(4,0)$ 程式都會出現異常。  
3. 但非遞迴的方法如果改成用string輸出或許可以計算，但遞迴不論如何都會爆開。

## 申論及開發報告

### 選擇遞迴的優點和缺點

在本程式中，使用遞迴來計算阿卡曼和的優點如下：

1. **程式邏輯簡單直觀**  
   遞迴的寫法能夠清楚的將問題公式化，蠻可讀的，一看就懂。  

2. **易於理解與實現**  
   遞迴的程式碼更接近阿卡曼函數的表示方式。  
   以本程式為例：  

   ```cpp
  	long long Acker(int a,int b)
	{
		if(a==0) return b+1;
		else if(b==0) return Acker(a-1,1);
		else return Acker(a-1,Acker(a,b-1));
	}
   ```
在本程式中，使用遞迴來計算阿卡曼和的缺點如下：.

1. **程式運行速度慢**  
   遞迴並不像迴圈一樣再跑一次那麼簡單，而是要存著這段再開一個functioin，
   在需要遞迴很多次的情況下，就會很消耗空間跟時間。

   
### 選擇非遞迴(公式解)的優點和缺點

在本程式中，使用非遞迴(公式解)來計算阿卡曼和的優點如下：

1. **程式邏輯運行速度快非常多**  
   因為只要直接計算然後就返回數值，所以執行速度快上超級多。

2. **程式需要的空間也少很多**  
   公式解只要用到少數必要的變數就好，不像遞迴要存很多東西。

3. **程式拓展性**  
   假如真的要計算(4,1)的值，這個方法稍微改一下就可以用了。

在本程式中，使用非遞迴(公式解)來計算阿卡曼和的缺點如下：

1. **程式邏可讀性變差**  
   因為是把那些條件直接變成公式，所以直接閱讀代碼是無法理解在幹嘛。
   ```cpp
	long long non(int a,int b)
	{
		if(a==0)return b+1;
		if(a==1)return b+2;
		if(a==2)return 2*b+3;
		if(a==3)return pow(2,b+3)-3;
		if(a==4) {  // 2↑↑(n+3)-3 
	        long long res = 2;
	        for(int i = 1; i < b+3; i++) {
	            res = (long long)pow(2, res);  
	        }
	        return res - 3;
	    }
	}
   ```

作業一 problem2

## 解題說明



### 解題策略

1. 把每一個子集合都當成岔路，先走到路的底端之後再到岔路口，再從岔路口出發



## 程式實作

以下為主要程式碼：

```cpp
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

    for (char c : input)  //分割逗號
    {  
        if (c != ',')  
        {      
            a += c;
        }
    }

    cout<<"powerSet={ ";
    powerSet(a,0,"");
    
    cout << " }";
}
```

## 效能分析
時間複雜度:O(2^n)
空間複雜度:O(n)

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | $a,b,c  $    | (),(c),(b),(b,c),(a),(a,c),(a,b),(a,b,c)       | (),(c),(b),(b,c),(a),(a,c),(a,b),(a,b,c)        |
| 測試二   | $e,g,1  $     | (),(1),(g),(g,1),(e),(e,1),(e,g),(e,g,1)        | (),(1),(g),(g,1),(e),(e,1),(e,g),(e,g,1)        |
| 測試三   | $aaa,bb,cc  $        | (),(aaa),(bb),(cc),(aaa,bb),(aaa,cc),(bb,cc),(aaa,bb,cc)       | (),(c),(c),(c,c),(b),(b,c),(b,c),(b,c,c),(b),(b,c),(b,c),(b,c,c),(b,b),(b,b,c),(b,b,c),(b,b,c,c),(a),(a,c),(a,c),(a,c,c),(a,b),(a,b,c),(a,b,c),(a,b,c,c),(a,b),(a,b,c),(a,b,c),(a,b,c,c),(a,b,b),(a,b,b,c),(a,b,b,c),(a,b,b,c,c),(a),(a,c),(a,c),(a,c,c),(a,b),(a,b,c),(a,b,c),(a,b,c,c),(a,b),(a,b,c),(a,b,c),(a,b,c,c),(a,b,b),(a,b,b,c),(a,b,b,c),(a,b,b,c,c),(a,a),(a,a,c),(a,a,c),(a,a,c,c),(a,a,b),(a,a,b,c),(a,a,b,c),(a,a,b,c,c),(a,a,b),(a,a,b,c),(a,a,b,c),(a,a,b,c,c),(a,a,b,b),(a,a,b,b,c),(a,a,b,b,c),(a,a,b,b,c,c),(a),(a,c),(a,c).......... }       |



### 編譯與執行指令

```shell
$PS D:\Homework-DataStructor> ^C
$PS D:\Homework-DataStructor>
$PS D:\Homework-DataStructor>  & 'c:\Users\huang\.vscode\extensions\ms-vscode.$'--stdin=Microsoft-MIEngine-In-ogjpaxud.kws' $'--stdout=Microsoft-MIEngine-Out-1v2f4epm.xdu' $'--stderr=Microsoft-MIEngine-Error-wkzhdsg4.35t' $'--pid=Microsoft-MIEngine-Pid-dtcabuh4.rbk' '--dbgExe=D:\mingw64\bin\gdb.$exe' '--interpreter=mi'
$a,b,c
powerSet={ (),(c),(b),(b,c),(a),(a,c),(a,b),(a,b,c), }
```

### 結論

1. 這樣寫程式能正確的計算只有單個字元的子集合 。  
2. 但在element字元有>1的情況下，這個方法就不適用了。  

## 申論及開發報告

### 選擇遞迴的優點和缺點

在本程式中，使用遞迴的優點如下：

1. **程式邏輯簡單化**  
   相比於其他解法，遞迴更能簡單明瞭的寫出來，只寫出想選或不選的決策樹就好。 

1. **易於理解**
    比起迴圈或其他寫法，程式比較短，也蠻好理解的，容易看懂。 


在本程式中，使用遞迴的缺點如下：.

1. **遞迴的限制**  
   當element大於一定的數量的時候，就沒辦法用遞迴了。
