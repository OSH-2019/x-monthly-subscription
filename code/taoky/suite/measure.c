// modified from fjw's code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "fatal_posix.h"
#include "prepare.h"
#include "config.h"

__inline__ uint64_t time_cnt(void) {
    uint32_t lo, hi;
    __asm__ __volatile__ (
        "rdtscp" : "=a"(lo), "=d"(hi)
    );
    return ((uint64_t)lo) | (((uint64_t)hi) << 32);
}

int main(int argc, char** argv) {	
    int times = 10;
    int correct_data = true;
    if (getuid() || geteuid()) {
        fprintf(stderr, "Need root to proceed\n");
        exit(-1);
    }
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s [RECV ITH] [SEND ITH] [TEST TIMES] [CORRECT? (1/0)]\n", argv[0]);
        exit(-1);
    }

    char send_iface[16] = SOURCE_IFACE;
    char recv_iface[16] = TARGET_IFACE;
    if (argc >= 3) {
        strcpy(recv_iface, argv[1]);
        strcpy(send_iface, argv[2]);
        if (argc >= 4) times = atoi(argv[3]);
        if (argc >= 5 && strcmp("0", argv[4]) == 0)
            correct_data = 0;
    }
    int sock_send = sender_prepare(send_iface);
    int sock_recv = receiver_prepare(recv_iface);

    struct sockaddr_in target_addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = 0xFFFFFFFF,
        .sin_port = htons(UDP_PORT)
    };

    size_t bufsize = 8192, cbufsize = 1024;
    byte *recv_buf = malloc(bufsize), *cbuf = malloc(cbufsize);
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
    int received;
    for (int t = 0; t < times; t++) {
        byte send_buf[272];
        // Prepare data
        {
            if (correct_data) {
                unsigned long magic = MAGIC;
                memcpy(send_buf + 0, &magic, sizeof magic);
                unsigned long zero = 0UL;
                memcpy(send_buf + 8, &zero, sizeof zero);
                double data;
                for (int i = 0; i < 32; i++) {
                    scanf(" %lf", &data);
                    memcpy(send_buf + 16 + 8 * i, &data, sizeof data);
                }
            } else {
                int fd = open("/dev/urandom", O_RDONLY);
                read(fd, send_buf, 272);
                send_buf[0] = send_buf[7] = '!';
                close(fd);
            }
        }

        uint64_t t1 = time_cnt();
        Sendto(sock_send, send_buf, sizeof(send_buf), 0, (struct sockaddr*)&target_addr, sizeof(struct sockaddr));
        for (;;) {
            received = Recvmsg(sock_recv, &msg, 0);
            if (received == 0 || recv_buf[28] != '!' || recv_buf[35] != '!') { // Bad magic, not constructed by this program
                ;
            } else {
                uint64_t t2 = time_cnt();
                printf("%ld %ld %ld\n", t1, t2, t2 - t1);
                break;
            }
        }
    }
    return 0;
}
