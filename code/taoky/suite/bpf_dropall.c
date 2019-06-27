#include <linux/bpf.h>
#include "bpf_helpers.h"

SEC("xdp")
int process_packet(struct xdp_md *ctx) {
	return XDP_DROP;
}
