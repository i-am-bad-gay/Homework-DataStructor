#include <iostream>
using namespace std;

class Polynomial;

class Term
{
    friend Polynomial;
    friend ostream& operator<<(ostream &output,const Polynomial &Poly);
    private:
        int exp;
        float coef;
};

class Polynomial{
    private:
    	Term *termArray;
    	int capacity;
    	int terms;
        
    public:
    	Polynomial():capacity(2),terms(0){
    		termArray = new Term[capacity];
    	}
    	~Polynomial(){delete[] termArray;}
    	Polynomial Add(Polynomial b);
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
    if(terms==capacity){
    	capacity*=2;
    	Term *temp=new Term[capacity];
    	copy(termArray,termArray+terms,temp);
    	delete []termArray;
    	termArray=temp;
    }
    termArray[terms].coef=theCoef;
    termArray[terms++].exp=theExp;
}

Polynomial Polynomial::Add(Polynomial b){
    Polynomial c;
    int aPos=0,bPos=0;
    while((aPos<terms)&&(bPos<b.terms)){
    	if(termArray[aPos].exp==b.termArray[bPos].exp){
    		float t=termArray[aPos].coef+b.termArray[bPos].coef;
    		if(t) c.newTerm(t,termArray[aPos].exp);
    		aPos++;bPos++;
    	}
    	else if(termArray[aPos].exp<b.termArray[bPos].exp){
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

int main(){
    Polynomial a,b;
    cin>>a>>b;
    cout<<a.Add(b)<<endl;
}