.PHONY: all clean dist install uninstall
.SUFFIXES: .c .h .o .out .exe

include config.mk

A_OUT = ${EXENAME}${EXEEXT}

SRC = ${STARTUP} ${GRAPHICS} memory.c objects.c
OBJ = ${SRC:.c=.o} # logo.o

all: ${A_OUT} ${EXTRA}

clean:
	rm -f ${A_OUT} ${EXTRA_CLEAN} *.o

dist: clean
	mkdir -p ${EXENAME}-${VERSION}
	cp -R makefile config.mk config.def.h memory.h objects.h graphics.h \
		${EXENAME}-${VERSION}
	tar -cf ${EXENAME}-${VERSION}.tar \
		${EXENAME}-${VERSION}
	gzip ${EXENAME}-${VERSION}.tar
	rm -rf ${EXENAME}-${VERSION}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f ${BIN} ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/${A_OUT}

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/${A_OUT}

${A_OUT}: ${OBJ}
	${CC} -o $@ $^ ${LDFLAGS}

${OBJ}: config.h config.mk

config.h:
	cp config.def.h $@

.c.o:
	${CC} ${CFLAGS} -c -o $@ $<
