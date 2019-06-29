common_objects = fatal_posix.o prepare.o
measure_objects = measure.o
sender_objects = sender.o
receiver_objects = receiver.o

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=gnu11
TARGET_ETH = enp9s0
REAL_DEV = 

measure : $(measure_objects) $(common_objects)
	$(CC) $(CFLAGS) -o measure $(measure_objects) $(common_objects)

sender : $(sender_objects) $(common_objects)
	$(CC) $(CFLAGS) -o sender $(sender_objects) $(common_objects)

receiver: $(receiver_objects) $(common_objects)
	$(CC) $(CFLAGS) -o receiver $(receiver_objects) $(common_objects)

bpf_dropall: bpf_dropall.c
	clang -O2 -target bpf -c bpf_dropall.c -o bpf_dropall.o

soft_load_dropall: bpf_dropall.c bpf_dropall.o
	sudo ip link set dev $(TARGET_ETH) xdp obj bpf_dropall.o sec xdp

remove:
	sudo ip link set dev $(TARGET_ETH) xdp off
ifdef REAL_DEV
	sudo ip link set dev $(TARGET_ETH) xdpoffload off
endif

.PHONY : clean
clean :
	rm measure $(measure_objects)
