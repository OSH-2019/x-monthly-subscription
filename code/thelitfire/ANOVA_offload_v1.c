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
    __u64 data[32];
};
static __always_inline __u32 double_to_u32(__u64 x){
    return ((x==(__u64)0)?(__u32)0:(__u32)(((x&(__u64)0x000FFFFFFFFFFFFF)|(__u64)0x0010000000000000)>>(((__u32)1065>(__u32)(x >> 52))?(__u32)1065-(__u32)(x >> 52):(__u32)(x >> 52)-(__u32)1065)))
}
SEC("xdp")
int process_packet(struct xdp_md *ctx)
{
    void *data_end = (void *)(long)ctx->data_end;
	void *data = (void *)(long)ctx->data;
    __u32 off = sizeof(struct ethhdr) + 28;
    struct packet_struct *raw;
    raw = data + off;
    if (raw + 1 > data_end) return XDP_DROP;
    if (raw->magic != MAGIC) {  // !fjwtql!
        return XDP_DROP;
    } else {
        if (raw->tag != 0) return XDP_DROP;
        //for first 16 data (type __u32), calculate mean and var
        __u32 dt,mean=(__u32)0;
        __u64 var=(__u64)0;
        dt=double_to_u32(raw->data[0]);mean+=dt;var+=(__u64)dt*(__u64)dt;
        dt=double_to_u32(raw->data[1]);mean+=dt;var+=(__u64)dt*(__u64)dt;
        dt=double_to_u32(raw->data[2]);mean+=dt;var+=(__u64)dt*(__u64)dt;
        dt=double_to_u32(raw->data[3]);mean+=dt;var+=(__u64)dt*(__u64)dt;
        dt=double_to_u32(raw->data[4]);mean+=dt;var+=(__u64)dt*(__u64)dt;
        dt=double_to_u32(raw->data[5]);mean+=dt;var+=(__u64)dt*(__u64)dt;
        dt=double_to_u32(raw->data[6]);mean+=dt;var+=(__u64)dt*(__u64)dt;
        dt=double_to_u32(raw->data[7]);mean+=dt;var+=(__u64)dt*(__u64)dt;
        dt=double_to_u32(raw->data[8]);mean+=dt;var+=(__u64)dt*(__u64)dt;
        dt=double_to_u32(raw->data[9]);mean+=dt;var+=(__u64)dt*(__u64)dt;
        dt=double_to_u32(raw->data[10]);mean+=dt;var+=(__u64)dt*(__u64)dt;
        dt=double_to_u32(raw->data[11]);mean+=dt;var+=(__u64)dt*(__u64)dt;
        dt=double_to_u32(raw->data[12]);mean+=dt;var+=(__u64)dt*(__u64)dt;
        dt=double_to_u32(raw->data[13]);mean+=dt;var+=(__u64)dt*(__u64)dt;
        dt=double_to_u32(raw->data[14]);mean+=dt;var+=(__u64)dt*(__u64)dt;
        dt=double_to_u32(raw->data[15]);mean+=dt;var+=(__u64)dt*(__u64)dt;
        mean>>=4;var>>=4;var-=(__u64)mean*mean;
        const __u32 e=0x66;
        const __u64 e2=0x28A4;
        __u32 tot=(__u32)0;

        dt=double_to_u32(raw->data[16]);tot+=( dt>=mean ? (dt-mean>=e ? (__u32)1 : (__u32)0) : (mean-dt>=e ? (__u32)1 : (__u32)0) );
        dt=double_to_u32(raw->data[17]);tot+=( dt>=mean ? (dt-mean>=e ? (__u32)1 : (__u32)0) : (mean-dt>=e ? (__u32)1 : (__u32)0) );
        dt=double_to_u32(raw->data[18]);tot+=( dt>=mean ? (dt-mean>=e ? (__u32)1 : (__u32)0) : (mean-dt>=e ? (__u32)1 : (__u32)0) );
        dt=double_to_u32(raw->data[19]);tot+=( dt>=mean ? (dt-mean>=e ? (__u32)1 : (__u32)0) : (mean-dt>=e ? (__u32)1 : (__u32)0) );
        dt=double_to_u32(raw->data[20]);tot+=( dt>=mean ? (dt-mean>=e ? (__u32)1 : (__u32)0) : (mean-dt>=e ? (__u32)1 : (__u32)0) );
        dt=double_to_u32(raw->data[21]);tot+=( dt>=mean ? (dt-mean>=e ? (__u32)1 : (__u32)0) : (mean-dt>=e ? (__u32)1 : (__u32)0) );
        dt=double_to_u32(raw->data[22]);tot+=( dt>=mean ? (dt-mean>=e ? (__u32)1 : (__u32)0) : (mean-dt>=e ? (__u32)1 : (__u32)0) );
        dt=double_to_u32(raw->data[23]);tot+=( dt>=mean ? (dt-mean>=e ? (__u32)1 : (__u32)0) : (mean-dt>=e ? (__u32)1 : (__u32)0) );
        dt=double_to_u32(raw->data[24]);tot+=( dt>=mean ? (dt-mean>=e ? (__u32)1 : (__u32)0) : (mean-dt>=e ? (__u32)1 : (__u32)0) );
        dt=double_to_u32(raw->data[25]);tot+=( dt>=mean ? (dt-mean>=e ? (__u32)1 : (__u32)0) : (mean-dt>=e ? (__u32)1 : (__u32)0) );
        dt=double_to_u32(raw->data[26]);tot+=( dt>=mean ? (dt-mean>=e ? (__u32)1 : (__u32)0) : (mean-dt>=e ? (__u32)1 : (__u32)0) );
        dt=double_to_u32(raw->data[27]);tot+=( dt>=mean ? (dt-mean>=e ? (__u32)1 : (__u32)0) : (mean-dt>=e ? (__u32)1 : (__u32)0) );
        dt=double_to_u32(raw->data[28]);tot+=( dt>=mean ? (dt-mean>=e ? (__u32)1 : (__u32)0) : (mean-dt>=e ? (__u32)1 : (__u32)0) );
        dt=double_to_u32(raw->data[29]);tot+=( dt>=mean ? (dt-mean>=e ? (__u32)1 : (__u32)0) : (mean-dt>=e ? (__u32)1 : (__u32)0) );
        dt=double_to_u32(raw->data[30]);tot+=( dt>=mean ? (dt-mean>=e ? (__u32)1 : (__u32)0) : (mean-dt>=e ? (__u32)1 : (__u32)0) );
        dt=double_to_u32(raw->data[31]);tot+=( dt>=mean ? (dt-mean>=e ? (__u32)1 : (__u32)0) : (mean-dt>=e ? (__u32)1 : (__u32)0) );

        if ((__u64)tot*e2 > (var<<4)){
            raw->tag = (__u64)1;// hua fen！
        }else{
            raw->tag = (__u64)2;// no hua fen
        }
    }
    return XDP_PASS;
}
    