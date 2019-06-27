// modified from fjw's code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "fatal_posix.h"

typedef unsigned char byte;

#define SOURCE_IFACE "enp1s0np1"
#define TARGET_IFACE "enp1s0np0"
#define UDP_PORT 6666
#define MAGIC 0x216C7174786A7A21UL

__inline__ uint64_t time_cnt(void) {
    uint32_t lo, hi;
    __asm__ __volatile__ (
        "rdtscp" : "=a"(lo), "=d"(hi)
    );
    return ((uint64_t)lo) | (((uint64_t)hi) << 32);
}

int main(int argc, char** argv) {
    if (getuid() || geteuid()) {
        fprintf(stderr, "Need root to proceed\n");
        exit(-1);
    }
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s [RECV ITH] [SEND ITH]\n", argv[0]);
        exit(-1);
    }
    int sock_send = Socket(AF_INET, SOCK_DGRAM, 0);
    int sock_recv = Socket(AF_PACKET, SOCK_DGRAM, 0);

    char send_iface[16] = TARGET_IFACE;
    char recv_iface[16] = SOURCE_IFACE;
    if (argc >= 3) {
        strcpy(recv_iface, argv[1]);
        strcpy(send_iface, argv[2]);
    }

    unsigned ifacenum = if_nametoindex(recv_iface), opt = 1;
    int optval = -1;

    Setsockopt(sock_send, SOL_SOCKET, SO_BINDTODEVICE, send_iface, 1 + strlen(send_iface));
    Setsockopt(sock_send, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &optval, sizeof(int));
    Setsockopt(sock_recv, SOL_SOCKET, SO_BINDTODEVICE, recv_iface, 1 + strlen(recv_iface));

    struct sockaddr_in target_addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = 0xFFFFFFFF,
        .sin_port = htons(UDP_PORT)
    };
    struct sockaddr_ll sll = {
        .sll_family = AF_PACKET,
        .sll_ifindex = ifacenum,
        .sll_protocol = htons(3)
    };
    Bind(sock_recv, (struct sockaddr*)&sll, sizeof sll);

    size_t bufsize = 8192, cbufsize = 1024;
    byte *recv_buf = malloc(bufsize), *cbuf = malloc(cbufsize);
    unsigned long magic = MAGIC;
    struct iovec iov = {
        .iov_base = recv_buf,
        .iov_len = bufsize
    };
    struct msghdr msg = {
        .msg_name = NULL,
        .msg_namelen = (socklen_t)0,
        .msg_iov = &iov,
        .msg_iovlen = 1,
        .msg_control = cbuf,
        .msg_controllen = (socklen_t)cbufsize,
        .msg_flags = 0
    };
    int saddr_size = sizeof(struct sockaddr), received;
    struct cmsghdr *cmsg;

    byte send_buf[272];
    // Prepare data
    {
        unsigned long magic = MAGIC;
        memcpy(send_buf + 0, &magic, sizeof magic);
        unsigned long zero = 0UL;
        memcpy(send_buf + 8, &zero, sizeof zero);
        double data;
        for (int i = 0; i < 32; i++) {
            scanf(" %lf", &data);
            memcpy(send_buf + 16 + 8 * i, &data, sizeof data);
        }
    }

    Sendto(sock_send, send_buf, sizeof(send_buf), 0, (struct sockaddr*)&target_addr, sizeof(struct sockaddr));
    uint64_t t1 = time_cnt();
    // ...
    received = Recvmsg(sock_recv, &msg, 0);
    // if (received == 0)
    //     break;
    if (recv_buf[28] != '!' || recv_buf[35] != '!') { // Bad magic
        ;
    } else {
        uint64_t t2 = time_cnt();
        printf("%ld %ld\n", t1, t2);
    }

    return 0;
}