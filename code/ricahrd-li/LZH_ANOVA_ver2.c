#include<linux/bpf.h>

__u32 calculate_mean( __u64 x[]){
    //循环展开
    __u32 mean=(x[0]+x[1]+x[2]+x[3]+x[4]+x[5]+x[6]+x[7]+x[8]+x[9]+x[10]+x[11]+x[12]+x[13]+x[14]+x[15])>>4;
    return mean;
}

__u64 calculate_var(__u64 x[],__u32 mean){

   __u64 var=((x[0]*x[0]+x[1]*x[1]+x[2]*x[2]+x[3]*x[3]+x[4]*x[4]
       +x[5]*x[5]+x[6]*x[6]+x[7]*x[7]+x[8]*x[8]+x[9]*x[9]
       +x[10]*x[10]+x[11]*x[11]+x[12]*x[12]+x[13]*x[13]+x[14]*x[14]
       +x[15]*x[15])>>4)-(__u64)mean*mean;
   return var;
}

static __always_inline int Judge(__u64 x[],__u32 mean,__u64 var){
    const __u16 e=0x66;
    __u64 abs[16]; //使用abs[16]而不是单个abs原因：
    //使用单个abs: clang报错：warning: multiple unsequenced modifications to 'abs' [-Wunsequenced]
    //而且改为16个abs可能有利于利用数据流架构
    __u8 tot=(
            ( ( ( ( (abs[0]=x[16]-mean)>0? abs[0]: -abs[0]) )>=e )? 1 : 0 )+
            ( ( ( ( (abs[1]=x[17]-mean)>0? abs[1]: -abs[1]) )>=e )? 1 : 0 )+
            ( ( ( ( (abs[2]=x[18]-mean)>0? abs[2]: -abs[2]) )>=e )? 1 : 0 )+
            ( ( ( ( (abs[3]=x[19]-mean)>0? abs[3]: -abs[3]) )>=e )? 1 : 0 )+
            ( ( ( ( (abs[4]=x[20]-mean)>0? abs[4]: -abs[4]) )>=e )? 1 : 0 )+
            ( ( ( ( (abs[5]=x[21]-mean)>0? abs[5]: -abs[5]) )>=e )? 1 : 0 )+
            ( ( ( ( (abs[6]=x[22]-mean)>0? abs[6]: -abs[6]) )>=e )? 1 : 0 )+
            ( ( ( ( (abs[7]=x[23]-mean)>0? abs[7]: -abs[7]) )>=e )? 1 : 0 )+
            ( ( ( ( (abs[8]=x[24]-mean)>0? abs[8]: -abs[8]) )>=e )? 1 : 0 )+
            ( ( ( ( (abs[9]=x[25]-mean)>0? abs[9]: -abs[9]) )>=e )? 1 : 0 )+
            ( ( ( ( (abs[10]=x[26]-mean)>0? abs[10]: -abs[10]) )>=e )? 1 : 0 )+
            ( ( ( ( (abs[11]=x[27]-mean)>0? abs[11]: -abs[11]) )>=e )? 1 : 0 )+
            ( ( ( ( (abs[12]=x[28]-mean)>0? abs[12]: -abs[12]) )>=e )? 1 : 0 )+
            ( ( ( ( (abs[13]=x[29]-mean)>0? abs[13]: -abs[13]) )>=e )? 1 : 0 )+
            ( ( ( ( (abs[14]=x[30]-mean)>0? abs[14]: -abs[14]) )>=e )? 1 : 0 )+
            ( ( ( ( (abs[15]=x[31]-mean)>0? abs[15]: -abs[15]) )>=e )? 1 : 0 )
            );
 
    return (tot*e*e-(__u64)16*var)>0 ? 1 :0 ; //return 1: 划分，0:不划分
}
