	.text
	.section	xdp,"ax",@progbits
	.globl	process_packet          # -- Begin function process_packet
	.p2align	3
process_packet:                         # @process_packet
#   This is the handwriting version of ANOVA 1
#   NO VERIFIER ERROR

	r2 = *(u32 *)(r1 + 4)
	r1 = *(u32 *)(r1 + 0)	# r1: data package pointer
	r3 = r1
	r3 += 186
	if r3 > r2 goto LBB_RETURN
	r2 = *(u64 *)(r1 + 42)
	r3 = 2408424645786761761 ll
	if r2 != r3 goto LBB_RETURN
	r2 = *(u64 *)(r1 + 50)
	if r2 != 0 goto LBB_RETURN
	r4 = *(u64 *)(r1 + 58)	# data[0]
    if r4 == 0 goto CH_DT0_CHUNK
    r0 = 4503599627370495 ll
    r5 = r4
    r5 >>= 52
    w5 -= 1023
    r4 &= r0
    r0 += 1
    r4 |= r0
    if r5 >= 42 goto CH_DT0_B
    w6 = 42
    w6 -= w5
    r4 >>= w6
    goto CH_DT0_CHUNK
CH_DT0_B:
    w5 -= 42
    r4 <<= w5
CH_DT0_CHUNK:
    r4 <<= 32
    r4 >>= 32
	r2 = *(u64 *)(r1 + 66)	# data[1]
    if r2 == 0 goto CH_DT1_CHUNK
    r0 -= 1
    r5 = r2
    r5 >>= 52
    w5 -= 1023
    r2 &= r0
    r0 += 1
    r2 |= r0
    if r5 >= 42 goto CH_DT1_B
    w6 = 42
    w6 -= w5
    r2 >>= w6
    goto CH_DT1_CHUNK
CH_DT1_B:
    w5 -= 42
    r2 <<= w5
CH_DT1_CHUNK:
    r2 <<= 32
    r2 >>= 32
	w3 = w2					
	w3 += w4				# w3: mean
	r2 *= r2
	r4 *= r4
	r2 += r4				# r2: var
	r9 = *(u64 *)(r1 + 74)	# data[2]
    if r9 == 0 goto CH_DT2_CHUNK
    r0 -= 1
    r5 = r9
    r5 >>= 52
    w5 -= 1023
    r9 &= r0
    r0 += 1
    r9 |= r0
    if r5 >= 42 goto CH_DT2_B
    w6 = 42
    w6 -= w5
    r9 >>= w6
    goto CH_DT2_CHUNK
CH_DT2_B:
    w5 -= 42
    r9 <<= w5
CH_DT2_CHUNK:
    r9 <<= 32
    r9 >>= 32
	w3 += w9
	r9 *= r9
	r2 += r9
	r4 = *(u64 *)(r1 + 82)	# data[3]
    if r4 == 0 goto CH_DT3_CHUNK
    r0 -= 1
    r5 = r4
    r5 >>= 52
    w5 -= 1023
    r4 &= r0
    r0 += 1
    r4 |= r0
    if r5 >= 42 goto CH_DT3_B
    w6 = 42
    w6 -= w5
    r4 >>= w6
    goto CH_DT3_CHUNK
CH_DT3_B:
    w5 -= 42
    r4 <<= w5
CH_DT3_CHUNK:
    r4 <<= 32
    r4 >>= 32
	w3 += w4
	r4 *= r4
	r2 += r4
	r9 = *(u64 *)(r1 + 90)	# data[4]
    if r9 == 0 goto CH_DT4_CHUNK
    r0 -= 1
    r5 = r9
    r5 >>= 52
    w5 -= 1023
    r9 &= r0
    r0 += 1
    r9 |= r0
    if r5 >= 42 goto CH_DT4_B
    w6 = 42
    w6 -= w5
    r9 >>= w6
    goto CH_DT4_CHUNK
CH_DT4_B:
    w5 -= 42
    r9 <<= w5
CH_DT4_CHUNK:
    r9 <<= 32
    r9 >>= 32
	w3 += w9
	r9 *= r9
	r2 += r9
	r7 = *(u64 *)(r1 + 98)	# data[5]
    if r7 == 0 goto CH_DT5_CHUNK
    r0 -= 1
    r5 = r7
    r5 >>= 52
    w5 -= 1023
    r7 &= r0
    r0 += 1
    r7 |= r0
    if r5 >= 42 goto CH_DT5_B
    w6 = 42
    w6 -= w5
    r7 >>= w6
    goto CH_DT5_CHUNK
