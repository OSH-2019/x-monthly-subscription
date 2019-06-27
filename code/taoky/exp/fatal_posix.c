#include "fatal_posix.h"

// int Open(const char *pathname, int flags) {
//     int ret = open(pathname, flags);
//     if (ret == -1) {
//         FATAL("open()");
//     }
//     return ret;
// }

// int Open(const char *pathname, int flags, mode_t mode) {
//     int ret = open(pathname, flags, mode);
//     if (ret == -1) {
//         FATAL("open()");
//     }
//     return ret;
// }

int Close(int fd) {
    int ret = close(fd);
    if (ret == -1) {
        FATAL("close()");
    }
    return ret;
}

int Dup2(int fildes, int fildes2) {
    int ret = dup2(fildes, fildes2);
    if (ret == -1) {
        FATAL("dup2()");
    }
    return ret;
}

int Pipe(int fildes[2]) {
    int ret = pipe(fildes);
    if (ret == -1) {
        FATAL("pipe()");
    }
    return ret;
}

ssize_t Write(int fd, const void *buf, size_t count) {
    ssize_t ret = write(fd, buf, count);
    if (ret == -1) {
        FATAL("write()");
    }
    return ret;
}

int Mkstemp(char *Template) {
    int ret = mkstemp(Template);
    if (ret == -1) {
        FATAL("mkstemp()");
    }
    return ret;
}

int Socket(int domain, int type, int protocol) {
    int ret = socket(domain, type, protocol);
    if (ret == -1) {
        FATAL("socket()");
    }
    return ret;
}

int Bind(int domain, const struct sockaddr *address, socklen_t address_len) {
    int ret = bind(domain, address, address_len);
    if (ret == -1) {
        FATAL("bind()");
    }
    return ret;
}

int Listen(int socket, int backlog) {
    int ret = listen(socket, backlog);
    if (ret == -1) {
        FATAL("listen()");
    }
    return ret;
}

int Stat(const char *pathname, struct stat *statbuf) {
    int ret = stat(pathname, statbuf);
    if (ret == -1) {
        FATAL("stat()");
    }
    return ret;
}

sighandler_t Signal(int signum, sighandler_t handler) {
    sighandler_t ret = signal(signum, handler);
    if (ret == SIG_ERR) {
        FATAL("signal()");
    }
    return ret;
}

int Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen) {
    int ret = setsockopt(sockfd, level, optname, optval, optlen);
    if (ret == -1) {
        FATAL("setsockopt()");
    }
    return ret;
}

ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen) {
    int ret = sendto(sockfd, buf, len, flags, dest_addr, addrlen);
    if (ret == -1) {
        FATAL("sendto()");
    }
    return ret;
}

ssize_t Recvmsg(int sockfd, struct msghdr *msg, int flags) {
    int ret = recvmsg(sockfd, msg, flags);
    if (ret == -1) {
        FATAL("recvmsg()");
    }
    return ret;
}