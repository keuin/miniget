#ifndef MINIGET_SOCKET_H
#define MINIGET_SOCKET_H

#include <sys/socket.h>
#include "syscall.h"

int sys_socket(int family, int type, int protocol) {
    return (intptr)
            syscall3(
                    (uintptr) SYS_socket,
                    (void *) (uintptr) family,
                    (void *) (uintptr) type,
                    (void *) (uintptr) protocol
            );
}

int sys_connect(int fd, __CONST_SOCKADDR_ARG addr, socklen_t len) {
    return (intptr)
            syscall3(
                    (uintptr) SYS_connect,
                    (void *) (uintptr) fd,
                    (void *) (uintptr) addr,
                    (void *) (uintptr) len
            );
}

#endif