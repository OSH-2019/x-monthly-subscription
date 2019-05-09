// File: util.c
// Author: iBug

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

void errorexit(const char* s) {
    perror(s);
    if (errno)
        exit(1);
}

void hexdump(const unsigned char* s, size_t len) {
    int x = 0;
    while (x + 16 <= len) {
        printf("%08X  ", x);
        for (int i = x; i < x + 8; i++)
            printf("%02X ", s[i]);
        for (int i = x + 8; i < x + 16; i++)
            printf(" %02X", s[i]);
        printf("  |");
        for (int i = x; i < x + 16; i++)
            putchar((s[i] >= 32 && s[i] < 127) ? s[i] : '.');
        printf("|\n");
        x += 16;
    }
    if (x < len) {
        printf("%08X  ", x);
        for (int i = x; i < x + 8 && i < len; i++)
            printf("%02X ", s[i]);
        for (int i = x + 8; i < len; i++)
            printf(" %02X", s[i]);
        int padding = 2 + 3 * (16 + x - len);
        char t[8];
        snprintf(t, sizeof(t), "%%%ds|", padding);
        printf(t, "");
        for (int i = x; i < len; i++)
            putchar((s[i] >= 32 && s[i] < 127) ? s[i] : '.');
        printf("|\n");
    }
    printf("%08zX\n", len);
}

void sanity_check(void) {
    if (getuid() || geteuid()) {
        fprintf(stderr, "Need root to proceed\n");
        exit(1);
    }
}
