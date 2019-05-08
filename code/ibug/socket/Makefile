CC ?= gcc

.PHONY: all clean

all: send

send: send.o util.o
	${CC} -o $@ $^

%.o: %.c config.h
	${CC} ${CFLAGS} -c -o $@ $<

clean:
	rm -f send *.o
