#include "fatal_posix.h"
#include "prepare.h"
#include "config.h"
#include <assert.h>

struct packet {
    unsigned long magic;
    unsigned long tag;
    unsigned int data[16];
};

int main(int argc, char** argv) {
    int debug = 0;
    require_root();
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s [RECV ITH] [DEBUG (0/1/2)]\n", argv[0]);
        exit(-1);
    }

    char recv_iface[16] = TARGET_IFACE;
    if (argc >= 2) {
        strcpy(recv_iface, argv[1]);
        if (argc >= 3)
            debug = atoi(argv[2]);
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
                    printf("%u ", packet->data[i]);
                }
                printf("\n");
            }
            printf("tag: %lu\n", packet->tag);
        }
    }
    return 0;
}
