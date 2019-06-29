#ifndef PREPARE_H
#define PREPARE_H

#include "config.h"
#include <string.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>


int sender_prepare(char *);
int receiver_prepare(char *);
void send_targetaddr(struct sockaddr_in *);
void recv_msgvar(struct msghdr *, byte *, size_t);
void require_root();
void hexdump(const unsigned char*, size_t);

#endif
