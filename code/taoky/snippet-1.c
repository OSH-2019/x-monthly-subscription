#include <linux/bpf.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/if_ether.h>
#include <linux/in.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "bpf_helpers.h"
#include "bpf_endian.h"

int test[100];

struct data {
    __u64 data[32];
};

struct packet_struct {
    __u64 magic;
    __u64 tag;
    __u64 data[32];
};

struct pkt_meta {
	// union {
		__be32 src;
		// __be32 srcv6[4];
	// };
	// union {
		__be32 dst;
		// __be32 dstv6[4];
	// };
	// __u16 port16[2];
	__u16 l3_proto;
	__u16 l4_proto;
	__u16 data_len;
	__u16 pkt_len;
	__u32 seq;
};


/* parser from official xdp example */
/* assuming: ipv4 & udp */
static __always_inline bool parse_udp(void *data, __u64 off, void *data_end)
{
	struct udphdr *udp;

	udp = data + off;
	if (udp + 1 > data_end)
		return false;

	// pkt->port16[0] = udp->source;
	// pkt->port16[1] = udp->dest;
	return true;
}

static __always_inline bool parse_ip4(void *data, __u64 off, void *data_end)
{
	struct iphdr *iph;

	iph = data + off;
	if (iph + 1 > data_end)
		return false;

	if (iph->ihl != 5)
		return false;

	// pkt->src = iph->saddr;
	// pkt->dst = iph->daddr;
	// pkt->l4_proto = iph->protocol;

	return true;
}

static __always_inline bool parse_fjw(void *data, __u64 off, void *data_end,
                        struct data *dt)
{
    struct packet_struct *raw;

    raw = data + off;
    if (raw + 1 > data_end)
        return false;

    if (raw->magic != 0x21666A7774716C21) {  // !lqtwjf!
        return false;
    } else {
        raw->magic = 0x21626B73666A7721;  // !bkswjf!
        dt->tag = raw->tag;
        dt->data[0] = raw->data[0];
        dt->data[1] = raw->data[1];
        dt->data[2] = raw->data[2];
        test[0] = raw->data[0];

        int a = 0, b = 5;
        int *ret = bpf_map_lookup_elem(&window_map, &b);
        if (!ret) {
            return true;
        }
        bpf_map_update_elem(&window_map, &a, &b, BPF_ANY);
    }
    return true;
}

int process_packet(struct xdp_md *ctx)
{
    // ctx->rx_queue_index = 1;
    void *data_end = (void *)(long)ctx->data_end;
	void *data = (void *)(long)ctx->data;
    struct ethhdr *eth = data;
    struct pkt_meta pkt = {};
    struct data dt = {};
    __u32 off;

    off = sizeof(struct ethhdr);
    if (data + off > data_end) {
        return XDP_PASS;  // not a valid ethernet packet
    }

    pkt.l3_proto = bpf_htons(eth->h_proto);  // get next layer protocol

    if (pkt.l3_proto == ETH_P_IP) {
        // ipv4
        if (!parse_ip4(data, off, data_end)) {
            return XDP_PASS;  // not a valid ipv4 packet
        }
        off += sizeof(struct iphdr);
    } else {
        return XDP_PASS;  // won't handle ipv6
    }

    if (data + off > data_end) {
        return XDP_PASS;
    }

    if (pkt.l4_proto == IPPROTO_UDP) {
        if (!parse_udp(data, off, data_end)) {
            return XDP_PASS;
        }
        off += sizeof(struct udphdr);
    } else {
        return XDP_PASS;
    }

    pkt.pkt_len = data_end - data;
	pkt.data_len = data_end - data - off;

    if (!parse_fjw(data, off, data_end, &dt)) {
        return XDP_PASS;
    }

    return XDP_PASS;
}

char _license[] SEC("license") = "FJW";