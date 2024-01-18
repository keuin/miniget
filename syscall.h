#ifndef MINIGET_SYSCALL_H
#define MINIGET_SYSCALL_H

// amd64 syscall

#define SYS_read 0
#define SYS_write 1
#define SYS_stat  4
#define SYS_exit  60
#define SYS_socket 41
#define SYS_connect 42

typedef unsigned long int uintptr; /* size_t */
typedef long int intptr; /* ssize_t */
typedef uintptr size_t;

void *syscall1(
        uintptr number,
        void *arg1
);

void *syscall2(
        uintptr number,
        void *arg1,
        void *arg2
);

void *syscall3(
        uintptr number,
        void *arg1,
        void *arg2,
        void *arg3
);

void *syscall4(
        uintptr number,
        void *arg1,
        void *arg2,
        void *arg3,
        void *arg4
);

void *syscall5(
        uintptr number,
        void *arg1,
        void *arg2,
        void *arg3,
        void *arg4,
        void *arg5
);

static
intptr sys_write(int fd, void const *data, uintptr nbytes) {
    return (intptr)
            syscall5(
                    SYS_write,
                    (void *) (intptr) fd,
                    (void *) data,
                    (void *) nbytes,
                    0, /* ignored */
                    0  /* ignored */
            );
}

static
intptr sys_read(int fd, void *buf, unsigned long n) {
    return (intptr)
            syscall3(
                    SYS_read,
                    (void *) (intptr) fd,
                    (void *) buf,
                    (void *) n
            );
}

static void sys_exit(int errno) {
    syscall1(SYS_exit, (void *) (uintptr) errno);
}

#endif