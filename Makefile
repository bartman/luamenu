# luamenu - dynamic menu
# See LICENSE file for copyright and license details.

include config.mk

TARGET = luamenu

SRC = luamenu.c
OBJ = ${SRC:%.c=%.o}
DEP = ${SRC:%.c=.%.c.dep}
XDEP= $(wildcard ${DEP})

Q=$(if ${V},,@)

all: ${TARGET}

include ${XDEP}

options:
	@echo ${TARGET} build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

${OBJ}: %.o: %.c .%.c.dep
	@echo "  CC $<"
	${Q} ${CC} -c ${CFLAGS} $< -o $@

${DEP}: .%.c.dep: %.c Makefile
	@echo "  DEP $<"
	${Q} ${CC} ${CFLAGS} -MM $< > $@


${OBJ}: config.h config.mk

${TARGET}: ${OBJ}
	@echo "  LINK $@"
	${Q} ${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	${Q} rm -f ${TARGET} ${OBJ} ${DEP} ${TARGET}-${VERSION}.tar.gz *~

dist: clean
	@echo creating dist tarball
	${Q} mkdir -p ${TARGET}-${VERSION}
	${Q} cp -R LICENSE Makefile README config.mk ${TARGET}.1 config.h luamenu_path luamenu_run ${SRC} ${TARGET}-${VERSION}
	${Q} tar -cf ${TARGET}-${VERSION}.tar ${TARGET}-${VERSION}
	${Q} gzip ${TARGET}-${VERSION}.tar
	${Q} rm -rf ${TARGET}-${VERSION}

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	${Q} mkdir -p ${DESTDIR}${PREFIX}/bin
	${Q} cp -f ${TARGET} luamenu_path luamenu_run ${DESTDIR}${PREFIX}/bin
	${Q} chmod 755 ${DESTDIR}${PREFIX}/bin/${TARGET}
	${Q} chmod 755 ${DESTDIR}${PREFIX}/bin/luamenu_path
	${Q} chmod 755 ${DESTDIR}${PREFIX}/bin/luamenu_run
	${Q} echo installing manual page to ${DESTDIR}${MANPREFIX}/man1
	${Q} mkdir -p ${DESTDIR}${MANPREFIX}/man1
	${Q} sed "s/VERSION/${VERSION}/g" < ${TARGET}.1 > ${DESTDIR}${MANPREFIX}/man1/${TARGET}.1
	${Q} chmod 644 ${DESTDIR}${MANPREFIX}/man1/${TARGET}.1

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	${Q} rm -f ${DESTDIR}${PREFIX}/bin/${TARGET} ${DESTDIR}${PREFIX}/bin/luamenu_path
	${Q} rm -f ${DESTDIR}${PREFIX}/bin/${TARGET} ${DESTDIR}${PREFIX}/bin/luamenu_run
	${Q} echo removing manual page from ${DESTDIR}${MANPREFIX}/man1
	${Q} rm -f ${DESTDIR}${MANPREFIX}/man1/${TARGET}.1

.PHONY: all options clean dist install uninstall
