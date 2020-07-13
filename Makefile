NAME = sni-shunt
VERSION = 0.2

W = -Wall -Wextra
I = -Isrc
D = -D_POSIX_C_SOURCE=200811L -DVERSION='"${VERSION}"'
CFLAGS = -std=c99 -pedantic $W $I $D
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/man

SRC = src/envfmt.c src/log.c src/mem.c src/str.c src/util.c
HDR = src/envfmt.h src/log.h src/mem.h src/str.h src/util.h
BIN = sni-shunt
OBJ = ${SRC:.c=.o}

all: ${BIN}

${BIN}: ${BIN:=.o} ${OBJ}
	${CC} -o $@ ${BIN}.o ${OBJ}

.c.o:
	${CC} -c -o $@ ${CFLAGS} $<

clean:
	rm -rf ${NAME}-${VERSION} ${BIN} *.o */*.o *.gz

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f ${BIN} ${DESTDIR}${PREFIX}/bin
	cp -f sni-debug.sh ${DESTDIR}${PREFIX}/bin/sni-debug
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	cp -f doc/*.1 ${DESTDIR}${MANPREFIX}/man1

dist: clean
	mkdir -p ${NAME}-${VERSION}
	cp -r README Makefile doc src ${BIN:=.c} ${NAME}-${VERSION}
	tar -cf - ${NAME}-${VERSION} | gzip -c >${NAME}-${VERSION}.tar.gz
