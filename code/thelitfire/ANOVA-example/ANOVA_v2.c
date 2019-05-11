#include <stdio.h>

int main(void) {
	freopen("data_arranged_000001SZ.txt", "r", stdin);
	freopen("data_segmented_000001SZ.txt", "w", stdout);
	while (1) {
		//�����ڴ˴��յ�һ�������������ʼִ������ѭ���ڵĲ���
		//�����������1+1+32��64λ��С������ data[34]
		//data[0]����һ�� magic number��Ϊ�����趨��һ����ʶ����������������Ҫ�����ݰ����÷������Ա�������ʽ�����
		//data[1]����һ����ǩλ������������Ǻ�����һ����ӹ��ܵ�һ������flag
		//data[2]��data[34]������Ҫ����Ĵ������ݣ�data[2]��data[17]��һ�����ڣ�data[18]��data[33]�ǰ��ŵ������ڵ���һ������
		//��������16����ÿ�δ����ƶ���ֻ����1�����ݣ������������õ�һ�����ں͵�ʮ�߸��������Ƚϣ��㷨����ʦ���ģ�

		//�����ǽӰ�������ת�������������� ��������
		//������ʾ������ٶ�һ����һ�����ݰ�����17�У���Ӧ17�����ݣ�magic number �ݲ������ǣ�
		//�����ǵ����ݰ��У�ÿһ��������64λdouble��ʾ�ĸ�����������������ԭʼ���ݲ�����65535��С�����λ��������3
		//�ʣ�С����ǰ������65535�Ĳ�����16λ��ʾ��2^15 - 1 = 65535����С�������10λ��ʾ��2^-10 = 0.0009765625������26λ
		//����26λ��ʾ��unsigled long long��ͬʱĬ����ʽ����һ�� ����2^-10 �ġ�Ȩ��
		double data[32];
		for (int i = 0; i < 32; ++i) if (scanf("%lf", data + i) != 1) return 0;
		unsigned long long x[32];
		//ѭ���ֶ�չ�����ˣ����Ҷ�ջ�ռ�ķ��ʲ����漰������ offset��
		for (int i = 0; i < 32; ++i) x[i] = (unsigned long long)(data[i] * 1024);

		//�����������һ�����ڵľ�ֵ�ͷ���
		//��ֵ����漰�� 26 + 4 = 30λ����������漰�� 26*2 + 4 = 56 λ�����漰�߾���
		unsigned long long mean = 0, var = 0;
		for (int i = 0; i < 16; ++i) {
			mean += x[i];
			var += x[i] * x[i];
		}
		mean >>= 4;var >>= 4;
		var -= mean * mean;
		//�������жϵڶ��������ڵ�ÿ�����ݣ��Ƿ��� P(|x-u|>=e) <= o^2/e^2 ������e������ȡ������������Ϊ0.1
		unsigned long long tot = 0;
		const unsigned long long e = 0x66;//0.1
		unsigned long long abs;
		for (int i = 16; i < 32; ++i) {
			if (mean < x[i]) abs = x[i] - mean; else abs = mean - x[i];
			if (abs >= e) ++tot;
		}
		// tot/16 <= var/e^2
		if (tot*e*e > 16ull*var) {
			//Ҫ���֣��򷵻�һ������״̬�����߸������ǩ�����߸��û��ռ䷢��Ϣ������������������ļ������һ������
			for (int i = 0; i < 15; ++i) printf("%.3lf ", data[i]);
			printf("%.3lf|", data[15]);
			for (int i = 16; i < 31; ++i) printf("%.3lf ", data[i]);
			printf("%.3lf\n", data[31]);
		}
		else {
			//�����֣��򲻷��ػ���״̬�����ﲻ������
			for (int i = 0; i < 15; ++i) printf("%.3lf ", data[i]);
			printf("%.3lf ", data[15]);
			for (int i = 16; i < 31; ++i) printf("%.3lf ", data[i]);
			printf("%.3lf\n", data[31]);
		}
	}
}
