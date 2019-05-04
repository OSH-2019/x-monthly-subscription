#include <stdio.h>

int main(void) {
	freopen("data_arranged_000001SZ.txt", "r", stdin);
	freopen("data_segmented_000001SZ.txt", "w", stdout);
	while (1) {
		//我们在此处收到一个网络包，并开始执行以下循环内的部分
		//假设网络包有1+1+32个64位大小的数据 data[34]
		//data[0]包含一个 magic number，为我们设定的一个标识，标明这是我们需要的数据包（该方法可以被其他方式替代）
		//data[1]包含一个标签位，这可以是我们后续进一步添加功能的一个所需flag
		//data[2]到data[34]是我们要处理的窗口数据；data[2]到data[17]是一个窗口，data[18]到data[33]是挨着但不相邻的另一个窗口
		//窗口数据16个，每次窗口移动都只滑动1个数据；但是我们是拿第一个窗口和第十七个窗口作比较（算法是老师给的）

		//以下是接包、浮点转整数、存入数组 三个步骤
		//本人演示程序里，假定一行是一个数据包，共17列，对应17个数据（magic number 暂不做考虑）
		//在我们的数据包中，每一个数据是64位double表示的浮点数，均是正数，原始数据不超过65535，小数点后位数不超过3
		//故，小数点前不超过65535的部分用16位表示（2^15 - 1 = 65535），小数点后用10位表示（2^-10 = 0.0009765625），共26位
		//将这26位表示成unsigled long long，同时默认隐式存在一个 乘以2^-10 的“权”
		double data[32];
		for (int i = 0; i < 32; ++i) if (scanf("%lf", data + i) != 1) return 0;
		unsigned long long x[32];
		//循环手动展开好了，并且对栈空间的访问不能涉及“变量 offset”
		for (int i = 0; i < 32; ++i) x[i] = (unsigned long long)(data[i] * 1024);

		//接下来计算第一个窗口的均值和方差
		//均值最多涉及到 26 + 4 = 30位，方差最多涉及到 26*2 + 4 = 56 位：不涉及高精度
		unsigned long long mean = 0, var = 0;
		for (int i = 0; i < 16; ++i) {
			mean += x[i];
			var += x[i] * x[i];
		}
		mean >>= 4;var >>= 4;
		var -= mean * mean;
		//接下来判断第二个窗口内的每个数据，是否有 P(|x-u|>=e) <= o^2/e^2 成立，e是我们取定的数，暂设为0.1
		unsigned long long tot = 0;
		const unsigned long long e = 0x66;//0.1
		unsigned long long abs;
		for (int i = 16; i < 32; ++i) {
			if (mean < x[i]) abs = x[i] - mean; else abs = mean - x[i];
			if (abs >= e) ++tot;
		}
		// tot/16 <= var/e^2
		if (tot*e*e > 16ull*var) {
			//要划分，则返回一个划分状态，或者给包打标签，或者给用户空间发信息，待定。这里在输出文件那里加一条竖线
			for (int i = 0; i < 15; ++i) printf("%.3lf ", data[i]);
			printf("%.3lf|", data[15]);
			for (int i = 16; i < 31; ++i) printf("%.3lf ", data[i]);
			printf("%.3lf\n", data[31]);
		}
		else {
			//不划分，则不返回划分状态，这里不打竖线
			for (int i = 0; i < 15; ++i) printf("%.3lf ", data[i]);
			printf("%.3lf ", data[15]);
			for (int i = 16; i < 31; ++i) printf("%.3lf ", data[i]);
			printf("%.3lf\n", data[31]);
		}
	}
}
