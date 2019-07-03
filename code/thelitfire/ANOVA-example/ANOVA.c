#include <stdio.h>
#include <stdbool.h>

int main(void) {
	freopen("data_arranged_000001SZ.txt", "r", stdin);
	freopen("data_segmented_000001SZ.txt", "w", stdout);
	while (1) {
		double data[16];
		for (int i = 0; i < 16; ++i) if (scanf("%lf", data + i) != 1) return 0;
		unsigned long long x[16];
		for (int i = 0; i < 16; ++i) x[i] = (unsigned long long)(data[i] * 32768);

		unsigned long long mean1 = 0, var1 = 0;
		double mean2 = 0., var2 = 0.;
		for (int i = 0; i < 8; ++i) {
			mean1 += x[i];
			var1 += x[i] * x[i];
			mean2 += data[i];
			var2 += data[i] * data[i];
		}
		mean1 >>= 3; var1 >>= 3;
		var1 -= mean1 * mean1;
		mean2 /= 8; var2 /= 8;
		var2 -= mean2 * mean2;
		unsigned long long tot1 = 0, tot2 = 0;
		const unsigned long long e1 = 0xccd;
		double e2 = 0.1;
		unsigned long long abs1;
		double abs2;
		for (int i = 8; i < 16; ++i) {
			if (mean1 < x[i]) abs1 = x[i] - mean1; else abs1 = mean1 - x[i];
			if (abs1 >= e1) ++tot1;
			if (mean2 < data[i]) abs2 = data[i] - mean2; else abs2 = mean2 - data[i];
			if (abs2 >= e2) ++tot2;
		}
		// tot/8 <= var/e^2
		bool flag1=false;
		if (tot1*e1*e1 > 8ull*var1) {
			flag1=true;
		}
		bool flag2=false;
		if (tot2*e2*e2 > 8*var2) {
			flag2=true;
		}
		printf("%d\t%d\t%d\n",flag1==flag2,flag1,flag2); 
	}
}
