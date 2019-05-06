#include<linux/bpf.h>

int main(){
    __u64 x[32]; //输入的数据
    __u32 mean=0;
    __u64 var=0;
    //循环展开
    mean=(x[0]+x[1]+x[2]+x[3]+x[4]+x[5]+x[6]+x[7]+x[8]+x[9]+x[10]+x[11]+x[12]+x[13]+x[14]+x[15]+x[16]+
         x[17]+x[18]+x[19]+x[20]+x[21]+x[22]+x[23]+x[24]+x[25]+x[26]+x[27]+x[28]+x[29]+x[30]+x[31])>>4;

    var=((x[0]*x[0]+x[1]*x[1]+x[2]*x[2]+x[3]*x[3]+x[4]*x[4]
       +x[5]*x[5]+x[6]*x[6]+x[7]*x[7]+x[8]*x[8]+x[9]*x[9]
       +x[10]*x[10]+x[11]*x[11]+x[12]*x[12]+x[13]*x[13]+x[14]*x[14]
       +x[15]*x[15]+x[16]*x[16]+x[17]*x[17]+x[18]*x[18]+x[19]*x[19]
       +x[20]*x[20]+x[21]*x[21]+x[22]*x[22]+x[23]*x[23]+x[24]*x[24]
       +x[25]*x[25]+x[26]*x[26]+x[27]*x[27]+x[28]*x[28]+x[29]*x[29]
       +x[30]*x[30]+x[31]*x[31])>>4)-(__u64)mean*mean;
    
    return 0;
}


/*
[osh@agilio1415 ~]$ clang  -target bpf -c LZH_ANOVA_v2.c -o LZH_test.o
[osh@agilio1415 ~]$ llvm-objdump-4.0 -S LZH_test.o

LZH_test.o:     file format ELF64-BPF

Disassembly of section .text:
main:
       0:       b7 00 00 00 00 00 00 00         r0 = 0
       1:       95 00 00 00 00 00 00 00         exit
*/
