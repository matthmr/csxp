CC?=gcc
CCFLAG?=
CTAGS?=ctags
GZIP?=gzip
PREFIX?=/usr/local

default: csxp

tags:
	@echo [ .. ] Updating tags file
	${CTAGS} -R .

install: csxp man config.h
	@echo [ .. ] Installing csxp
	install -Dm755 csxp ${PREFIX}/bin/csxp
	install -Dm544 config.h ${PREFIX}/include/csxp/config.h
	install -Dm544 csxp.1.gz ${PREFIX}/man/man1/csxp.1.gz

csxp: csxp.o config.h
	@echo [ .. ] Linking to 'csxp'
	${CC} ${CCFLAG} csxp.o -o csxp

csxp.o: csxp.c csxp.h config.h
	@echo [ .. ] Compiling 'csxp.o'
	${CC} -c ${CCFLAG} csxp.c -o csxp.o

clean:
	@echo [ .. ] Cleaning compilation junk
	rm -f *.gz *.o *.a

man:
	@echo [ .. ] Compressing 'csxp' man page
	${GZIP} -c csxp.1 > csxp.1.gz

.PHONY: csxp tags