CH_DT5_B:
    w5 -= 42
    r7 <<= w5
CH_DT5_CHUNK:
    r7 <<= 32
    r7 >>= 32
	w3 += w7
	r7 *= r7
	r2 += r7
	r8 = *(u64 *)(r1 + 106)	# data[6]
    if r8 == 0 goto CH_DT6_CHUNK
    r0 -= 1
    r5 = r8
    r5 >>= 52
    w5 -= 1023
    r8 &= r0
    r0 += 1
    r8 |= r0
    if r5 >= 42 goto CH_DT6_B
    w6 = 42
    w6 -= w5
    r8 >>= w6
    goto CH_DT6_CHUNK
CH_DT6_B:
    w5 -= 42
    r8 <<= w5
CH_DT6_CHUNK:
    r8 <<= 32
    r8 >>= 32
	w3 += w8
	r8 *= r8
	r2 += r8
	r9 = *(u64 *)(r1 + 114)	# data[7]
    if r9 == 0 goto CH_DT7_CHUNK
    r0 -= 1
    r5 = r9
    r5 >>= 52
    w5 -= 1023
    r9 &= r0
    r0 += 1
    r9 |= r0
    if r5 >= 42 goto CH_DT7_B
    w6 = 42
    w6 -= w5
    r9 >>= w6
    goto CH_DT7_CHUNK
CH_DT7_B:
    w5 -= 42
    r9 <<= w5
CH_DT7_CHUNK:
    r9 <<= 32
    r9 >>= 32
	w3 += w9
	r9 *= r9
	r2 += r9
	r3 >>= 3	# w3: mean
	r2 >>= 3
	w4 = w3
	r4 *= r4
	r2 -= r4	# r2: var

	r9 = *(u64 *)(r1 + 122)	# data[8]
    if r9 == 0 goto CH_DT8_CHUNK
    r0 -= 1
    r5 = r9
    r5 >>= 52
    w5 -= 1023
    r9 &= r0
    r0 += 1
    r9 |= r0
    if r5 >= 42 goto CH_DT8_B
    w6 = 42
    w6 -= w5
    r9 >>= w6
    goto CH_DT8_CHUNK
CH_DT8_B:
    w5 -= 42
    r9 <<= w5
CH_DT8_CHUNK:
    r9 <<= 32
    r9 >>= 32
	w8 = w3
	w8 -= w9
	if r3 > r9 goto ABS_DT8
	w9 -= w3
	w8 = w9
ABS_DT8:
	w4 = 0
	if r8 < 102 goto LD_DT9
	w4 = 1
LD_DT9:
	r9 = *(u64 *)(r1 + 130)	# data[9]
    if r9 == 0 goto CH_DT9_CHUNK
    r0 -= 1
    r5 = r9
    r5 >>= 52
    w5 -= 1023
    r9 &= r0
    r0 += 1
    r9 |= r0
    if r5 >= 42 goto CH_DT9_B
    w6 = 42
    w6 -= w5
    r9 >>= w6
    goto CH_DT9_CHUNK
CH_DT9_B:
    w5 -= 42
    r9 <<= w5
CH_DT9_CHUNK:
    r9 <<= 32
    r9 >>= 32
	w7 = w3
	w7 -= w9
	if r3 > r9 goto ABS_DT9
	w9 -= w3
	w7 = w9
ABS_DT9:
	if r7 < 102 goto LD_DT10
	w4 += 1
LD_DT10:
	r8 = *(u64 *)(r1 + 138)	# data[10]
    if r8 == 0 goto CH_DT10_CHUNK
    r0 -= 1
    r5 = r8
    r5 >>= 52
    w5 -= 1023
    r8 &= r0
    r0 += 1
    r8 |= r0
    if r5 >= 42 goto CH_DT10_B
    w6 = 42
    w6 -= w5
    r8 >>= w6
    goto CH_DT10_CHUNK
CH_DT10_B:
    w5 -= 42
    r8 <<= w5
CH_DT10_CHUNK:
    r8 <<= 32
    r8 >>= 32
	w9 = w3
	w9 -= w8
	if r3 > r8 goto ABS_DT10
	w8 -= w3
	w9 = w8
