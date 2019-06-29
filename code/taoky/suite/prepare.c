#include "fatal_posix.h"
#include "config.h"
#include "prepare.h"

int sender_prepare(char *send_iface) {
    int sock_send = Socket(AF_INET, SOCK_DGRAM, 0);
    int optval = -1;
    Setsockopt(sock_send, SOL_SOCKET, SO_BINDTODEVICE, send_iface, 1 + strlen(send_iface));
    Setsockopt(sock_send, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &optval, sizeof(int));
    return sock_send;
}

int receiver_prepare(char *recv_iface) {
    int sock_recv = Socket(AF_PACKET, SOCK_DGRAM, 0);
    Setsockopt(sock_recv, SOL_SOCKET, SO_BINDTODEVICE, recv_iface, 1 + strlen(recv_iface));
    unsigned ifacenum = if_nametoindex(recv_iface);
    struct sockaddr_ll sll = {
        .sll_family = AF_PACKET,
        .sll_ifindex = ifacenum,
        .sll_protocol = htons(3)
    };
    Bind(sock_recv, (struct sockaddr*)&sll, sizeof sll);
    return sock_recv;
}

void send_targetaddr(struct sockaddr_in *target_addr) {
    target_addr->sin_family = AF_INET;
    target_addr->sin_addr.s_addr = 0xFFFFFFFF;
    target_addr->sin_port = htons(UDP_PORT);
}

void recv_msgvar(struct msghdr *msg, byte *recv_buf, size_t bufsize) {
    size_t cbufsize = 1024;
    byte *cbuf = malloc(cbufsize);
    struct iovec *iov = malloc(sizeof(struct iovec));
    iov->iov_base = recv_buf;
    iov->iov_len = bufsize;
    msg->msg_name = NULL;
    msg->msg_namelen = (socklen_t)0;
    msg->msg_iov = iov;
    msg->msg_iovlen = 1;
    msg->msg_control = cbuf;
    msg->msg_controllen = (socklen_t)cbufsize;
    msg->msg_flags = 0;
}

void hexdump(const unsigned char* s, size_t len) {
    size_t x = 0;
    while (x + 16 <= len) {
        printf("%08lX  ", x);
        for (size_t i = x; i < x + 8; i++)
            printf("%02X ", s[i]);
        for (size_t i = x + 8; i < x + 16; i++)
            printf(" %02X", s[i]);
        printf("  |");
        for (size_t i = x; i < x + 16; i++)
            putchar((s[i] >= 32 && s[i] < 127) ? s[i] : '.');
        printf("|\n");
        x += 16;
    }
    if (x < len) {
        printf("%08lX  ", x);
        for (size_t i = x; i < x + 8 && i < len; i++)
            printf("%02X ", s[i]);
        for (size_t i = x + 8; i < len; i++)
            printf(" %02X", s[i]);
        int padding = 2 + 3 * (16 + x - len);
        char t[8];
        snprintf(t, sizeof(t), "%%%ds|", padding);
        printf(t, "");
        for (size_t i = x; i < len; i++)
            putchar((s[i] >= 32 && s[i] < 127) ? s[i] : '.');
        printf("|\n");
    }
    printf("%08zX\n", len);
}
void require_root() {
    if (getuid() || geteuid()) {
        fprintf(stderr, "Need root to proceed\n");
        exit(-1);
    }
}
