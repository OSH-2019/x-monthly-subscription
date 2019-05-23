	.text
	.section	xdp,"ax",@progbits
	.globl	process_packet          # -- Begin function process_packet
	.p2align	3
process_packet:                         # @process_packet
	r2 = *(u32 *)(r1 + 4)
	r1 = *(u32 *)(r1 + 0)	# r1: data package pointer
	r3 = r1
	r3 += 122
	if r3 > r2 goto LBB_RETURN
	r2 = *(u64 *)(r1 + 42)
	r3 = 2408424645786761761 ll
	if r2 != r3 goto LBB_RETURN
	r2 = *(u64 *)(r1 + 50)
	if r2 != 0 goto LBB_RETURN
	r4 = *(u32 *)(r1 + 58)	# data[0]
	r2 = *(u32 *)(r1 + 62)	# data[1]
	w3 = w2					
	w3 += w4				# w3: mean
	r2 *= r2
	r4 *= r4
	r2 += r4				# r2: var
	r5 = *(u32 *)(r1 + 66)	# data[2]
	w3 += w5
	r5 *= r5
	r2 += r5
	r4 = *(u32 *)(r1 + 70)	# data[3]
	w3 += w4
	r4 *= r4
	r2 += r4
	r6 = *(u32 *)(r1 + 74)	# data[4]
	w3 += w6
	r6 *= r6
	r2 += r6
	r7 = *(u32 *)(r1 + 78)	# data[5]
	w3 += w7
	r7 *= r7
	r2 += r7
	r8 = *(u32 *)(r1 + 82)	# data[6]
	w3 += w8
	r8 *= r8
	r2 += r8
	r9 = *(u32 *)(r1 + 86)	# data[7]
	w3 += w9
	r9 *= r9
	r2 += r9
	r3 >>= 3	# w3: mean
	r2 >>= 3
	w4 = w3
	r4 *= r4
	r2 -= r4	# r2: var

	r5 = *(u32 *)(r1 + 90)	# data[8]
	w0 = w3
	w0 -= w5
	if r3 > r5 goto ABS_DT8
	w5 -= w3
	w0 = w5
ABS_DT8:
	w4 = 0
	if r0 < 102 goto LD_DT9
	w4 = 1
LD_DT9:
	r6 = *(u32 *)(r1 + 94)	# data[9]
	w7 = w3
	w7 -= w6
	if r3 > r6 goto ABS_DT9
	w6 -= w3
	w7 = w6
ABS_DT9:
	if r7 < 102 goto LD_DT10
	w4 += 1
LD_DT10:
	r8 = *(u32 *)(r1 + 98)	# data[10]
	w9 = w3
	w9 -= w8
	if r3 > r8 goto ABS_DT10
	w8 -= w3
	w9 = w8
ABS_DT10:
	if r9 < 102 goto LD_DT11
	w4 +=1
LD_DT11:
	r5 = *(u32 *)(r1 + 102)	# data[11]
	w0 = w3
	w0 -= w5
	if r3 > r5 goto ABS_DT11
	w5 -= w3
	w0 = w5
ABS_DT11:
	if r0 < 102 goto LD_DT12
	w4 += 1
LD_DT12:
	r6 = *(u32 *)(r1 + 106)	# data[12]
	w7 = w3
	w7 -= w6
	if r3 > r6 goto ABS_DT12
	w6 -= w3
	w7 = w6
ABS_DT12:
	if r7 < 102 goto LD_DT13
	w4 += 1
LD_DT13:
	r8 = *(u32 *)(r1 + 110)	# data[13]
	w9 = w3
	w9 -= w8
	if r3 > r8 goto ABS_DT13
	w8 -= w3
	w9 = w8
ABS_DT13:
	if r9 < 102 goto LD_DT14
	w4 +=1
LD_DT14:
	r5 = *(u32 *)(r1 + 114)	# data[14]
	w0 = w3
	w0 -= w5
	if r3 > r5 goto ABS_DT14
	w5 -= w3
	w0 = w5
ABS_DT14:
	if r0 < 102 goto LD_DT15
	w4 += 1
LD_DT15:
	r6 = *(u32 *)(r1 + 118)	# data[15]
	w7 = w3
	w7 -= w6
	if r3 > r6 goto ABS_DT15
	w6 -= w3
	w7 = w6
ABS_DT15:
	if r7 < 102 goto JUDGE
	w4 += 1
JUDGE:
	r4 *= 10404
	r2 <<= 3
	r3 = 1
	if r4 > r2 goto JIEGUO
	r3 = 2	
JIEGUO:
	*(u64 *)(r1 + 50) = r3
LBB_RETURN:
	w0 = 2
	exit
                                        # -- End function

