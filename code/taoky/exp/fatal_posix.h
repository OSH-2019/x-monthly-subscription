#ifndef FATAL_H
#define FATAL_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#define FATAL(msg) perror(msg); exit(-1);

typedef void (*sighandler_t)(int);

// int Open(const char *pathname, int flags);
// int Open(const char *pathname, int flags, mode_t mode);
int Close(int fd);
int Dup2(int fildes, int fildes2);
int Pipe(int fildes[2]);
ssize_t Write(int fd, const void *buf, size_t count);
int Mkstemp(char *Template);

int Socket(int domain, int type, int protocol);
int Bind(int domain, const struct sockaddr *address, socklen_t address_len);
int Listen(int socket, int backlog);
int Stat(const char *pathname, struct stat *statbuf);
sighandler_t Signal(int signum, sighandler_t handler);
int Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t Recvmsg(int sockfd, struct msghdr *msg, int flags);

#endif