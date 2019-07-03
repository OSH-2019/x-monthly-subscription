#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
int main(void){
	freopen("data_000001SZ.txt","r",stdin);
	freopen("data_arranged_000001SZ.txt","w",stdout);
	double x[16];
	for (int i=0;i<16;++i){
		if (scanf("%lf",x+i)==EOF) return 255;
	}
	for (int i=0;i<15;++i) printf("%.3lf ",x[i]);printf("%.3lf\n",x[15]);
	do{
		for (int i=0;i<15;++i) x[i]=x[i+1];
		for (int i=15;i<16;++i) if (scanf("%lf",x+i)==EOF) return 0;
		for (int i=0;i<15;++i) printf("%.3lf ",x[i]);printf("%.3lf\n",x[15]);
	}while (1);
	return 0;
} 
