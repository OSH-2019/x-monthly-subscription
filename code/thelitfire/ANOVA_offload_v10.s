	.text
	.section	xdp,"ax",@progbits
	.globl	process_packet          # -- Begin function process_packet
	.p2align	3
process_packet:                         # @process_packet
#   This is the ANOVA version 1.0
#   compile steps: ANOVA_offload_v1.c
#       clang -emit-llvm -O2 -S A.c
#       llc -mattr=+alu32 A.ll
#       llvm-mc -triple bpf -filetype=obj -o A.o A.s
#   verifier error:
#       w3 >>= 3; Instruction not supported

# %bb.0:
	w2 = *(u32 *)(r1 + 4)
	w1 = *(u32 *)(r1 + 0)
	r3 = r1
	r3 += 122
	if r3 > r2 goto LBB0_38
# %bb.1:
	r2 = *(u64 *)(r1 + 42)
	r3 = 2408424645786761761 ll
	if r2 != r3 goto LBB0_38
# %bb.2:
	r2 = *(u64 *)(r1 + 50)
	if r2 != 0 goto LBB0_38
# %bb.3:
	w4 = *(u32 *)(r1 + 58)
	w2 = *(u32 *)(r1 + 62)
	w3 = w2
	*(u64 *)(r10 - 56) = r4
	w3 += w4
	w4 = *(u32 *)(r1 + 66)
	*(u64 *)(r10 - 48) = r4
	w3 += w4
	w4 = *(u32 *)(r1 + 70)
	*(u64 *)(r10 - 40) = r4
	w3 += w4
	w4 = *(u32 *)(r1 + 74)
	*(u64 *)(r10 - 32) = r4
	w3 += w4
	w4 = *(u32 *)(r1 + 78)
	*(u64 *)(r10 - 24) = r4
	w3 += w4
	w4 = *(u32 *)(r1 + 82)
	*(u64 *)(r10 - 16) = r4
	w3 += w4
	w4 = *(u32 *)(r1 + 86)
	*(u64 *)(r10 - 8) = r4
	w3 += w4
	w3 >>= 3
	w4 = *(u32 *)(r1 + 118)
	w5 = w3
	w5 -= w4
	if r3 > r4 goto LBB0_5
# %bb.4:
	w4 -= w3
	w5 = w4
LBB0_5:
	w4 = *(u32 *)(r1 + 106)
	w0 = w3
	w0 -= w4
	if r3 > r4 goto LBB0_7
# %bb.6:
	w4 -= w3
	w0 = w4
LBB0_7:
	w4 = *(u32 *)(r1 + 110)
	w6 = w3
	w6 -= w4
	if r3 > r4 goto LBB0_9
# %bb.8:
	w4 -= w3
	w6 = w4
LBB0_9:
	w4 = *(u32 *)(r1 + 114)
	w7 = w3
	w7 -= w4
	if r3 > r4 goto LBB0_11
# %bb.10:
	w4 -= w3
	w7 = w4
LBB0_11:
	w4 = *(u32 *)(r1 + 102)
	w8 = w3
	w8 -= w4
	if r3 > r4 goto LBB0_13
# %bb.12:
	w4 -= w3
	w8 = w4
LBB0_13:
	*(u64 *)(r10 - 64) = r5
	w4 = *(u32 *)(r1 + 98)
	w9 = w3
	w9 -= w4
	if r3 > r4 goto LBB0_15
# %bb.14:
	w4 -= w3
	w9 = w4
LBB0_15:
	*(u64 *)(r10 - 72) = r0
	w5 = *(u32 *)(r1 + 90)
	w4 = w3
	w4 -= w5
	if r3 > r5 goto LBB0_17
# %bb.16:
	w5 -= w3
	w4 = w5
LBB0_17:
	w0 = *(u32 *)(r1 + 94)
	w5 = w3
	w5 -= w0
	if r3 > r0 goto LBB0_19
# %bb.18:
	w0 -= w3
	w5 = w0
LBB0_19:
	w0 = 1
	if r5 > 101 goto LBB0_21
# %bb.20:
	w0 = 0
LBB0_21:
	w5 = 1
	if r4 > 101 goto LBB0_23
# %bb.22:
	w5 = 0
LBB0_23:
	*(u64 *)(r10 - 88) = r5
	w5 = 1
	r4 = *(u64 *)(r10 - 64)
	if r9 > 101 goto LBB0_25
# %bb.24:
	w5 = 0
LBB0_25:
	w9 = 1
	if r8 > 101 goto LBB0_27
# %bb.26:
	w9 = 0
LBB0_27:
	w8 = 1
	if r7 > 101 goto LBB0_29
# %bb.28:
	w8 = 0
LBB0_29:
	*(u64 *)(r10 - 80) = r8
	r8 = r5
	w7 = 1
	if r6 > 101 goto LBB0_31
# %bb.30:
	w7 = 0
LBB0_31:
	w6 = 1
	r5 = *(u64 *)(r10 - 72)
	if r5 > 101 goto LBB0_33
# %bb.32:
	w6 = 0
LBB0_33:
	w5 = 1
	if r4 > 101 goto LBB0_35
# %bb.34:
	w5 = 0
LBB0_35:
	r4 = *(u64 *)(r10 - 56)
	r4 *= r4
	r2 *= r2
	r2 += r4
	r4 = *(u64 *)(r10 - 48)
	r4 *= r4
	r2 += r4
	r4 = *(u64 *)(r10 - 40)
	r4 *= r4
	r2 += r4
	r4 = *(u64 *)(r10 - 32)
	r4 *= r4
	r2 += r4
	r4 = *(u64 *)(r10 - 88)
	r0 += r4
	r4 = *(u64 *)(r10 - 24)
	r4 *= r4
	r2 += r4
	r0 += r8
	r4 = *(u64 *)(r10 - 16)
	r4 *= r4
	r2 += r4
	r0 += r9
	r4 = *(u64 *)(r10 - 8)
	r4 *= r4
	r2 += r4
	r0 += r6
	r0 += r7
	r4 = *(u64 *)(r10 - 80)
	r0 += r4
	w3 *= w3
	r3 <<= 3
	r2 -= r3
	r0 += r5
	r2 &= -8
	r0 &= 65535
	r0 *= 10404
	r3 = 1
	if r0 > r2 goto LBB0_37
# %bb.36:
	r3 = 2
LBB0_37:
	*(u64 *)(r1 + 50) = r3
LBB0_38:
	w0 = 2
	exit
                                        # -- End function

