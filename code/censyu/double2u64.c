// double2u64.c
// - CA
//
// __u64 & double - 64bits
// double
// 63    | 62            52 | 51     0 |
// sign  | 11 bits exponent | mantissa |
// ull x
// 63               25           10         0
// 0 ......        2^15         2^0       2^-10
// 
// TEST
// printf("%llx\n", *((__u64*)&data));
// printf("%lf\n", *((double*)&exponent));

typedef unsigned long long __u64;

__u64* double2u64(__u64 data[32])
{
    
    int i;
    __u64 exponent;
    __u64 x[32];

    for (i = 0; i < 32; i++)
    {
        if(data == 0)
            continue;
        exponent = (data >> 52) -1023;
        x = data & 0x000FFFFFFFFFFFFFull | 0x0010000000000000ull;
        if(42ull - exponent > 0ull)
            x >>= 42ull - exponent;
        else
            x <<= exponent - 42ull;
    }

    return x;
}