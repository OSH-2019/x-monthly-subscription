#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<time.h>
// double2u64.c
// - CA
//
// __u64 & double - 64bits
// double
// 63    | 62            52 | 51     0 |
// sign  | 11 bits exponent | mantissa |
// ull x
// 63               29           14         0
// 0 ......        2^15         2^0       2^-14
// 
// TEST
// printf("%llx\n", *((__u64*)&data));
// printf("%lf\n", *((double*)&exponent));

// ZJX: TEST ACcepted;
typedef unsigned __u32;
typedef unsigned long long __u64;
__u32 double_to_u32(__u64 data){
    __u32 exponent;
    __u64 x;
    if (data==(__u64)0) x=(__u64)0;
    exponent=(__u32)(data >> 52)-(__u32)1023;
    x=data&(__u64)0x000FFFFFFFFFFFFF|(__u64)0x0010000000000000;
    if ((__u32)38-exponent>(__u32)0) x>>=(__u32)38-exponent;
    else x<<=exponent-(__u32)38;
    return (__u32)x;
}

int main(void){
	srand(time(0));
	for (int i=0;i<10000;++i){
		double a=(double)rand()+(double)rand()/32768.0;
		freopen("1.txt","wb",stdout);
		fwrite(&a,sizeof(double),(size_t)1,stdout);
		fclose(stdout);
		freopen("1.txt","rb",stdin);
		__u64 b;fread(&b,sizeof(__u64),(size_t)1,stdin);
		__u32 x=double_to_u32(b);
		fclose(stdin); 
		if ((__u32)(a*16384.0) != x) return 255;
	}
	return 0;
}
