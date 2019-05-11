#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
int main(void){
	freopen("data_000001SZ.txt","r",stdin);
	freopen("data_arranged_000001SZ.txt","w",stdout);
	double x[32];
	for (int i=0;i<32;++i){
		if (scanf("%lf",x+i)==EOF) return 255;
	}
	for (int i=0;i<31;++i) printf("%.3lf ",x[i]);printf("%.3lf\n",x[31]);
	do{
		for (int i=0;i<31;++i) x[i]=x[i+1];
		for (int i=31;i<32;++i) if (scanf("%lf",x+i)==EOF) return 0;
		for (int i=0;i<31;++i) printf("%.3lf ",x[i]);printf("%.3lf\n",x[31]);
	}while (1);
	return 0;
} 