ABS_DT10:
	if r9 < 102 goto LD_DT11
	w4 +=1
LD_DT11:
	r7 = *(u64 *)(r1 + 146)	# data[11]
    if r7 == 0 goto CH_DT11_CHUNK
    r0 -= 1
    r5 = r7
    r5 >>= 52
    w5 -= 1023
    r7 &= r0
    r0 += 1
    r7 |= r0
    if r5 >= 42 goto CH_DT11_B
    w6 = 42
    w6 -= w5
    r7 >>= w6
    goto CH_DT11_CHUNK
CH_DT11_B:
    w5 -= 42
    r7 <<= w5
CH_DT11_CHUNK:
    r7 <<= 32
    r7 >>= 32
	w8 = w3
	w8 -= w7
	if r3 > r7 goto ABS_DT11
	w7 -= w3
	w8 = w7
ABS_DT11:
	if r8 < 102 goto LD_DT12
	w4 += 1
LD_DT12:
	r9 = *(u64 *)(r1 + 154)	# data[12]
    if r9 == 0 goto CH_DT12_CHUNK
    r0 -= 1
    r5 = r9
    r5 >>= 52
    w5 -= 1023
    r9 &= r0
    r0 += 1
    r9 |= r0
    if r5 >= 42 goto CH_DT12_B
    w6 = 42
    w6 -= w5
    r9 >>= w6
    goto CH_DT12_CHUNK
CH_DT12_B:
    w5 -= 42
    r9 <<= w5
CH_DT12_CHUNK:
    r9 <<= 32
    r9 >>= 32
	w7 = w3
	w7 -= w9
	if r3 > r9 goto ABS_DT12
	w9 -= w3
	w7 = w9
ABS_DT12:
	if r7 < 102 goto LD_DT13
	w4 += 1
LD_DT13:
	r8 = *(u64 *)(r1 + 162)	# data[13]
    if r8 == 0 goto CH_DT13_CHUNK
    r0 -= 1
    r5 = r8
    r5 >>= 52
    w5 -= 1023
    r8 &= r0
    r0 += 1
    r8 |= r0
    if r5 >= 42 goto CH_DT13_B
    w6 = 42
    w6 -= w5
    r8 >>= w6
    goto CH_DT13_CHUNK
CH_DT13_B:
    w5 -= 42
    r8 <<= w5
CH_DT13_CHUNK:
    r8 <<= 32
    r8 >>= 32
	w9 = w3
	w9 -= w8
	if r3 > r8 goto ABS_DT13
	w8 -= w3
	w9 = w8
ABS_DT13:
	if r9 < 102 goto LD_DT14
	w4 +=1
LD_DT14:
	r7 = *(u64 *)(r1 + 170)	# data[14]
    if r7 == 0 goto CH_DT14_CHUNK
    r0 -= 1
    r5 = r7
    r5 >>= 52
    w5 -= 1023
    r7 &= r0
    r0 += 1
    r7 |= r0
    if r5 >= 42 goto CH_DT14_B
    w6 = 42
    w6 -= w5
    r7 >>= w6
    goto CH_DT14_CHUNK
CH_DT14_B:
    w5 -= 42
    r7 <<= w5
CH_DT14_CHUNK:
    r7 <<= 32
    r7 >>= 32
	w9 = w3
	w9 -= w7
	if r3 > r7 goto ABS_DT14
	w7 -= w3
	w9 = w7
ABS_DT14:
	if r9 < 102 goto LD_DT15
	w4 += 1
LD_DT15:
	r8 = *(u64 *)(r1 + 178)	# data[15]
    if r8 == 0 goto CH_DT15_CHUNK
    r0 -= 1
    r5 = r8
    r5 >>= 52
    w5 -= 1023
    r8 &= r0
    r0 += 1
    r8 |= r0
    if r5 >= 42 goto CH_DT15_B
    w6 = 42
    w6 -= w5
    r8 >>= w6
    goto CH_DT15_CHUNK
CH_DT15_B:
    w5 -= 42
    r8 <<= w5
CH_DT15_CHUNK:
    r8 <<= 32
    r8 >>= 32
	w7 = w3
	w7 -= w8
	if r3 > r8 goto ABS_DT15
	w8 -= w3
	w7 = w8
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

