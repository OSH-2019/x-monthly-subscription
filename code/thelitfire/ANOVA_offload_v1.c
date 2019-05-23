#include <linux/bpf.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/if_ether.h>
#include <linux/in.h>
#include <stddef.h>
#include <stdbool.h>
#include "bpf_helpers.h"
#include "bpf_endian.h"

#define MAGIC 0x216C7174786A7A21ULL

struct packet_struct {
    __u64 magic;
    __u64 tag;
    __u32 data[16];
};
/*
static __always_inline __u32 double_to_u32(__u64 x){
    return ((x==(__u64)0)?(__u32)0:(__u32)(((x&(__u64)0x000FFFFFFFFFFFFF)|(__u64)0x0010000000000000)>>(((__u32)1065>(__u32)(x >> 52))?(__u32)1065-(__u32)(x >> 52):(__u32)(x >> 52)-(__u32)1065)));
}
*/
/*
static __always_inline __u64 mul2(__u32 a){
    __u16 a2=(a>>16)&(__u32)65535,a1=((a<<16)>>16)&(__u32)65535;
    return (__u64)a2*a1;
}
static __always_inline __u64 mul3(__u32 a){
    __u16 a2=(a>>16)&(__u32)65535;
    return (__u64)a2*a2;
}
static __always_inline __u64 mul1(__u32 a){
    __u16 a1=((a<<16)>>16)&(__u32)65535;
    return (__u64)a1*a1;
}
*/
SEC("xdp")
int process_packet(struct xdp_md *ctx)
{
    void *data_end = (void *)(long)ctx->data_end;
	void *data = (void *)(long)ctx->data;
    __u32 off = sizeof(struct ethhdr) + 28;
    struct packet_struct *raw;
    raw = data + off;
    if (raw + 1 > data_end) return XDP_PASS;
    if (raw->magic != MAGIC) {
        return XDP_PASS;
    } else {
        if (raw->tag != 0) return XDP_PASS;
        //for first 16 data (type __u32), calculate mean and var
        __u32 mean=(__u32)0;
        __u64 var=(__u64)0;
        mean+=raw->data[0];var+=raw->data[0]*raw->data[0];
        mean+=raw->data[1];var+=raw->data[1]*raw->data[1];
        mean+=raw->data[2];var+=raw->data[2]*raw->data[2];
        mean+=raw->data[3];var+=raw->data[3]*raw->data[3];
        mean+=raw->data[4];var+=raw->data[4]*raw->data[4];
        mean+=raw->data[5];var+=raw->data[5]*raw->data[5];
        mean+=raw->data[6];var+=raw->data[6]*raw->data[6];
        mean+=raw->data[7];var+=raw->data[7]*raw->data[7];
        mean>>=3;var>>=3;var-=mean*mean;
        const __u32 e=0x66;
        const __u16 e2=0x28A4;
        __u16 tot=(__u32)0;

        tot+=( raw->data[8]>=mean ? (raw->data[8]-mean>=e ? (__u16)1 : (__u16)0) : (mean-raw->data[8]>=e ? (__u16)1 : (__u16)0) );
        tot+=( raw->data[9]>=mean ? (raw->data[9]-mean>=e ? (__u16)1 : (__u16)0) : (mean-raw->data[9]>=e ? (__u16)1 : (__u16)0) );
        tot+=( raw->data[10]>=mean ? (raw->data[10]-mean>=e ? (__u16)1 : (__u16)0) : (mean-raw->data[10]>=e ? (__u16)1 : (__u16)0) );
        tot+=( raw->data[11]>=mean ? (raw->data[11]-mean>=e ? (__u16)1 : (__u16)0) : (mean-raw->data[11]>=e ? (__u16)1 : (__u16)0) );
        tot+=( raw->data[12]>=mean ? (raw->data[12]-mean>=e ? (__u16)1 : (__u16)0) : (mean-raw->data[12]>=e ? (__u16)1 : (__u16)0) );
        tot+=( raw->data[13]>=mean ? (raw->data[13]-mean>=e ? (__u16)1 : (__u16)0) : (mean-raw->data[13]>=e ? (__u16)1 : (__u16)0) );
        tot+=( raw->data[14]>=mean ? (raw->data[14]-mean>=e ? (__u16)1 : (__u16)0) : (mean-raw->data[14]>=e ? (__u16)1 : (__u16)0) );
        tot+=( raw->data[15]>=mean ? (raw->data[15]-mean>=e ? (__u16)1 : (__u16)0) : (mean-raw->data[15]>=e ? (__u16)1 : (__u16)0) );
        if ((__u64)((__u32)tot*e2) > (var<<3)){
            raw->tag = (__u64)1;// hua fen！
        }else{
            raw->tag = (__u64)2;// no hua fen
        }
    }
    return XDP_PASS;
}
    