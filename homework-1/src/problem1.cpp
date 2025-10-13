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
    }else return -1;	
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
