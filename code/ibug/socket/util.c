// File: util.c
// Author: iBug

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void errorexit(const char* s) {
    perror(s);
    if (errno)
        exit(1);
}
