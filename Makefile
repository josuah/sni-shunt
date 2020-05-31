include content.mk
include config.mk

all: ${bin}

clean:
	rm -rf *.o ${bin}

install:
	mkdir -p "${PREFIX}/bin"
	cp ${bin} "${PREFIX}/bin"
	cp sni-debug.sh "${PREFIX}/bin/sni-debug"
	mkdir -p "${MANPREFIX}/man1"
	cp doc/*.1 "${MANPREFIX}/man1"

${bin}: ${bin:=.o} ${src:.c=.o}
	${CC} -o $@ ${bin}.o ${src:.c=.o}

.c.o:
	${CC} -c -o $@ ${CFLAGS} $<
