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

int test[100];

struct data {
    __u64 data[32];
};

struct packet_struct {
    __u64 magic;
    __u64 tag;
    __u64 data[32];
};

static __always_inline bool parse_fjw(void *data, __u64 off, void *data_end,
                        struct data *dt)
{
    struct packet_struct *raw;

    raw = data + off;
    if (raw + 1 > data_end)
        return false;

    if (raw->magic != MAGIC) {  // !fjwtql!
        return false;
    } else {
        if (raw->tag != 0) {
            return false;
        }
        #pragma clang loop unroll(full)
        for (int i = 0; i < 32; i++) {
            dt->data[i] = raw->data[i];
        }
    }
    return true;
}

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

SEC("xdp")
int process_packet(struct xdp_md *ctx)
{
    void *data_end = (void *)(long)ctx->data_end;
	void *data = (void *)(long)ctx->data;
    struct data dt = {};
    __u32 off = sizeof(struct ethhdr) + 28;

    if (!parse_fjw(data, off, data_end, &dt)) {
        return XDP_PASS;
    }

    return XDP_PASS;
}