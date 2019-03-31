# 调研报告

**Monthly Subscription Group**

## 小组成员

- **赵家兴 (PB17111625) (组长)**
- 陶柯宇 (PB17111630)
- 付佳伟 (PB17111643)
- 陈昂 (PB17061250)
- 李喆昊 (PB17050941)

## 项目简介

我们的项目是：对冯·诺伊曼架构的计算机在网络包处理上进行非冯化改造，绕过 CPU 与操作系统内核，直接在网卡上对网络包进行数据流驱动的实时处理。目标是纳秒级的延迟。

## 项目背景

### 一般的数据包处理模式

在网卡接收到数据包后，网卡会通过 DMA 把数据包复制到内核内存中（如果没有 DMA，那么复制就要由 CPU 来做）。之后产生硬件中断，通知 CPU 数据复制完成，此时 CPU 运行网卡驱动程序的对应函数，清空这个中断，并且启动 NAPI (New API) 的函数。在此之后，网卡驱动与内核进行了一系列复杂的处理……

### cBPF 与 eBPF

我们如何对数据包进行一些处理，比如说过滤掉一些满足指定规则的包呢？在用户层写程序非常方便，但是用户层的网络处理程序很可能带来从内核层到用户层的复制等操作，降低效率。BPF (Berkeley Packet Filter) 的思想是：不需要处理的网络包就尽可能早地丢弃掉。为了实现这个目的，BPF 设计了一套虚拟机，允许用户在内核层执行 BPF 的字节码，减少不必要的从内核层到用户层的网络包复制。

#### cBPF

BPF 于 1992 年末由 Steve McCane 和 Van Jacobson 在论文 *The BSD Packet Filter: A New Architecture for User-level Packet Capture* 中提出。在 14 年新的 BPF 架构被提出之后，之前的 BPF 被称为 cBPF ("classic BPF")。cBPF 的虚拟机非常精简，只有用于索引的寄存器（Index register）与累加器两个寄存器，指令也只有二十余条。

我们可以使用 Linux 上的 `tcpdump` 查看一些 cBPF 指令的例子。

```
$ sudo tcpdump -d ip
(000) ldh      [12]
(001) jeq      #0x800           jt 2	jf 3
(002) ret      #262144
(003) ret      #0
$ sudo tcpdump -d tcp
(000) ldh      [12]
(001) jeq      #0x86dd          jt 2	jf 7
(002) ldb      [20]
(003) jeq      #0x6             jt 10	jf 4
(004) jeq      #0x2c            jt 5	jf 11
(005) ldb      [54]
(006) jeq      #0x6             jt 10	jf 11
(007) jeq      #0x800           jt 8	jf 11
(008) ldb      [23]
(009) jeq      #0x6             jt 10	jf 11
(010) ret      #262144
(011) ret      #0
```

`-d` 参数会输出“人类可读”的 BPF 代码。上面两个例子分别过滤了 IP 包和 TCP 包。

2011 年，Eric Dumazet 将 BPF 转译器添加了 JIT，使得内核可以将 BPF 程序直接翻译为支持的目标架构的指令，如 x86，ARM，MIPS 等，进一步提高了处理速度。

#### eBPF

2014 年，Alexei Starovoitov 扩充了 BPF，实质上创建了一个新的架构，称为 eBPF (extended BPF)。eBPF 添加了以下这些新的改进：

- 与 x86-64 类似的架构：eBPF 使用 64 位寄存器，并将可用寄存器的数量从 2（累加器和 X 寄存器）增加到 10。 eBPF 还扩展了操作码的数量。
- 与网络子系统分离：cBPF 受限于基于数据包的数据模型。由于它用于包过滤，因此其代码存在于网络子系统中。但是，eBPF VM 不再受限于此。现在可以将 eBPF 程序附加到跟踪点或 kprobe 中。这为 eBPF 打开了插桩（Instrumentation），性能分析以及其他内核子系统中的更多用途的大门。
- 全局数据存储结构 Map：Map 是一种通用数据结构，以键值对的形式存储不同类型的数据。它们允许在 eBPF 内核程序之间以及内核和用户空间应用程序之间共享数据。
- 辅助函数（Helper functions）：如数据包重写，校验和计算或数据包克隆。与用户空间编程不同，这些函数在内核中执行。此外，还可以从eBPF程序执行系统调用。
- 尾调用（Tail-calls）：eBPF 程序大小限制为 4096 字节。尾调用功能允许 eBPF 程序通过控制新的 eBPF 程序来克服此限制。

