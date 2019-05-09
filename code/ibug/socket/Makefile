CC ?= gcc

.PHONY: all clean

all: send recv

send: send.o util.o
	${CC} -o $@ $^

recv: recv.o util.o
	${CC} -o $@ $^

%.o: %.c config.h
	${CC} ${CFLAGS} -c -o $@ $<

clean:
	rm -f send recv *.o
