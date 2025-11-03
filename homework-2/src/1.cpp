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


Polynomial Polynomial::Add(const Polynomial b){
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

Polynomial Polynomial::Mult(const Polynomial b){
    Polynomial c;

	for(int i=0;i<terms;i++)
	{
		Polynomial d;
		for(int j=0;j<b.terms;j++)
		{
			float coef = termArray[i].coef * b.termArray[j].coef;
			int exp = termArray[i].exp + b.termArray[j].exp;
			d.newTerm(coef, exp);
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