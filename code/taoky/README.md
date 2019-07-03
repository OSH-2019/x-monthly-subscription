# `suite/` 代码设计与使用说明

代码包含了以下部分：

- 测时（`measure`）
- 发送与接收（`sender` & `receiver`）

这两部分都使用了 fjw 的发送与接收数据包的设计。

## 发送与接收的设计

在我们的测试服务器上，目标网卡暴露了两个 interface：`ens160np0` 和 `ens160np1`。

```
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
2: ens192: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq state UP mode DEFAULT group default qlen 1000
    link/ether 00:50:56:af:4a:dc brd ff:ff:ff:ff:ff:ff
3: ens160np0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq state UP mode DEFAULT group default qlen 1000
    link/ether 00:15:4d:13:5d:26 brd ff:ff:ff:ff:ff:ff
4: ens160np1: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq state UP mode DEFAULT group default qlen 1000
    link/ether 00:15:4d:13:5d:27 brd ff:ff:ff:ff:ff:ff
```

我们的目标 BPF 程序挂载在 `ens160np1` 上。据 fjw 称，这两个 interface 物理上直接用一根网线相连。

由于给这两个 interface 配置 IP 等比较麻烦（尝试配过，但是数据包总是会经过本地回环），最后 fjw 采用了比较省事的办法：从 `ens160np0` 发送广播包，`ens160np1` 接收广播包，给 BPF 程序做处理。

包是 UDP 包。内容的结构如下：

```c
struct packet {
    unsigned long magic;
    unsigned long tag;
    union {
        unsigned int data[16];  // 整数化后数据
        double data_raw[16];  // 原始数据
    };
};
```

## 测时

使用 `rdtscp` 指令。不直接使用 `rdtsc` 的原因是考虑到 CPU 的乱序执行可能会让测试结果失去意义，而 `rdtscp` 可以保证同步。

之前考虑过使用厂家在 email 里推荐的 [PROX](https://github.com/nvf-crucio/PROX)，我自己在虚拟机里尝试编译的时候甚至发现这个项目的一个[编译错误的问题](https://github.com/nvf-crucio/PROX/commit/305f09d6d9488d8bc3ab40a3955ad878c86780c6#r33680319)。不过在测试之后我放弃了这个想法，因为这个项目依赖于 DPDK，而 DPDK：

- 需要对网络栈做处理，这可能会导致我们的 SSH 连接中断。
- 使用网卡时需要特定的驱动，可能会导致我们的 BPF 程序无法挂载。

所以最后使用了比较简单的方式：

1. 准备发送的数据。
2. 执行 `rdtscp`。
3. 发送包（`sendto()`）
4. 接收包（`recvmsg()`）、判断是否是本程序发送的（判断 flag）
5. 如果是，再执行 `rdtscp`，求差。
6. 2 至 5 步执行 1000 次，然后再执行 n 次，直到最小值恒定。

## 发送与接收程序

`sender` 和 `receiver` 对 fjw 的设计做了简单的包装。`sender` 读入一些浮点数，乘以 1024 后整数化打包发送。`receiver` 判断 magic 正确后接收，判断 `tag` 与预期值是否一致。

## 使用说明

进入目录。使用 `make measure` 编译测时程序，使用 `make sender` 和 `make receiver` 分别编译发包和收包程序。所有编译得到的程序都需要 root 权限。

输入 `./xxx --help` 可以获取帮助。

- `measure` 参数分别为 `[接收数据包的 interface] [发送数据包的 interface] [n] [是否发送正确的数据包（1 为正确，0 为错误，错误时从 /dev/urandom 读取随机字符串）] [是否整数化（0 是，1 否）]`
- `sender` 参数为 `[发送数据包的 interface] [数据文件名] [是否整数化（0 是，1 否）]`
- `receiver` 参数为 `[接收数据包的 interface] [信息详细程度（0: 只显示 tag 是否与预期一致，1: 检查 magic 是否完全正确，输出接收到的数据内容，2: 额外显示接收的包的 hexdump）] [是否整数化（0 是，1 否）]`

