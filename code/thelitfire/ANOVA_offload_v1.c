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
//constant//
#define MAGIC 0x216C7174786A7A21ULL
//type//
struct packet_struct {
    __u64 magic;
    __u64 tag;
    __u64 data[32];
};
//functions//
/*
double_to_u32: Programmed by censyu
double:
63    | 62            52 | 51     0 |
sign  | 11 bits exponent | mantissa |
__u32 x:
31               25           10         0
0 ......        2^15         2^0       2^-10
*/
static __always_inline __u32 double_to_u32(__u64 data_double){
    __u32 exponent;
    __u64 x;
    if (data_double==(__u64)0) x=(__u64)0;
    exponent=(__u32)(data_double >> 52)-(__u32)1023;
    x=data_double&(__u64)0x000FFFFFFFFFFFFF|(__u64)0x0010000000000000;
    if ((__u32)42-exponent>(__u32)0) x>>=(__u32)42-exponent;
    else x<<=exponent-(__u32)42;
    return (__u32)x;
}

static __always_inline bool parse_fjw(void *data, __u64 off, void *data_end){
    struct packet_struct *raw;
    raw = data + off;
    if (raw + 1 > data_end) return false;
    if (raw->magic != MAGIC) {  // !fjwtql!
        return false;
    } else {
        if (raw->tag != 0) return false;
//for first 16 data (type __u32), calculate mean and var
        __u32 dt,mean;
        __u64 var;
        #pragma clang loop unroll(full)
        for (int i = 0; i < 16; i++) {
            dt=double_to_u32(raw->data[i]);
            mean+=dt;
            var+=(__u64)dt*(__u64)dt;
        }
        mean>>=4;var>>=4;
        var-=(__u64)mean*mean;
        const __u32 e=0x66;
        const __u64 e2=0x28A4;
        __u32 tot;
        #pragma clang loop unroll(full)
        for (int i=16;i<32;++i){
            dt=double_to_u32(raw->data[i]);
            tot+=( dt>=mean ? (dt-mean>=e ? (__u32)1 : (__u32)0) : (mean-dt>=e ? (__u32)1 : (__u32)0) );
        }
        if ((__u64)tot*e2 > (var<<4)){
            raw->tag = (__u64)1;// hua fen！
        }else{
            raw->tag = (__u64)2;// no hua fen
        }
    }
    return true;
}
/*
static __always_inline int modify_tag(void *data, __u64 off, void *data_end) {
    struct packet_struct *raw;

    raw = data + off;
    if (raw + 1 > data_end)
        return false;

    if (raw->magic != MAGIC) {
        return false;
    } else { 
        raw->tag = 0;
        return XDP_TX;
    }
}
*/
SEC("xdp")
int process_packet(struct xdp_md *ctx)
{
    void *data_end = (void *)(long)ctx->data_end;
	void *data = (void *)(long)ctx->data;
    __u32 off = sizeof(struct ethhdr) + 28;
    if (!parse_fjw(data, off, data_end, &dt)) {
        return XDP_DROP;
    }
    return XDP_PASS;
}