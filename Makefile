NAME = sni-shunt
VERSION = 0.2

SRC = src/str.c src/log.c src/mem.c src/compat/strsep.c src/compat/strlcpy.c \
  src/envfmt.c

HDR = src/mem.h src/envfmt.h src/compat.h src/str.h src/log.h

BIN = sni-shunt

OBJ = ${SRC:.c=.o}

LIB =

W = -Wall -Wextra -std=c99 --pedantic
I = -I./src
L =
D = -D_POSIX_C_SOURCE=200811L -DVERSION='"${VERSION}"'
CFLAGS = $I $D $W -g
LDFLAGS = $L -static
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/man

all: ${BIN}

.c.o:
	${CC} -c ${CFLAGS} -o $@ $<

${OBJ}: ${HDR}
${BIN}: ${OBJ} ${BIN:=.o}
	${CC} ${LDFLAGS} -o $@ $@.o ${OBJ} ${LIB}

clean:
	rm -rf *.o */*.o ${BIN} ${NAME}-${VERSION} *.gz

install:
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -rf bin ${BIN} ${DESTDIR}${PREFIX}/bin
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	cp -rf *.1 ${DESTDIR}${MANPREFIX}/man1

dist: clean
	mkdir -p ${NAME}-${VERSION}
	cp -r README.md Makefile bin ${SRC} ${NAME}-${VERSION}
	tar -cf - ${NAME}-${VERSION} | gzip -c >${NAME}-${VERSION}.tar.gz

site: dist
	notmarkdown README.md | notmarkdown-html | cat .head.html - >index.html
	notmarkdown README.md | notmarkdown-gph | cat .head.gph - >index.gph
	sed -i "s/VERSION/${VERSION}/g" index.*
