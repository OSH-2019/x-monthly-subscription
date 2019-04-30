#include<cstdio>
#include<cstdlib>
#include<climits>
#include<ctime>
#include<cmath>
//variable//
unsigned long long window_end_time[2];//[0]为一个窗口的结束数据时刻，[1]为它后一个窗口的结束数据时刻
char str[4096];//单行整体读取，只取第二、三、四、五列的数据
unsigned long long t;//单组数据的时间戳
double a[3],x[512],MEANmean[2],VARmean[2],MEANvar[2],VARvar[2];//单组数据的三轴加速度，三轴加速度平方和，上一个窗口的均值的均值，均值的方差，方差的均值，方差的方差

//function prototype//

//solve//
int main(void){
    srand(time(NULL));
    freopen("raw.txt","r",stdin);
    /*
    在 raw.txt 文件中：
    第一列为数据行为标识，如“走路，裤兜”，“静止，桌面”等等；这不是我们所需要的数据，但可以做切割效果参考。
    第二列为时间戳，为精确到毫秒的一个相对时间，64位无符号整数。
    第三、四、五列分别为（手机的）三轴加速度。我们不必精确理解每个轴的取向变化，只需计算其加速度向量的模，即三轴平方和再开根号，再或者不开根号。
    后面几列的数据为轨迹数据，GPS数据等，也不是我们所关心的。
    由于此文件是多个不同宏观时间的数据的整合，所以从上到下看第二列的时间值虽是递增，但有较大的间断。

    本次我们以此数据集合做方差分析算法的编写：固定窗口内的数据数！！！而非固定窗口的时间跨度！！！！
    由于数据集大部分是10ms一个数据，我们取一个窗口512=2^9个数据，大概保持1个窗口5s的行为；抽样一次抽128=2^7个，抽样次数64=2^6次，窗口重叠75%（每次新增替换128个）
    同时请特别注意！！！：取这样的2的幂次的值，可以方便我们使用位运算快速进行判定、除法等等操作！
    */
    freopen("segmentation.txt","w",stdout);
    /*
    在segmentation文件中，每个数值为应当做划分的时刻。该时刻的取值对应着 raw.txt 中第二列的值
    */
    /*
    需要强调的是！！！：所有输入，scanf，fgets等都会被转换并表示成对数据包每一位的访问！！！！所有输出都会转换为其他行为！！！！
    这一部分涉及到fjw管理的数据传递进入网卡的方式与格式，可以先不涉及
    */

    //处理第一个窗口。它不必做更多判定，只计算均值的均值、均值的方差、方差的均值、方差的方差即可。
    unsigned int count=0u;
    while (fgets(str,4096,stdin)!=NULL){
        sscanf(str,"%*d %llu %lf %lf %lf",&t,a,a+1,a+2);
        /*
        浮点化整，对六十四位浮点double型，假设其符合IEE754浮点标准（参阅https://en.wikipedia.org/wiki/Double-precision_floating-point_format#C_and_C++）
        我们目标是以二进制格式来处置浮点数到整数的转化
        实际上，数据是否是浮点数也取决于数据本身的格式；浮点化整的精度要求也会随着数据本身格式的位数多少而不同。
        我们很可能不能用2^64大小的unsigned long long来满足精度需求而使用高精度数值（如两个unsigned long long拼接起来形成2^128）

        本例中，我们采用将一个double格式表示的64位二进制小数，假定它是正常数值（而非缩小的、表示无穷的、NaN等）,这意味着fraction区域隐藏了一个前导1
        数据集合里各个加速度浮点数绝对值最大值33.068770，最小值0.00003；遂控制我们转成整数后最高位对应2^5，最低位对应2^-20(大约对应六位小数)共26位二进制值（类似E-2定义B-20）
        加速度为26位二进制（B-20），加速度平方52位二进制（B-40），三个方向的加速度平方和X是(至多)54位二进制（B-40），
        为了求均值mean，128个样本平方和的求和61位（B-40），再除以128变回54位mean（B-40）
        为了求方差var=EX^2-(EX)^2，平方和X再平方108位二进制（B-80），X^2的128个求和115位（B-80），再除以128变回108位（B-80），得var为108位（B-80）
        同理，均值的均值MEANmean是54位二进制（B-40），均值的方差VARmean是108位（B-80），
        方差的均值108位（B-80），方差的方差216位（B-160）

        以上讨论的二进制位数很大，即使原始浮点数只被展成了16位二进制整数。
        组长认为在本次实验中比较好的做法是，领到这个锅的人使用高精度拼接的方式，每一位高精度值是64位ull表示，使用低32位，即为了表示216位要用8个这样的ull值拼起来。
        如有更好的实现，或者有充足的理由降低精度，或者本次任务过重希望降低精度的，请自行调整上述讨论中的位数。

        在这里组长为了描述算法的方便，保持了浮点运算过程（不然岂不是替你们把代码写好了）
        以下涉及到浮点数的加减乘，在eBPF程序中要使用“函数调用”形式。这个函数调用是用高精度转换好并实现好了的过程。
        除法只涉及到除64，128等2的整次幂，所以只要实现高精度移位。
        综上，要实现高精度加减乘，移位运算。
        */
        x[count++]=a[0]*a[0]+a[1]*a[1]+a[2]*a[2];//这里是26位*26位=52位，52位三个相加最多54位，直接用ull计算
        if (!(count>>9)) continue;//即 count<512
        window_end_time[0]=t;
        /*
        在512个数据中抽样64次，每次抽样128个
        此例使用无放回抽样，被分到这部分锅的人若觉得实现过于复杂任务过于繁重，可以采用有放回抽样，但这种简化很可能让我们的最终结题报告的时候被问的下不来台（
        */
        MEANmean[0]=0.,VARmean[0]=0.,MEANvar[0]=0.,VARvar[0]=0.;//MEANmean直接用ull；VARmean用128位高精度；MEANvar用128高精度；VARvar用256位高精度
        for (unsigned int i=0u;!(i>>6);++i){
            double mean=0.,var=0.;//mean用ull表示，var用128位高精度表示
            unsigned long long vis[8]={0ull};//由于512=64*8，用8个ull来表示这个“是否已抽过”的bool关系就可以
            for (unsigned int j=0u;!(j>>7);){
                unsigned int k=((unsigned int)rand())&((1u<<9)-1u);//这里的&((1<<9)-1)代表对512取模，它也可以用一次左移再一次右移实现。下面出现的这种写法都一样。
                if (vis[k>>6] & (1ull << ( k & ((1u<<6) - 1u) ) ) ) continue;
                vis[k>>6] |= 1ull << ( k & ((1u<<6) - 1u) );
                ++j;
                mean+=x[k];var+=x[k]*x[k];//mean是最多128个54位相加，直接用ull；var是54位乘54位=108位，用128位高精度
            }
            mean/=128.;//mean右移7位，直接用ull
            var/=128.;var-=mean*mean;//var用128位右移7位的运算；后一个用ull*ull返回128位高精度的操作
            MEANmean[0]+=mean;//直接用ull
            VARmean[0]+=mean*mean;//用64*64加给128位高精度
            MEANvar[0]+=var;//用128位高精度+128位高精度
            VARvar[0]+=var*var;//256位高精度+=128位高精度*128位高精度
        }
        MEANmean[0]/=64;MEANvar[0]/=64;//这里要转换成高精移位：分别是ull右移6位，和128位高精度右移6位
        VARmean[0]/=64;VARmean[0]-=MEANmean[0]*MEANmean[0];//这里要转换成128位高精右移6位，128位高精-=64*64位ull
        VARvar[0]/=64;VARvar[0]-=MEANvar[0]*MEANvar[0];//这里要转换成256位高精右移6位，256位高精度-=128位*128位高精度
        break;
    }
    //接下来不断处理新的窗口数据
    //unsigned int count 将变成新的数据覆盖位置的指针。
    //比如说对于第一个窗口之后的第二个窗口，前128个要被替换。这时count=512，它对512取模=0，这正好是我们要覆盖的数据位置
    unsigned int newcount=0u;
    while (fgets(str,4096,stdin)!=NULL){
        sscanf(str,"%*d %llu %lf %lf %lf",&t,a,a+1,a+2);
        //开始用新数据覆盖旧数据
        x[(count+newcount++)&((1u<<9)-1u)]=a[0]*a[0]+a[1]*a[1]+a[2]*a[2];//这里要转换成高精乘和高精加
        if (!(newcount>>7)) continue;//即 newcount<128
        window_end_time[1]=t;count+=newcount;count&=(1u<<9)-1u;
        //开始抽样
        MEANmean[1]=0.,VARmean[1]=0.,MEANvar[1]=0.,VARvar[1]=0.;
        unsigned tot_mean=0u,tot_var=0u;//用于记录|x-mean|>epsilon的个数，这涉及到高精度比较
        for (unsigned int i=0u;!(i>>6);++i){//64次抽样
            double mean=0.,var=0.;
            unsigned long long vis[8]={0ull};//由于512=64*8，用8个ull来表示这个“是否已抽过”的bool关系就可以
            for (unsigned int j=0u;!(j>>7);){
                unsigned int k=((unsigned int)rand())&((1u<<9)-1u);//这里的&((1<<9)-1)代表对512取模，它也可以用一次左移再一次右移实现。下面出现的这种写法都一样。
                if (vis[k>>6] & (1ull << ( k & ((1u<<6) - 1u) ) ) ) continue;
                vis[k>>6] |= 1ull << ( k & ((1u<<6) - 1u) );
                ++j;
                mean+=x[k];var+=x[k]*x[k];//这里要转换成高精乘和高精加
            }
            mean/=128.;var/=128.;var-=mean*mean;//这里要转换成高精移位，高精乘和高精减

            //判断关于mean的不等式|mean-MEANmean[0]|>4*Sqrt{VARmean[0]}是否成立。注意这里换成了4倍而非5倍是为了用移位替代乘法
            //不等式等价于 (mean-MEANmean[0])*(mean-MEANmean[0])/16 > VARmean[0] 是否成立。
            //不等式左边是64位ull*64位ull=128位高精度，再右移4位，再与不等式右边的128位高精度比较
            if ((mean-MEANmean[0])*(mean-MEANmean[0])>16*VARmean[0]) ++tot_mean;

            //同样，判断关于var的不等式|var-MEANvar[0]|>8*Sqrt{VARvar[0]}是否成立。这里换成了8倍而非10倍。
            //不等式等价于(var-MEANvar[0])*(var-MEANvar[0])/64 > VARvar[0]
            //不等式左边是128位高精度*128位高精度=256位高精度，再右移6位，再与不等式右边的256位高精度比较
            if ((var-MEANvar[0])*(var-MEANvar[0])>64*VARvar[0]) ++tot_var;
            
            MEANmean[1]+=mean;VARmean[1]+=mean*mean;//这里要转换成高精乘和高精加
            MEANvar[1]+=var;VARvar[1]+=var*var;//这里要转换成高精乘和高精加
        }
        MEANmean[1]/=64;MEANvar[1]/=64;//这里要转换成高精移位
        VARmean[1]/=64;VARmean[1]-=MEANmean[1]*MEANmean[1];//这里要转换成高精移位，高精乘和高精减
        VARvar[1]/=64;VARvar[1]-=MEANvar[1]*MEANvar[1];//这里要转换成高精移位，高精乘和高精减
        //判定是否做划分
        //若tot_mean/64 > 1/16 即 totmean > 4 则划分
        //若tot_var/64 > 1/64 即 tot_var > 1 则划分
        if (tot_mean>4u||tot_var>1u) printf("%llu\n",window_end_time[0]);
        //为下一个窗口做准备
        window_end_time[0]=window_end_time[1];newcount=0u;
        MEANmean[0]=MEANmean[1];MEANvar[0]=MEANvar[1];VARmean[0]=VARmean[1];VARvar[0]=VARvar[1];
    }
}