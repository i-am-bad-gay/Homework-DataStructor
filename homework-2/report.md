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

1. 程式能正確執行多項式的 輸入、輸出、加法、乘法與代入運算，在正常輸入條件下運作正確。 
2. 加法部分能正確合併同次方項，乘法部分亦能產生正確的高次多項式結果。
3. 當多項式項數過多或次方過高時，若使用動態陣列記憶體不足，可能會導致輸出異常。

## 申論及開發報告

### 件導向設計概念的應用

本程式以 Term（單項式）與 Polynomial（多項式）為兩個類別核心。

1.  Term 專責儲存「係數與指數」。

2.  Polynomial 負責運算與結構管理。

這樣的封裝（Encapsulation）設計讓程式結構清晰、易於維護，也體現了物件導向的「資料抽象化」與「模組化」原則。
### 程式邏輯與功能分析

1. 多項式加法的設計

設計理念：
利用「雙指標」的方式，比較兩個多項式每一項的指數，若指數相同則合併，若不同則依照指數大小插入結果中。
```cpp
Polynomial Polynomial::Add(Polynomial b) {
    Polynomial c;
    int aPos = 0, bPos = 0;
    while (aPos < terms && bPos < b.terms) {
        if (termArray[aPos].exp == b.termArray[bPos].exp) {
            float t = termArray[aPos].coef + b.termArray[bPos].coef;
            if (t) c.newTerm(t, termArray[aPos].exp);
            aPos++; bPos++;
        } else if (termArray[aPos].exp < b.termArray[bPos].exp) {
            c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
            bPos++;
        } else {
            c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
            aPos++;
        }
    }
    for (; aPos < terms; aPos++) c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
    for (; bPos < b.terms; bPos++) c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
    return c;
}
```

2. 多項式乘法的設計
設計理念：
利用兩層迴圈，讓每一個項次兩兩相乘，並使用 Add() 函式整合相同次方項。
```cpp
Polynomial Polynomial::Mult(const Polynomial& b) const {
    Polynomial c;
    for (int i = 0; i < terms; i++) {
        Polynomial temp;
        for (int j = 0; j < b.terms; j++) {
            float coef = termArray[i].coef * b.termArray[j].coef;
            int exp = termArray[i].exp + b.termArray[j].exp;
            temp.newTerm(coef, exp);
        }
        c = c.Add(temp); // 合併結果
    }
    return c;
}
```

3. 多項式代入運算 (eval)

設計理念：
以迴圈遍歷所有項，再加總。
```cpp
float Polynomial::eval(float x) const {
    float result = 0;
    for (int i = 0; i < terms; i++) {
        result += termArray[i].coef * pow(x, termArray[i].exp);
    }
    return result;
}
```