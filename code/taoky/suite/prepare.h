#ifndef PREPARE_H
#define PREPARE_H

#include "config.h"

int sender_prepare(char *);
int receiver_prepare(char *);
void send_targetaddr(struct sockaddr_in *);
void recv_msgvar(struct msghdr *, byte *, size_t);

#endif
