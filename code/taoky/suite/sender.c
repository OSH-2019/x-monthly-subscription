#include "fatal_posix.h"
#include "prepare.h"
#include "config.h"
#include <string.h>

#define SIZE 4096
unsigned int *data;
double *data_raw;
size_t data_len = 0;
size_t data_allocated = SIZE;

void data_append(void *d, int raw) {
    if (data_len < data_allocated) {
        if (raw)
            data_raw[data_len++] = *(double *)d;
        else
            data[data_len++] = *(unsigned int *)d;
    }
    else {
        if (raw)
            data_raw = realloc(data_raw, (data_allocated + SIZE) * sizeof(double));
        else
            data = realloc(data, (data_allocated + SIZE) * sizeof(unsigned int));
        data_allocated += SIZE;
        if (!data) {
            fprintf(stderr, "out of mem\n");
            exit(-1);
        }
        if (raw)
            data_raw[data_len++] = *(double *)d;
        else
            data[data_len++] = *(unsigned int *)d;
    }
}

void prepare_file(char *filename, int raw) {
    if (!raw) {
        data = malloc(SIZE * sizeof(unsigned int));
        if (!data) {
            fprintf(stderr, "out of mem\n");
            exit(-1);
        }
    } else {
        data_raw = malloc(SIZE * sizeof(double));
        if (!data_raw) {
            fprintf(stderr, "out of mem\n");
            exit(-1);
        }
    }
    double d;
    if (filename) {
        FILE *f = fopen(filename, "r");
        if (!f) {
            fprintf(stderr, "Cannot open file %s\n", filename);
            exit(-1);
        }
        while (fscanf(f, "%lf", &d) == 1) {
            unsigned int this_d_nraw = (unsigned int)(d * 16384);
            if (!raw)
                data_append(&this_d_nraw, 0);
            else
                data_append(&d, 1);
        }
    } else {
        while (scanf("%lf", &d) == 1) {
            unsigned int this_d_nraw = (unsigned int)(d * 16384);
            if (!raw)
                data_append(&this_d_nraw, 0);
            else
                data_append(&d, 1);
        };
    }
}

void sender(int sock_send, struct sockaddr_in* target_addr, char *filename, int raw) {
    prepare_file(filename, raw);
    for (size_t i = 0; i + 16 < data_len; i++) {
        byte send_buf[272];
        unsigned long magic = MAGIC;
        memcpy(send_buf + 0, &magic, sizeof magic);
        unsigned long tag = 0UL;
        memcpy(send_buf + 8, &tag, sizeof tag);
        if (!raw)
            memcpy(send_buf + 16, data + i, sizeof(unsigned int) * 16);
        else
            memcpy(send_buf + 16, data_raw + i, sizeof(double) * 16);
        Sendto(sock_send, send_buf, sizeof(send_buf), 0, (struct sockaddr *) target_addr, sizeof(struct sockaddr));
    }
}

int main(int argc, char** argv) {
    char *filename = NULL;
    int raw = 0;
    require_root();
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s [SEND ITH] [FILENAME] [RAW (1/0)]\n", argv[0]);
        exit(-1);
    }

    char send_iface[16] = SOURCE_IFACE;
    if (argc >= 2) {
        strcpy(send_iface, argv[1]);
        if (argc >= 3) {
            filename = strdup(argv[2]);
            if (argc >= 4) {
                raw = atoi(argv[3]);
            }
        }
    }
    int sock_send = sender_prepare(send_iface);
    struct sockaddr_in target_addr;
    send_targetaddr(&target_addr);

    sender(sock_send, &target_addr, filename, raw);
    free(filename);
    //for (size_t i = 0; i < data_len; i++) {
    //    printf("%u ", data[i]);
    //}
    free(data);
    return 0;
}
