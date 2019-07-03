#include "fatal_posix.h"
#include "prepare.h"
#include "config.h"
#include <stdbool.h>
#include <assert.h>

struct packet {
    unsigned long magic;
    unsigned long tag;
    union {
        unsigned int data[16];
        double data_raw[16];
    };
};

int main(int argc, char** argv) {
    int debug = 0;
    int raw = 0;
    require_root();
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s [RECV ITH] [DEBUG (0/1/2)] [RAW (0/1)]\n", argv[0]);
        exit(-1);
    }

    char recv_iface[16] = TARGET_IFACE;
    if (argc >= 2) {
        strcpy(recv_iface, argv[1]);
        if (argc >= 3) {
            debug = atoi(argv[2]);
            if (argc >= 4) {
                raw = atoi(argv[3]);
            }
        }
    }
    int sock_recv = receiver_prepare(recv_iface);
    size_t bufsize = 8192;
    byte *recv_buf = malloc(bufsize);
    struct msghdr msg;
    recv_msgvar(&msg, recv_buf, bufsize);
    printf("Listening...\n");
    for (;;) {
        int received = Recvmsg(sock_recv, &msg, 0);
        if (received == 0 || recv_buf[28] != '!' || recv_buf[35] != '!') {
            if (debug == 2 && received != 0) {
                printf("received wrong packet.\n");
            }
        } else {
            struct packet *packet = recv_buf + 28;
            if (debug == 2)
                hexdump(recv_buf, received);
            if (debug >= 1) {
                printf("magic: %lu\n", packet->magic);
                assert(packet->magic == 0x216C7174786A7A21UL);
                for (int i = 0; i < 16; i++) {
                    if (!raw)
                        printf("%u ", packet->data[i]);
                    else
                        printf("%lf ", packet->data_raw[i]);
                }
                printf("\n");
            }
            if (!raw) {
                unsigned long long mean = 0;
                unsigned long long var = 0;
                for (int i = 0; i < 8; i++) {
                    mean += packet->data[i];
                    var += (unsigned long long)packet->data[i] * packet->data[i];
                }
                mean >>= 3; var >>= 3;
                var -= mean * mean;
                unsigned int mean2 = (unsigned int) mean;
                const unsigned int e = 0x666;
                const unsigned int e2 = 2684191;
                unsigned int tot = 0;
                for (int i = 8; i < 16; ++i) {
                    tot += packet->data[i] >= mean2 ? (
                            packet->data[i] - mean2 >= e ? 1u : 0u) : (
                                mean2 - packet->data[i] >= e ? 1u : 0u);
                }
                bool flag = false;
                if ((unsigned long long)tot * e2 > (var << 3)) 
                    flag = true;
                if ((flag && packet->tag == 1ull) || (!flag && packet->tag == 2ull)) {
                    puts("OK!");
                } else {
                    puts("AAAAA!");
                }
            } else {
                double mean = 0, var = 0;
                for (int i = 0; i < 8; i++) {
                    mean += packet->data_raw[i];
                    var += packet->data_raw[i] * packet->data_raw[i];
                }
                mean /= 8; var /= 8;
                var -= mean * mean;
                const double e = 0.1;
                unsigned int tot = 0;
                for (int i = 8; i < 16; ++i) {
                    tot += packet->data_raw[i] >= mean ? 
                            (packet->data_raw[i] - mean >= e ? 1u : 0u) : (
                                mean - packet->data_raw[i] >= e ? 1u : 0u);
                }
                bool flag = false;
                if (tot * e * e > (var * 8.)) 
                    flag = true;
                if ((flag && packet->tag == 1ull) || (!flag && packet->tag == 2ull)) {
                    puts("OK!");
                } else {
                    puts("AAAAA!");
                }
            }
        }
    }
    return 0;
}
