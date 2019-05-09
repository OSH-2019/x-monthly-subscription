#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "config.h"
#include "util.h"

typedef unsigned char byte;

int main(int argc, char** argv) {
    sanity_check();

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        errorexit("socket");

    char iface[16] = TARGET_IFACE;
    if (argc >= 2)
        strcpy(iface, argv[1]);
    if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, iface, 1 + strlen(iface)))
        errorexit("setsockopt");
    int optval = -1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &optval, sizeof(int)))
        errorexit("setsockopt");

    struct sockaddr_in target_addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = 0xFFFFFFFF,
        .sin_port = htons(UDP_PORT)
    };

    byte buf[272];
    // Prepare data
    {
        unsigned long magic = MAGIC;
        memcpy(buf + 0, &magic, sizeof magic);
        unsigned long zero = 0UL;
        memcpy(buf + 8, &zero, sizeof zero);
        double data;
        for (int i = 0; i < 32; i++) {
            scanf(" %lf", &data);
            memcpy(buf + 16 + 8 * i, &data, sizeof data);
        }
    }

    int sent = sendto(sock, buf, sizeof(buf), 0, (struct sockaddr*)&target_addr, sizeof(struct sockaddr));
    if (sent != 0)
        errorexit("send");
    printf("%d bytes sent.\n", sent);

    if (close(sock))
        errorexit("close");

    return 0;
}