由于 eBPF 的指令集变得更加复杂，单纯使用汇编的方式进行开发是比较难的。目前，eBPF 程序可以使用 C 语言书写，使用 `clang` 和 `llvm` 编译之后就可以得到 eBPF 代码。最终，内核中的 JIT 编译器将 eBPF 代码转为原生代码。

![eBPF C-based Programming](files/research/eBPF-C-based-programing.png)

*图：C 程序转为内核态原生指令的过程*

需要注意的是，eBPF 程序最终会注入内核，那么如何保证注入代码的安全性？eBPF 使用 verifier 来检测代码。

第一轮检查中，verifier 检测代码中是否出现以下现象：

- 循环（即代码是否构成有向无环图）。
- 跳出 eBPF 代码范围的 `JMP`。
- 无法到达的指令。
- 过长（超过了 4096 字节）

再之后第二轮检查中，verifier 细致地检查每一个分支，发现任何错误则失败。在第二轮检查中对分支数和指令数都有要求。

eBPF 的整体架构如下。

![eBPF Overview of Runtime Architecture](files/research/eBPF-overview-of-runtime-arch.PNG)

*图：eBPF 整体的运行时架构*

### XDP 与 DPDK

目前在 Linux 上的高性能网络处理实践中，主要有两种方案：XDP 和 DPDK。

#### DPDK

首先介绍 DPDK，它实现了 kernel bypass，在用户空间处理所有的网络请求。由于绕过了内核，网卡也需要由用户空间的驱动来管理。

将网卡的完全控制权交给用户空间程序的好处是，我们减少了内核所带来的障碍，比如上下文切换、网络层处理、中断等。这足以使网络数据传输达到 10Gbps 甚至更高。kernel bypass 与其他特性（如批量包处理）和性能调整方法（如 NUMA 感知，CPU 隔离等），整合出了高性能用户空间网络的基础结构。

但 DPDK 基于的用户空间网络的不足则在于：

- 用户空间程序绕过了操作系统内核，就意味着跳过了操作系统提供的硬件资源抽象而直接管理硬件本身。用户空间驱动程序虽然可以保证正常运作，但一般地相比内核更缺乏测试与兼容性。
- 用户空间程序同时跳过了内核提供的网络管理函数，这意味着用户空间程序要“重复发明轮子”，重新实现那些已经被内核提供过的功能。
- 程序在沙箱中运行，而这限制了它与操作系统其他部分的集成与交互。
- 内核可以为网络提供充分的安全层，而这在用户空间程序中并不存在。

#### XDP

与用户空间网络截然相反的是，XDP（eXpress Data Path）将诸如过滤器、映射器、路由等全部用户空间网络程序转移到了内核的领域里。XDP 允许我们的网络程序在网络包到达网卡而进入内核网络层之前立即执行，这显著提高了网络包处理速度。

![XDP Packet Processing](files/research/xdp-packet-processing.png)

*图：XDP 网络包处理整体情况*

XDP 基于上文提到的 BPF，实现高速的包处理。

## 立项依据

### 智能网卡  SmartNIC

#### 产生

现代的工作负载和数据中心设计给 CPU 核心带来了太多的网络开销。随着更快的网络速度（每个链接可高达 200 Gb/s）， CPU 花费了太多开销对网络流量进行分类、跟踪和控制。这些昂贵的 CPU 本是为通用应用程序处理而设计的，而不是消耗所有这些处理能力仅用来查看和管理数据的移动。

智能网卡（SmartNIC）应运而生，它可以将本应由 CPU 处理的工作转移到网卡上执行，减少 CPU 工作量的同时提高性能，其核心是通过 FPGA 协助 CPU 处理网络负载，具有以下特征：

- 通过 FPGA 本地化编程支持数据面（Data plane）和控制面（Control plane）功能定制，协助 CPU 处理网络负载。
- 通常包含多个端口和内部交换机，快速转发数据并基于网络数据包、应用程序套接字等智能映射到到相关应用程序。
- 检测和管理网络流量。

