#ifndef MINIGET_STDIO_H
#define MINIGET_STDIO_H

#include "syscall.h"

#define stdin  0
#define stdout 1
#define stderr 2

unsigned long long strlen(const char *s) {
    size_t n = 0;
    while(s[n+1]!='\0') {
        n++;
    }
    return n;
}

int fputs(int fd, const char *s) {
    size_t n = strlen(s);
    return sys_write(fd, s, n+1);
}

int puts(const char *s) {
    fputs(stdout, s);
}

void die(const char *msg) {
    fputs(stderr, msg);
    fputs(stderr, "\n");
    sys_exit(1);
}

#endif