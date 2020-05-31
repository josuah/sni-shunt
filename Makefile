CFLAGS = -Isrc -Wall -Wextra -std=c99 -pedantic -D_POSIX_C_SOURCE=200811L

bin = sni-shunt
obj = src/log.o src/util.o

all: ${bin}

clean:
	rm -rf *.o ${bin}

sni-shunt: sni-shunt.o ${obj}
	${CC} -o $@ sni-shunt.o ${obj}

.c.o:
	${CC} -c -o $@ ${CFLAGS} $<
