// modified from fjw's code

#define _GNU_SOURCE
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
#include <sched.h>

#include "fatal_posix.h"
#include "prepare.h"
#include "config.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

__inline__ uint64_t time_cnt(void) {
    uint32_t lo, hi;
    __asm__ __volatile__ (
        "rdtscp" : "=a"(lo), "=d"(hi)
    );
    return ((uint64_t)lo) | (((uint64_t)hi) << 32);
}

__inline__ uint64_t measure(int sock_send, int sock_recv, byte *send_buf, struct sockaddr_in target_addr, struct msghdr *msg, byte *recv_buf) {
    int received;
    uint64_t t1 = time_cnt();
    uint64_t t2;
    Sendto(sock_send, send_buf, sizeof(send_buf), 0, (struct sockaddr*)&target_addr, sizeof(struct sockaddr));
    for (;;) {
        received = Recvmsg(sock_recv, msg, 0);
        if (received == 0 || recv_buf[28] != '!' || recv_buf[35] != '!') { // Bad magic, not constructed by this program
            ;
        } else {
            t2 = time_cnt();
            // printf("%ld %ld %ld\n", t1, t2, t2 - t1);
            break;
        }
    }
    return t2 - t1;
}

void set_cpu_one_core(void) {
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);  // always use the first cpu core
    if (sched_setaffinity(getpid(), sizeof(set), &set) == -1) {
        perror("sched_setaffinity()");
        exit(-1);
    }
}

int main(int argc, char** argv) {	
    int check_times = 10000000;
    int correct_data = true;
    int raw = false;
    require_root();
    set_cpu_one_core();
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s [RECV ITH] [SEND ITH] [TEST CHECK TIMES] [CORRECT? (1/0)] [RAW? (1/0)]\n", argv[0]);
        exit(-1);
    }

    char send_iface[16] = SOURCE_IFACE;
    char recv_iface[16] = TARGET_IFACE;
    if (argc >= 3) {
        strcpy(recv_iface, argv[1]);
        strcpy(send_iface, argv[2]);
        if (argc >= 4) check_times = atoi(argv[3]);
        if (argc >= 5 && strcmp("0", argv[4]) == 0)
            correct_data = 0;
        if (argc >= 6 && strcmp("1", argv[5]) == 0)
            raw = 1;
    }
    int sock_send = sender_prepare(send_iface);
    int sock_recv = receiver_prepare(recv_iface);

    struct sockaddr_in target_addr;
    send_targetaddr(&target_addr);

    size_t bufsize = 8192;
    byte *recv_buf = malloc(bufsize);
    struct msghdr msg;
    recv_msgvar(&msg, recv_buf, bufsize);

    byte send_buf[272];
    // Prepare data
    {
        if (correct_data) {
            unsigned long magic = MAGIC;
            memcpy(send_buf + 0, &magic, sizeof magic);
            unsigned long zero = 0UL;
            memcpy(send_buf + 8, &zero, sizeof zero);
            double data;
            for (int i = 0; i < 16; i++) {
                scanf(" %lf", &data);
                if (raw)
                    memcpy(send_buf + 16 + 8 * i, &data, sizeof data);
                else {
                    unsigned int data_int = (unsigned int)(data * 16384);
                    memcpy(send_buf + 16 + 4 * i, &data_int, sizeof(data_int));
                }
            }
        } else {
            int fd = open("/dev/urandom", O_RDONLY);
            read(fd, send_buf, 272);
            send_buf[0] = send_buf[7] = '!';
            close(fd);
        }
    }

    uint64_t min_time = UINT64_MAX;
    int total_t = 0;
    for (int t = 0; t < 20000000; t++) {
        fprintf(stderr,"t = %d\t", ++total_t);
        uint64_t this_time = measure(sock_send, sock_recv, send_buf, target_addr, &msg, recv_buf);
        min_time = MIN(min_time, this_time);
    }
    uint64_t min_time_2 = min_time;
    for (;;) {
        for (int t = 0; t < check_times; t++) {
            fprintf(stderr,"t = %d\t", ++total_t);
            uint64_t this_time = measure(sock_send, sock_recv, send_buf, target_addr, &msg, recv_buf);
            min_time_2 = MIN(min_time_2, this_time);
        }
        if (min_time_2 == min_time)
            break;
        min_time = min_time_2;
    }

    printf("%ld\n", min_time);
    return 0;
}