过去 30 年来，网卡已经从具有单一 MAC，PHY（端口物理层）和系统接口的简单网卡发展到具有多个网络接口和用于 TCP/IP 的硬件卸载（Hardware offload）引擎的高性能适配器、虚拟化等功能。最新的 NIC 基于支持高速网络接口的 I/O 控制器。SmartNIC 将 FPGA、处理器或基于处理器的智能 I/O 控制器与分组处理和虚拟化加速集成在一起。大多数 SmartNIC 可以使用标准的开发工具进行编程，越来越多的厂商也开始增加了对 eBPF 以及可编程语言 P4 的支持。

目前业界提供基于 FPGA 的 SmartNIC 的厂商包括 Accolade、BittWare、Enyx 等。这些适配器集成了来自 Intel 或 Xilinx 的 FPGA。而 Broadcom、Cavium、Intel、Netronome 等均可提供基于处理器的 SmartNIC。同时，亚马逊和谷歌已经开发了自己的 SmartNIC ASIC。

#### 应用场景

- 安全隔离

出于安全性考虑，有时需要将网络与 CPU 相隔离，通常黑客攻击和恶意软件来源于网络。使用智能网卡便可以在网卡上检查网络流量、阻止攻击和进行加密传输，从而带来了安全性上的提升。如果主 CPU 受到威胁，那么智能网卡仍然可以检测恶意活动，在不立即占用 CPU 资源的情况下阻止攻击。

- 存储虚拟化和云

智能网卡的一个较新的用例是虚拟化软件定义的存储、超融合基础架构和其他云资源。在超融合架构数据中心中，SmartNIC 为虚拟化应用程序提供硬件加速与网络接口紧密结合，并可分布在大型服务器网络中，减小 CPU 负载，提供额外的边缘计算能力，加速特定应用和虚拟化功能，并且通过正确的语言和工具链支持，为用户提供应用加速即服务的附加价值。智能网卡甚至可以虚拟化 GPU（或一些神经网络处理器），以便任何服务通过网络访问。

![SmartNIC using cases](files/research/Capture-16.jpg)

*图：SmartNIC 功能与对应硬软件情况*

一个好的智能网卡必须是可编程的。虽然为了获得最佳性能，大多数加速功能必须在硬件中运行，但为了获得最大的灵活性，这些功能的控制和编程需要在软件中运行。

有许多功能可以在智能网卡上编程。通常特定的卸载方法，加密算法和传输机制不会发生太大变化，但路由规则、加密密钥和网络地址始终会发生变化。我们将前者的功能视为数据面，后者则用作控制面功能。一旦建立标准化，数据面规则和算法就可以编写到硬件上了。然而控制面规则和编程变化多样，无法硬编码到芯片中，但可以在 FPGA 上运行或在 C 可编程的 Linux 环境中运行。

### 数据流计算机架构

#### 数据流架构与传统冯诺伊曼架构的区别

##### 回顾冯诺依曼架构

![Von Neumann Architecture](files/research/1280px-Von_Neumann_Architecture.svg.png)

*图：冯·诺伊曼架构示意图*

**主要特点：**

- 基于控制流概念。
- 指令与数据一起放在内存中。
- 程序的执行需要依靠程序计数器（PC），通过移动 PC 将对应地址的指令取入 IR (Instruction Register) 并执行。

**缺陷：**

- 天生存在 CPU 与内存之间信息交换的瓶颈，在处理并行问题方面有本质困难。

  ![Speed Benchmark](files/research/v2-8b403bda4f4654ec09f082a3e33f1b13_hd.jpg)

*图：内存数据读写与 CPU 读写数据速度差距巨大*

- 近年来基于冯·诺依曼架构的新式计算机体系虽然采用并行技术，但没有摆脱传统的以控制流为主的设计思想，并行处理能力受限。

#### 数据流结构



## 重要性与前瞻性分析

数据中心 etc.

## 相关工作

Hardware offload……

## 参考文献

1. [Packet processing](https://en.wikipedia.org/wiki/Packet_processing)
2. [Monitoring and Tuning the Linux Networking Stack: Receiving Data](https://blog.packagecloud.io/eng/2016/06/22/monitoring-tuning-linux-networking-stack-receiving-data/)
3. [The BSD Packet Filter: A New Architecture for User-level Packet Capture](http://www.tcpdump.org/papers/bpf-usenix93.pdf)
4. [eBPF 简史](https://www.ibm.com/developerworks/cn/linux/l-lo-eBPF-history/index.html)
