#include "syscall.h"
#include "stdio.h"
#include "log.h"
#include "socket.h"
#include <sys/socket.h>
#include <netinet/in.h>

#define READ_BUFFER_SIZE 1048576
static char read_buffer[READ_BUFFER_SIZE];

#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-reserved-identifier"

void __stack_chk_fail(void) {
    sys_exit(255);
}

#pragma clang diagnostic pop

int read_int(const char **s, int *dst) {
    int buf = 0, cnt = 0;
    while ('0' <= **s && **s <= '9') {
        buf = buf * 10 + (**s - '0');
        ++*s;
        ++cnt;
    }
    if (cnt > 0) {
        *dst = buf;
    }
    return cnt;
}

int parse_ipv4_address(struct sockaddr_in *dst, const char *src) {
    struct in_addr addr = {0};
    for (int i = 0; i < 4; i++) {
        int section;
        if (!read_int(&src, &section)) {
            return i + 1;
        }
        addr.s_addr += (section & 0xFF) << i * 8;
        if (*src == '.') {
            ++src;
        }
    }
    int port = 0;
    if (*src != ':') {
        // use default port
        port = 8001;
    } else {
        ++src;
        if (!read_int(&src, &port)) {
            return 5;
        }
    }
    dst->sin_family = AF_INET;
    dst->sin_addr = addr;
    dst->sin_port = (in_port_t) htons(port);
    for (int i = 0; i < sizeof(dst->sin_zero) / sizeof(dst->sin_zero[0]); ++i) {
        dst->sin_zero[i] = 0;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        puts("Usage: miniget <host:port>\n");
        sys_exit(1);
    }

    int sock;
    if ((sock = sys_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        log_fatal("socket() failed");
    }

    struct sockaddr_in saddr; // server address
    if (parse_ipv4_address(&saddr, argv[1]) != 0) {
        log_fatal("parse_ipv4_address() failed");
    }

    if (sys_connect(sock, (const struct sockaddr *) &saddr, sizeof(saddr)) != 0) {
        log_fatal("connect() failed");
    }

    log_info("connect() success");

    // this program uses a simple stream-based protocol
    // [ length: 4bytes ] [ payload: `length` bytes ]

    char len_buf[4];
    int n = sizeof(len_buf);
    while (n > 0) {
        long n_read;
        if ((n_read = sys_read(sock, len_buf + (sizeof(len_buf) - n), n)) < 0) {
            log_fatal("read() payload length failed");
        }
        n -= (int) n_read;
    }
    uint32_t len = ntohl(*((uint32_t *) len_buf));

    while (len > 0) {
        long read_size = (len > READ_BUFFER_SIZE) ? READ_BUFFER_SIZE
                                                  : (int) len;
        long n_read;
        if ((n_read = sys_read(sock, read_buffer, read_size)) < 0) {
            log_fatal("read() payload failed");
        }
        len -= n_read;

        long n_written, total_written = 0;

        while (n_read > 0) {
            if ((n_written = sys_write(
                    stdout, read_buffer + total_written,
                    n_read)) < 0) {
                log_fatal("write() payload to stdout failed");
            }
            total_written += n_written;
            n_read -= n_written;
        }
    }

    return 0;
}