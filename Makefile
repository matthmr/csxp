CC?=gcc
CCFLAG?=
GZIP?=gzip
PREFIX?=/usr/local

default: csxp

install: csxp man config.h
	@echo [ .. ] Installing csxp
	mv csxp.1.gz ${PREFIX}/man/man1/csxp.1.gz
	mv csxp ${PREFIX}/bin/csxp

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
	${GZIP} -c csxp.1 > csxp.1.gz ${PREFIX}/man/man1/csxp.1.gz
