#include "fatal_posix.h"
#include "prepare.h"
#include "config.h"
#include <string.h>

#define SIZE 4096
unsigned int *data;
size_t data_len = 0;
size_t data_allocated = SIZE;

void data_append(unsigned int d) {
    if (data_len < data_allocated)
        data[data_len++] = d;
    else {
        data = realloc(data, (data_allocated + SIZE) * sizeof(unsigned int));
        data_allocated += SIZE;
        if (!data) {
            fprintf(stderr, "out of mem\n");
            exit(-1);
        }
        data[data_len++] = d;
    }
}

void prepare_file(char *filename) {
    data = malloc(SIZE * sizeof(unsigned int));
    if (!data) {
        fprintf(stderr, "out of mem\n");
        exit(-1);
    }
    double d;
    if (filename) {
        FILE *f = fopen(filename, "r");
        if (!f) {
            fprintf(stderr, "Cannot open file %s\n", filename);
            exit(-1);
        }
        while (fscanf(f, "%lf", &d) == 1)
            data_append((int)(d * 1024));
    } else {
        while (scanf("%lf", &d) == 1)
            data_append((int)(d * 1024));
    }
}

void sender(int sock_send, struct sockaddr_in* target_addr, char *filename) {
    prepare_file(filename);
    for (size_t i = 0; i + 16 < data_len; i++) {
        byte send_buf[272];
        unsigned long magic = MAGIC;
        memcpy(send_buf + 0, &magic, sizeof magic);
        unsigned long tag = 0UL;
        memcpy(send_buf + 8, &tag, sizeof tag);
        memcpy(send_buf + 16, data + i, sizeof(unsigned int) * 16);
        Sendto(sock_send, send_buf, sizeof(send_buf), 0, (struct sockaddr *) target_addr, sizeof(struct sockaddr));
    }
}

int main(int argc, char** argv) {
    char *filename = NULL;
    require_root();
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s [SEND ITH] [FILENAME]\n", argv[0]);
        exit(-1);
    }

    char send_iface[16] = SOURCE_IFACE;
    if (argc >= 2) {
        strcpy(send_iface, argv[1]);
        if (argc >= 3) {
            filename = strdup(argv[2]);
        }
    }
    int sock_send = sender_prepare(send_iface);
    struct sockaddr_in target_addr;
    send_targetaddr(&target_addr);

    sender(sock_send, &target_addr, filename);
    free(filename);
    //for (size_t i = 0; i < data_len; i++) {
    //    printf("%u ", data[i]);
    //}
    free(data);
    return 0;
}
