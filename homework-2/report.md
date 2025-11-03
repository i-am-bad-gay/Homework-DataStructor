# 41343139

作業一 problem1

## 解題說明

實作多項式，使用Class來實現多種功能

### 解題策略

1. 輸入先用 class term 把輸入的值捆在一起
2. 然後再用 class Polynomial 把term捆再一起
3. 之後再對 Polynomial操作就行
然後實作 ADD MULT NEW TERM EVAl
4. new term 要注意 加之後等於0 已經有相同的指數 或是 空間不夠
5. 但我還額外再做一個按照排序插入 這樣之後很好用
6. ADD就是只要找到相同的指數就加起來
7. Mult 用迴圈遍歷就行
只是 ADD跟MULT 寫起來要簡單new term 就要多寫一點東西

## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
#include <cmath>
using namespace std;

class Polynomial;

class Term
{
    friend Polynomial;
    friend ostream& operator<<(ostream &output,const Polynomial &Poly);
    private:
        int exp;	// 指数
        float coef; // 系数
};

class Polynomial{
    private:
    	Term *termArray;
    	int capacity;	//容量
    	int terms;		//實際使用
        
    public:
    	Polynomial():capacity(2),terms(0){
    		termArray = new Term[capacity];
    	}
    	~Polynomial(){delete[] termArray;}

    	Polynomial Add(Polynomial b);
		Polynomial Mult(Polynomial b);
		float eval(float f);
    	void newTerm(const float newcoef,const int newexp);

    	friend istream& operator>>(istream &input,Polynomial &Poly);
    	friend ostream& operator<<(ostream& os, const Polynomial& poly);
    
};

istream& operator>>(istream& is, Polynomial& poly){
    float coef;int exp,n;
    is>>n;
    while(n--){
    	is>>coef>>exp;
    	poly.newTerm(coef,exp);
    }
    return is;
}

ostream& operator<<(ostream& os, const Polynomial& poly){
    for(int i=0;i<poly.terms;++i){
    	if(i>0) os<<"+";
    	os<<poly.termArray[i].coef<<"X^"<<poly.termArray[i].exp;
    }
    return os;
}

void Polynomial::newTerm (const float theCoef, const int theExp) {
    if(theCoef==0) return;

    if(terms==capacity){ 	//擴充
    	capacity*=2;
    	Term *temp=new Term[capacity];
    	copy(termArray,termArray+terms,temp);
    	delete []termArray;
    	termArray=temp;
    }

	int i = 0;
    while (i < terms && termArray[i].exp > theExp) i++; 	//找到插入的位置

	
    if (i < terms && termArray[i].exp == theExp) { 		// 如果同次項已存在 直接加系數
        termArray[i].coef += theCoef;
        if (termArray[i].coef == 0) { 	// 如果加完變 0 就移除這項
            
            for (int j = i; j < terms - 1; j++) 	//這是把i之後的都往後移動
                termArray[j] = termArray[j + 1];
            terms--;
        }
        return;
	}

	
	for (int j = terms; j > i; --j)
        termArray[j] = termArray[j - 1];
    
    termArray[i].coef = theCoef;
    termArray[i].exp = theExp;
    terms++;
}


Polynomial Polynomial::Add(Polynomial b){
    Polynomial c;
    int aPos=0,bPos=0;
    while((aPos<terms)&&(bPos<b.terms)){
    	if(termArray[aPos].exp==b.termArray[bPos].exp){				//指數相同
    		float t=termArray[aPos].coef+b.termArray[bPos].coef;	//係數相加
    		if(t) c.newTerm(t,termArray[aPos].exp); 	//確認係數不為0
    		aPos++;bPos++;
    	}
    	else if(termArray[aPos].exp<b.termArray[bPos].exp){ 	//指數比大小
    		c.newTerm(b.termArray[bPos].coef,b.termArray[bPos].exp);
    		bPos++;
    	}else{
    		c.newTerm(termArray[aPos].coef,termArray[aPos].exp);
    		aPos++;
    	}
    }
	for(;aPos<terms;aPos++)
		c.newTerm(termArray[aPos].coef,termArray[aPos].exp);
	for(;bPos<b.terms;bPos++)
		c.newTerm(b.termArray[bPos].coef,b.termArray[bPos].exp);
	return c;
}

Polynomial Polynomial::Mult(Polynomial b){
    Polynomial c;

	for(int i=0;i<b.terms;i++)
	{
		Polynomial d;
		for(int j=0;j<terms;j++)
		{
			float coef = termArray[i].coef * b.termArray[j].coef;
			int exp = termArray[i].exp + b.termArray[j].exp;
			c.newTerm(coef, exp);
		}
	}
	return c; 
}


float Polynomial::eval(float f) {
    float result = 0.0;
    for (int i = 0; i < terms; ++i) {
        result += termArray[i].coef * pow(f, termArray[i].exp);
    }
    return result;
}

int main(){
	Polynomial a, b;
	cin >> a >> b;

	cout << "a(x): " << a << endl;
	cout << "b(x): " << b << endl;

	Polynomial c = a.Add(b);
	cout << "a + b: " << c << endl;

	Polynomial d = a.Mult(b);
	cout << "a * b: " << d << endl;

	cout << "a(2) = " << a.eval(2) << endl;
	cout << "b(2) = " << b.eval(2) << endl;
	system("pause");
}
```

## 遞迴效能分析

| 函式名稱| 時間複雜度                   | 空間複雜度    |
| --- | ----------------------- | -------- |
| newTerm()   | O(n)                   | O(n)     |
| Add()   | O(m + n)                    | O(m + n)     |
| Mult()   | O(m × n × k)                  | O(m + n)     |
| eval()   | O(n)                   | O(1)     |


## 測試與驗證

| 測試案例 | 輸入參數  |
|----------|--------------|
| Polynomial 1   | 3  2 3 -1 1 5 0     | 3        | 3        |
| Polynomial 2  | 4 1 4 3 3 2 1-7 0      | 5        | 5|

|          |預期輸出 | 實際輸出 |
|----------|----------|----------|
|ADD|||
|MULT|||
|EVAL(2)|a(2) = 19 , b(2) = 0|a(2) = 19 , b(2) = 0|
### 編譯與執行指令

```shell
PS D:\C++\data_structor>  & 'c:\Users\huang\.vscode\extensions\ms-vscode.cpptools-1.28.3-win32-x64\debugAdapters\bin\WindowsDebugLauncher.exe' '--stdin=Microsoft-MIEngine-In-vjsdutnk.gzg' '--stdout=Microsoft-MIEngine-Out-0ltnpyfy.ern' '--stderr=Microsoft-MIEngine-Error-j0jhau0s.uge' '--pid=Microsoft-MIEngine-Pid-kiehy0mu.v50' '--dbgExe=D:\mingw64\bin\gdb.exe' '--interpreter=mi' 
3
2 3
-1 1
5 0

4
1 4
3 3
2 1
-7 0
a(x): 2X^3+-1X^1+5X^0
b(x): 1X^4+3X^3+2X^1+-7X^0
a + b: 1X^4+5X^3+1X^1+-2X^0
a * b: 6.34788e-43X^-1048334848+6.34788e-43X^-1048341152+-1.5884e+38X^-17891602+-1.5884e+38X^-17891602+-1.5884e+38X^-17891602+-1.5884e+38X^-17891602+-1.5884e+38X^-17891602
a(2) = 19
b(2) = 0
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
