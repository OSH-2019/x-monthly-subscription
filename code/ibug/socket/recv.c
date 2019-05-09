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

#include "config.h"
#include "util.h"

typedef unsigned char byte;

int main(int argc, char** argv) {
    int sock = socket(AF_PACKET, SOCK_DGRAM, 0);
    if (sock < 0)
        errorexit("socket");

    char iface[16] = SOURCE_IFACE;
    if (argc >= 2)
        strcpy(iface, argv[1]);
    unsigned ifacenum = if_nametoindex(iface), opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, iface, 1 + strlen(iface)))
        errorexit("setsockopt");

    struct sockaddr_ll sll = {
        .sll_family = AF_PACKET,
        .sll_ifindex = ifacenum,
        .sll_protocol = htons(3)
    };
    if (bind(sock, (struct sockaddr*)&sll, sizeof sll))
        errorexit("sock");

    size_t bufsize = 8192, cbufsize = 1024;
    byte *buf = malloc(bufsize), *cbuf = malloc(cbufsize);
    unsigned long magic = MAGIC;
    struct iovec iov = {
        .iov_base = buf,
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
    puts("Starting to receive...");
    while (1) {
        received = recvmsg(sock, &msg, 0);
        if (received < 0)
            errorexit("receive");
        if (received == 0)
            break;
        if (buf[28] != '!' || buf[35] != '!') // Bad magic
            continue;
        printf("%d bytes received.\n", received);
        hexdump(buf, received);

        // Save packet for debugging
        FILE *fp = fopen("packet", "wb");
        fwrite(buf, received, 1, fp);
        fclose(fp);
    }

    if (close(sock))
        errorexit("close");

    free(buf);
    free(cbuf);
    return 0;
}
