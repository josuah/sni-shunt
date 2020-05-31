include content.mk

CFLAGS = -Isrc -Wall -Wextra -std=c99 -pedantic -D_POSIX_C_SOURCE=200811L

all: ${bin}

clean:
	rm -rf *.o ${bin}

${bin}: ${bin:=.o} ${src:.c=.o}
	${CC} -o $@ ${bin}.o ${src:.c=.o}

.c.o:
	${CC} -c -o $@ ${CFLAGS} $<
