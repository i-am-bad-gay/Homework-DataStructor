# 41343139

作業一

## 解題說明

實作阿卡曼函數，使用遞迴和非遞迴解題

### 解題策略

1. 遞迴很簡單，只要照著他給的條件遞迴就行
2. 非遞迴就難上很多，我後來想了很久，想說既然是函數那一定會有規律
   所以就有了以下的公式。
3. 主程式呼叫計算的函式，並輸出計算結果。

## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
#include <cmath>
using namespace std;

long long Acker(int a,int b)
{
	if(a==0) return b+1;
	else if(b==0) return Acker(a-1,1);
	else return Acker(a-1,Acker(a,b-1));
}

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

int main()
{
	long long a,b;
    while(cin>>a>>b)
    {
		cout<<"Recursive: "<<Acker(a,b)<<endl;
		cout<<"Non-Recursive: "<<non(a,b)<<endl;
	} 
}
```

## 遞迴效能分析

| a 值 | 時間複雜度                   | 空間複雜度    |
| --- | ----------------------- | -------- |
| 0   | O(1)                    | O(1)     |
| 1   | O(b)                    | O(b)     |
| 2   | O(b²)                   | O(b)     |
| 3   | O(2^b)                  | O(b)     |
| 4   | O(2^(2^(…))) (b 層二進位指數) | O(2^(…)) |


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
| 測試一   | $n = 0$      | 0        | 0        |
| 測試二   | $n = 1$      | 1        | 1        |
| 測試三   | $n = 3$      | 6        | 6        |
| 測試四   | $n = 5$      | 15       | 15       |
| 測試五   | $n = -1$     | 異常拋出 | 異常拋出 |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o sigma sigma.cpp
$ ./sigma
6
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
   這樣就會很消耗空間跟時間。

   
### 選擇非遞迴(公式解)的優點和缺點

在本程式中，使用非遞迴(公式解)來計算阿卡曼和的優點如下：

1. **程式邏輯運行速度快非常多**  
   因為只要直接計算然後就返回數值，所以執行速度快上超級多。

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

