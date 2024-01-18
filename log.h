#ifndef MINIGET_LOG_H
#define MINIGET_LOG_H

#include "stdio.h"

void log_info(const char *msg) {
    fputs(stderr, "[info] ");
    fputs(stderr, msg);
    fputs(stderr, "\n");
}

void log_fatal(const char *msg) {
    fputs(stderr, "[fatal] ");
    die(msg);
}

#endif